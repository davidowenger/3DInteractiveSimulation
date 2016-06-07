#include "Common.h"

namespace Demo
{

/*************************/
/****** Strategie ********/
/*************************/

Strategie::Strategie()
{
}


Strategie::Strategie(IntCode type, bool isFireActive, Cell* dest, CellQueue* path) :
    mType(type), mIsFireActive(isFireActive), mDest(dest), mPath(path),
    mState(STATE_INIT)
{
}

/*******************************/
/****** ClientStrategie ********/
/*******************************/

// Gameplay:
// Your squad has to go to the central point, where it you can create new units
// You have to go throught the obstacles and opponents fire to achieve this goal
// You can restore your units by going to your casern

ClientStrategie::ClientStrategie(
    Rendering* const rendering, Wrapper* const wrapper,
    GameStaticData* const gameStaticData, GameDynData* const gameDynData,
    ClientSquad* const squad, ClientSteering* const steering
) :
    mRendering(rendering), mWrapper(wrapper),
    mGameStaticData(gameStaticData), mGameDynData(gameDynData),
    mSteering(steering),
    mSquad(squad), mSquadData(squad->mSquadData),
    mLeader(squad->mLeader), mLeaderId(squad->mLeader->mId), maUnit(squad->maUnit),
    maPlayerDynData(gameDynData->aPlayerDynData),
    maRessourceStaticData(gameStaticData->aRessourceStaticData)
{
}

ClientStrategie::~ClientStrategie()
{
    // delete
}

void ClientStrategie::init()
{
    mLeaderStack    .push(new Strategie(WON));
    mLeaderStack    .push(getStrategie(WIN));
    mSquadStack     .push(new Strategie(FOLLOW));

    mSquadData->currentLeaderStrat = mLeaderStack.top();
    mSquadData->currentSquadStrat  = mSquadStack.top();
}

Strategie* ClientStrategie::getStrategie(IntCode type, bool isFireActive, Cell* dest, CellQueue* path)
{
    if (!dest) {
        dest = maRessourceStaticData[type]->cell;
        path = mSteering->getPath(maPlayerDynData[mLeaderId]->cell, dest);
    }
    if (isFireActive) {
        mLeader->mTarget        = maPlayerDynData[mSquad->getStrongestLeaderInCell( path->size() ? path->front() : dest )];
        mLeader->mTargetTime    = 0;
    }
    return new Strategie(type, isFireActive, dest, path);
}

void ClientStrategie::advance()
{
    this                            ->advanceSquad();
    this                            ->collectTargetUnits();
}

void ClientStrategie::advanceAI()
{
    Strategie*  oldStrat    = 0;
    CellQueue*  path        = mSquadData->currentLeaderStrat->mPath;
    
    if (path->size() && path->front() == maPlayerDynData[mLeaderId]->cell) {
        path->pop();
    }
    if (isLeaderOk(mSquadData->currentLeaderStrat) || isLeaderCanceled(mSquadData->currentLeaderStrat)) {
        oldStrat        = mSquadData->currentLeaderStrat;
        mLeaderStack     .pop();
    }
    if (isLeaderCanceled(mSquadData->currentLeaderStrat) || isLeaderDelayed(mSquadData->currentLeaderStrat)) {
        mLeaderStack     .push(getLeaderStrategie(mSquadData->currentLeaderStrat));
    }
    mSquadData->currentLeaderStrat = mLeaderStack.top();

    if (oldStrat) {
        delete oldStrat;
    }
}

void ClientStrategie::advanceSquad()
{
    Strategie* oldStrat = 0;

    if (isSquadOk(mSquadData->currentSquadStrat) || isSquadCanceled(mSquadData->currentSquadStrat)) {
        oldStrat        = mSquadData->currentSquadStrat;
        mSquadStack      .pop();
    }
    if (isSquadCanceled(mSquadData->currentSquadStrat) || isSquadDelayed(mSquadData->currentSquadStrat)) {
        mSquadStack      .push(getSquadStrategie(mSquadData->currentSquadStrat));
    }
    mSquadData->currentSquadStrat  = mSquadStack.top();

    if (oldStrat) {
        delete oldStrat;
    }
}

void ClientStrategie::collectTargetUnits()
{
    // Current target or agressor unit, if any
    mSquadData->currentTargetVec->clear();

    if (mLeader->mTarget) {
        mSquadData->currentTargetVec->push_back(mLeader->mTarget->aUnit);
    }
    for (IntCode index = 0 ; index < NB_OF_UNITS ; index++) {
        if (maUnit[index]->mAgressor) {
            mSquadData->currentTargetVec->push_back(maUnit[index]->mAgressor->aUnit);
        }
    }
}

Strategie* ClientStrategie::getLeaderStrategie(Strategie* prevStrat)
{
    Strategie*  strat;

    CellQueue*  path    = prevStrat->mPath;
    Cell*       dest    = prevStrat->mDest;
    Cell*       next    = dest;
    
    if (path->size()) {
        next = path->front();
    }
    if (
        mSquad->hasPeace() &&
        mSquad->isReachable(dest) &&
        mSquad->isFree(next)
    ) {
        strat = getStrategie(prevStrat->mType, false, dest, path);
    } else if (
        mSquad->hasRessource4Attack(next, AMMO) &&
        mSquad->hasRessource4Attack(next, HEALTH) &&
        mSquad->isReachable(dest)
    ) {
        strat = getStrategie(ATTACK, true, dest, path);
    } else if (
        !mSquad->isFree(next) &&
        path->size() >= 3 &&
        mSquad->isReachable(dest)
    ) {
        strat        = new Strategie(*prevStrat);
        strat->mPath = mSteering->getPath(maPlayerDynData[mLeaderId]->cell, dest, next);
    } else if (
        mSquad->hasRessource(HEALTH) &&
        mSquad->isReachable(AMMO)
    ) {
        strat = getStrategie(AMMO, true);
    } else if (
        mSquad->isReachable(HEALTH)
    ) {
        strat = getStrategie(HEALTH, true);
    } else {
        strat = getStrategie(FUEL, true);
    }
    return strat;
}

Strategie* ClientStrategie::getSquadStrategie(Strategie* prevStrat)
{
    Strategie* strat;
    
    if (
        prevStrat->mType != GOTO &&
        mLeader->isFireActive() &&
        mLeader->isMoving() &&
        mSquad->hasRessource(AMMO) &&
        mSquad->hasRessource(HEALTH) &&
        mSquad->hasRessource(FUEL)
    ) {
        strat = new Strategie(GOTO, true);
    } else if (
        prevStrat->mType != ATTACK_AND_DEFEND &&
        mLeader->isFireActive() &&
        !mLeader->isMoving() &&
        mSquad->hasRessource(AMMO) &&
        mSquad->hasRessource(HEALTH) &&
        mSquad->hasRessource(FUEL)
    ) {
        strat = new Strategie(ATTACK_AND_DEFEND, true);
    } else {
        strat = new Strategie(FOLLOW, false);
    }
    return strat;
}

bool ClientStrategie::hasWon()
{
    return mSquadData->currentLeaderStrat->mType == WON;
}

bool ClientStrategie::isLeaderCanceled(Strategie* strat)
{
    bool isCanceled = true;

    switch (strat->mType)
    {
    case WIN:
        isCanceled = false;
        break;

    case ALT:
        isCanceled =
            !mSquad->hasRessource(FUEL) ||
            !mSquad->hasPeace();
        break;

    case ATTACK:
        isCanceled =
            !mSquad->hasRessource4Attack(strat->mPath->size() ? strat->mPath->front() : strat->mDest, AMMO)   ||
            !mSquad->hasRessource4Attack(strat->mPath->size() ? strat->mPath->front() : strat->mDest, HEALTH) ||
            !mSquad->hasRessource(HEALTH) ||
            !mSquad->hasRessource(FUEL);
        break;

    case AMMO:
    case HEALTH:
    case FUEL:
        isCanceled = false;
        break;
    }
    return isCanceled;
}

bool ClientStrategie::isLeaderDelayed(Strategie* strat)
{
    bool isDelayed = true;

    switch (strat->mType)
    {
    case WIN:
        isDelayed = !mSquad->isFree(strat->mPath->size() ? strat->mPath->front() : strat->mDest );
        break;

    case ALT:
    case ATTACK:
        isDelayed = false;
        break;

    case AMMO:
    case HEALTH:
    case FUEL:
        isDelayed =
            !mSquad->isFree(strat->mPath->size() ? strat->mPath->front() : strat->mDest ) ||
            strat->mType + 1 <= FUEL && !mSquad->hasRessource(strat->mType + 1) ||
            strat->mType + 2 <= FUEL && !mSquad->hasRessource(strat->mType + 2);
        break;
    }
    return isDelayed;
}

bool ClientStrategie::isLeaderOk(Strategie* strat)
{
    bool isOk = true;

    switch (strat->mType)
    {
    case WIN:
        isOk = false;
        break;

    case ALT:
        isOk = mSquad->isFree(strat->mPath->size() ? strat->mPath->front() : strat->mDest );
        break;

    case ATTACK:
        isOk =
            mSquad->isFree(strat->mPath->size() ? strat->mPath->front() : strat->mDest ) ||
            !mLeader->mTarget ||
            mLeader->mTarget->aRessource[HEALTH] == 0;
        break;

    case AMMO:
    case HEALTH:
    case FUEL:
        isOk = mSquad->hasRessource(strat->mType);
        break;
    }
    return isOk;
}

bool ClientStrategie::isSquadCanceled(Strategie* strat)
{
    bool isCanceled = true;

    switch (strat->mType)
    {
    case FOLLOW:
        isCanceled = false;
        break;

    case GOTO:
        isCanceled = mLeader->isFireActive() && !mLeader->isMoving();
        break;

    case ATTACK_AND_DEFEND:
        isCanceled = mLeader->isFireActive() && mLeader->isMoving();
        break;
    }
    return isCanceled;
}

bool ClientStrategie::isSquadDelayed(Strategie* strat)
{
    bool isDelayed = true;

    switch (strat->mType)
    {
    case FOLLOW:
        isDelayed = mLeader->isFireActive();
        break;

    case GOTO:
    case ATTACK_AND_DEFEND:
        isDelayed = false;
        break;
    }
    return isDelayed;
}

bool ClientStrategie::isSquadOk(Strategie* strat)
{
    bool isOk = true;

    switch (strat->mType)
    {
    case FOLLOW:
        isOk = false;
        break;
        
    case GOTO:
        isOk = !mLeader->isFireActive();
        break;

    case ATTACK_AND_DEFEND:
        isOk = !mLeader->isFireActive();
        break;
    }
    return isOk;
}

} // END namespace
