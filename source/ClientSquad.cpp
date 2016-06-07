#include "Common.h"

namespace Demo
{

ClientSquad::ClientSquad(
    Rendering* const rendering, Wrapper* const wrapper,
    GameStaticData* const gameStaticData, GameDynData* const gameDynData,
    const Coord& coord, const IntVal playerId
) :
    mRendering(rendering), mWrapper(wrapper),
    mGameStaticData(gameStaticData), mGameDynData(gameDynData),
    mPlayerId(playerId),
    mLeaderId(playerId*NB_OF_UNITS),
    mSquadData(new SquadData(new PlayerDynDataVec(), new Strategie(), new Strategie())),
    maPlayerDynData(gameDynData->aPlayerDynData),
    maRessourceStaticData(gameStaticData->aRessourceStaticData),
    mLeader(0)
{
    // Create a struct for the squad data
    mLeader   = new ClientUnitLeader(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId, coord, new Input());
    maUnit[0] = mLeader;
    
    if (mPlayerId == 0) {
        maUnit[1] = new ClientUnit(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId + 1, coord + Vec3(-35.0f ,0.0f, +35.0f), new Input());
        maUnit[2] = new ClientUnit(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId + 2, coord + Vec3(-70.0f, 0.0f, +70.0f), new Input());
        maUnit[3] = new ClientUnit(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId + 3, coord + Vec3(-105.0f, 0.0f, +105.0f), new Input());
        maUnit[4] = new ClientUnit(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId + 4, coord + Vec3(-140.0f, 0.0f, +140.0f), new Input());
    }
    if (mPlayerId == 1) {
        maUnit[1] = new ClientUnit(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId + 1, coord - Vec3(35.0f ,0.0f, 35.0f), new Input());
        maUnit[2] = new ClientUnit(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId + 2, coord - Vec3(70.0f, 0.0f, 70.0f), new Input());
        maUnit[3] = new ClientUnit(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId + 3, coord - Vec3(105.0f, 0.0f, 105.0f), new Input());
        maUnit[4] = new ClientUnit(mRendering, mWrapper, mGameStaticData, mGameDynData, this, mLeaderId + 4, coord - Vec3(140.0f, 0.0f, 140.0f), new Input());
    }
    for (int index = NB_OF_UNITS; index < NB_OF_UNITS + NB_OF_VIRTUAL_UNITS; ++index) {
        maUnit[index] = new ClientUnit(mRendering, mWrapper);
    }
}
    
void ClientSquad::advance()
{
    for (IntCode index = 1 ; index < NB_OF_UNITS ; ++index) {
        maUnit[index]->advance();
    }
}

bool ClientSquad::hasPeace()
{
    return !isFireActive(false);
}

bool ClientSquad::isFireActive(bool includeLeaderTarget)
{
    // Leader has an active target or at least one unit is under fire
    bool isFireActive = includeLeaderTarget && mLeader->mTarget != 0;

    for (IntVal index = 0 ; index < NB_OF_UNITS ; index++) {
        isFireActive |= maUnit[index]->mAgressor!= 0;
    }
    return isFireActive;
}

bool ClientSquad::hasRessource(IntCode ressource)
{
    bool hasRessource = true;

    for (IntCode unitId = mLeaderId ; unitId < mLeaderId + NB_OF_UNITS ; ++unitId) {
        hasRessource &= maPlayerDynData[unitId]->aRessource[ressource] > 50;
    }
    return hasRessource;
}

bool ClientSquad::hasRessource4Attack(Cell* cell, IntCode ressource)
{
    return
        getRessource(&maPlayerDynData[mLeaderId], ressource) -
        getStrongestRessourceInCell(cell, ressource) > -25;
}

IntVal ClientSquad::getStrongestLeaderInCell(Cell* cell, IntCode ressource)
{
    IntVal  strongestId = 0;
    float   strongest   = 0;
    float   value       = 0;

    for (IntVal leaderId = 0 ; leaderId < NB_OF_PLAYERS*NB_OF_UNITS ; leaderId += NB_OF_UNITS) {
        if (
            leaderId   != mLeaderId &&
            cell       == maPlayerDynData[leaderId]->cell &&
            strongest   < (value = getRessource(&maPlayerDynData[leaderId], ressource, true))
        ) {
            strongest   = value;
            strongestId = leaderId;
        }
    }
    return strongestId;
}

float ClientSquad::getStrongestRessourceInCell(Cell* cell, IntCode ressource)
{
    return getRessource(&maPlayerDynData[getStrongestLeaderInCell(cell, ressource)], ressource, true);
}

float ClientSquad::getRessource(PlayerDynData** aPlayerDynData, IntCode ressource, bool isOpponent)
{
    float   value;

    float   sum          = 0;
    bool    hasRessource = true;

    for (IntCode index = 0 ; index < NB_OF_UNITS ; ++index) {
        value           = aPlayerDynData[index]->aRessource[ressource];
        hasRessource   &= isOpponent || value > 50;
        sum            += value;
    }
    return (int)hasRessource*sum/NB_OF_UNITS;
}

bool ClientSquad::isReachable(IntCode ressource)
{
    return isReachable(maRessourceStaticData[ressource]->cell);
}

bool ClientSquad::isReachable(Cell* cell)
{
    bool hasRessource = true;

    for (IntCode unitId = mLeaderId ; unitId < mLeaderId + NB_OF_UNITS ; ++unitId) {
        hasRessource &= maPlayerDynData[unitId]->aRessource[FUEL] > 0.13*(
            (cell->center - maPlayerDynData[unitId]->coord).magnitude() +
            (maRessourceStaticData[FUEL]->coord - cell->center).magnitude()
        );
    }
    return hasRessource;
}

bool ClientSquad::isFree(IntCode ressource)
{
    return isFree(maRessourceStaticData[ressource]->cell);
}

bool ClientSquad::isFree(Cell* cell)
{
    bool isFree = true;

    for (IntVal leaderId = 0 ; leaderId < NB_OF_PLAYERS*NB_OF_UNITS ; leaderId += NB_OF_UNITS) {
        isFree &=
            leaderId == mLeaderId ||
            maPlayerDynData[leaderId]->cell != cell;
    }
    return isFree;
}

} // END namespace
