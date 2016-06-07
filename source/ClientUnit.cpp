#include "Common.h"
#include "NxActor.h"

namespace Demo
{

ClientUnit::ClientUnit(
    Rendering* const rendering, Wrapper* const wrapper,
    GameStaticData* const gameStaticData, GameDynData* const gameDynData,
    ClientSquad* const squad, const IntCode id, const Coord& coord, Input* const input
) :
    mRendering(rendering), mWrapper(wrapper),
    mGameStaticData(gameStaticData), mGameDynData(gameDynData),
    mSquad(squad), mId(id), mCoord(coord),
    mInput(input), mMode(MODE_PLAYER), mCtrl(MODE_PLAYER),
    mSpeed(100.0f, 100.0f, Math::PI/360.0f, Math::PI/360.0f), mScale(0, 0, 0, 0), mDiff(0, 0.55, 0.46, 0.45), 
    mTarget(0), mAgressor(0), mTargetTime(TIME_TO_PEACE), mAgressorTime(TIME_TO_PEACE), mFireTime(TIME_TO_CHANGE), mMoveTime(TIME_TO_CHANGE),
    mInterfaceInput( squad ? new ClientInputGame(rendering, input) : 0 ),
    mSaladin( squad ? new Saladin(rendering, wrapper) : 0 ),
    mSquadData( squad ? squad->mSquadData : 0 ),
    mLeader( squad ? squad->mLeader : 0 ), maUnit( squad ? squad->maUnit : 0 ),
    maPlayerDynData( squad ? gameDynData->aPlayerDynData : 0 ),
    Actor(wrapper->scene)
{
    if (squad) {
        for (IntVal index = 0 ; index < 5 ; ++index) {
            maState[index] = 0;
        }
        for (IntVal index = 0 ; index < MAX_AMMO ; ++index) {
            maBullet[index] = 0;
        }
        mSpeed.x *= (0.60 + (bool)mLeader*0.60);

        NxOgre::RigidBodyDescription    description;
        description.mName               = "Unit";
        description.mMass               = 24500;
        description.mWakeUpCounter      = 1E12;
        description.mMassLocalPose      = Vec3(0.00f, -2.18f, 0.0f);

        mName                           = description.mName;
        mNameHash                       = NxOgre::Strings::hash(mName);
        NxOgre::ShapeDescriptions       vShapes;
        vShapes.push_back(createBox  ( 0.00f,  0.00f,  0.00f,  4.40f,  2.50f,  7.00f));
        vShapes.push_back(createBox  ( 0.10f,  0.70f,  0.70f,  4.40f,  1.50f,  4.40f));
        vShapes.push_back(createBox  (-0.10f,  0.40f,  4.10f,  0.50f,  0.50f,  4.00f));
        vShapes.push_back(createWheel(-2.30f, -2.18f,  3.54f));
        vShapes.push_back(createWheel(-2.29f, -2.18f,  0.50f));
        vShapes.push_back(createWheel(-2.31f, -2.18f, -3.00f));
        vShapes.push_back(createWheel( 2.30f, -2.18f,  3.54f));
        vShapes.push_back(createWheel( 2.29f, -2.18f,  0.50f));
        vShapes.push_back(createWheel( 2.31f, -2.18f, -3.00f));

        this                            ->createDynamic(coord, description, vShapes);
        mSaladin                        ->init(this);

        for (IntVal index = 0 ; index < vShapes.size() ; ++index) {
            delete vShapes[index];
        }
    }
}

NxOgre::BoxDescription* ClientUnit::createBox(Real x, Real y, Real z, Real width, Real height, Real depth)
{
    NxOgre::BoxDescription*	    box     = new NxOgre::BoxDescription(width, height, depth);
    box->mLocalPose                     = NxOgre::Matrix44(Vec3(x, y, z));
    box->mDensity                       = 1.0f;
    box->mMass                          = 0.0f;

    return box;
}

NxOgre::WheelDescription* ClientUnit::createWheel(Real x, Real y, Real z)
{
    NxOgre::WheelDescription*   wheel   = new NxOgre::WheelDescription();
    wheel->mLocalPose					= NxOgre::Matrix44(Vec3(x, y, z));
    wheel->mMass                        = 20.0f;
    
    wheel->mRadius                      = 1.15f;
    wheel->mInverseWheelMass            = 1.0f/50.0f;
    wheel->mSuspensionTravel            = (z != 0.50f)*0.07f + (z == 0.50f)*0.09f;
    wheel->mWheelFlags                  = WheelFlags_AxisContactNormal|WheelFlags_UnscaledSpringBehaviour;
    
    wheel->mLongitudalTireFunction.mStiffnessFactor = 1000000.0f;
    wheel->mLongitudalTireFunction.mAsymptoteSlip = 1000000000.0f;
    wheel->mLateralTireFunction.mStiffnessFactor = 250000.0f;
    wheel->mLateralTireFunction.mAsymptoteSlip = 1000000000.0f;
    
    wheel->mSuspension.mDamper          = 20000.0f;
    wheel->mSuspension.mTargetValue     = 0.02f;
    wheel->mSuspension.mSpring          = 1.0f;

    return wheel;
}

Vec3 ClientUnit::getPosition()
{
    return mSaladin->maReferentiel[Saladin::PART_CHASSIS]->position;
}

NxOgre::Quat ClientUnit::getOrientation()
{
    return mSaladin->maReferentiel[Saladin::PART_CHASSIS]->orientation;
}

Referentiel* const ClientUnit::getReferentiel(IntCode type)
{
    return mSaladin->maReferentiel[type];
}

void ClientUnit::simulateUnit()
{
    // Apply latest frame inputs just before simulation
    Real time = TIME_PER_FRAME;
    
    mScale = mSpeed*time;
    mAgressorTime += time;
    mTargetTime += time;
    mFireTime += time;
    mMoveTime += time;

    if (mAgressorTime >= TIME_TO_PEACE) {
        mAgressor = 0;
    }
    if (mTargetTime >= TIME_TO_PEACE) {
        mTarget = 0;
    }

    // Motion control
    mInput->vMove.x         = ((int)mInput->aKey[KEY_LEFT   ] - (int)mInput->aKey[KEY_RIGHT   ]);
    mInput->vMove.y         = ((int)mInput->aKey[KEY_UP     ] - (int)mInput->aKey[KEY_DOWN    ]);
    mInput->vMove.z         = ((int)mInput->aKey[KEY_FORWARD] - (int)mInput->aKey[KEY_BACKWARD]);
    mInput->vRotate.x       = -(int)mInput->aAxe[0];
    mInput->vRotate.y       =  (int)mInput->aAxe[1];

    // Apply motion control
    if (mCtrl != MODE_GHOST) {
        if (!maState[0] && mInput->aKey[KEY_FORWARD] || !maState[1] && mInput->aKey[KEY_BACKWARD]) {
            this->setLinearVelocity(Vec3(0.0f, 0.0f, 0.0f));
            this->setAngularVelocity(Vec3(0.0f, 0.0f, 0.0f));
            this->setLinearMomentum(Vec3(0.0f, 0.0f, 0.0f));
            this->setAngularMomentum(Vec3(0.0f, 0.0f, 0.0f));
        }
        if (mInput->aKey[KEY_BREAK]) {
            this->setLinearVelocity(Vec3(0.0f, 0.0f, 0.0f));
            this->setAngularVelocity(Vec3(0.0f, 0.0f, 0.0f));
            this->setLinearMomentum(Vec3(0.0f, 0.0f, 0.0f));
            this->setAngularMomentum(Vec3(0.0f, 0.0f, 0.0f));
        }
        if (!maState[3] && mInput->aKey[KEY_LEFT] || !maState[4] && mInput->aKey[KEY_RIGHT]) {
            if (mSaladin->maReferentiel[0]->speed > 10.0f) {
                setAngularVelocity(Vec3(0.0f, 0.0f, 0.0f));
                setAngularMomentum(Vec3(0.0f, 0.0f, 0.0f));
            }
        }
        if (!maState[2]) {
            mMoveTime = 0;
        }
        mSaladin->simulateRotation(
            mInput->vRotate.x*mScale.y,
            mInput->vRotate.y*mScale.y
        );
        mSaladin->simulateTorque(
            mInput->aKey[KEY_BREAK],                                                                                // DIRBRAKE
            mInput->vMove.z,                                                                                        // DIRTORQUE
            mInput->vMove.x,                                                                                        // DIRSTEER
            mSpeed,                                                                                                 // speed
            mScale,                                                                                                 // scaled speed
            mDiff                                                                                                   // TORQUE DIFFERENTIAL
        );
        simulateFire(
            mInput->aButton[0]
        );
        if (mInput->aKey[KEY_FORWARD]) {
            maState[0] = true;
            maState[1] = false;
            maState[2] = false;
        }
        if (mInput->aKey[KEY_BACKWARD]) {
            maState[0] = false;
            maState[1] = true;
            maState[2] = false;
        }
        if (mInput->aKey[KEY_LEFT] || mInput->aKey[KEY_RIGHT]) {
            maState[2] = false;
        }
        maState[3] = mInput->aKey[KEY_LEFT];
        maState[4] = mInput->aKey[KEY_RIGHT];

        if (mInput->aKey[KEY_BREAK]) {
            maState[0] = true;
            maState[1] = false;
            maState[2] = true;
            maState[3] = false;
            maState[4] = false;
        }
    }
}

void ClientUnit::advance()
{
    find1TargetUnit();
    applyMove();
    applyFire();
}

void ClientUnit::simulateFire(bool isActive)
{
    IntVal index;

    for (index = 0 ; index < MAX_AMMO ; ++index) {
        if (maBullet[index] && !maBullet[index]->mIsStarted) {
            mWrapper->scene->destroyActor(maBullet[index]);
            delete maBullet[index];
            maBullet[index] = 0;
        }
    }
    if (mFireTime >= TIME_TO_RELOAD && isActive && maPlayerDynData[mId]->aRessource[AMMO] > 0) {
        maPlayerDynData[mId]->aRessource[AMMO] -= 0; //1;
        mTargetTime                             = 0;
        mFireTime                               = 0;
        
        while (index) {
            if (!maBullet[--index]) {
                maBullet[index] = new Bullet(mRendering, mWrapper, this);
                index = 0;
            }
        }
    }
}

void ClientUnit::applyMove()
{
    // Set inputs according to strategie
    Referentiel* referentiel = mSaladin->maReferentiel[Saladin::PART_CHASSIS];
    Real angleAvoidance = Vec3(0, 1, 0).angle(referentiel->orientation, mAvoidance);
    Real angleCohesion = Vec3(0, 1, 0).angle(referentiel->orientation, mCohesion);
    Real dir = maState[1];
 
    if (!mInput->isStable) {
        if (dir == 0 && (angleCohesion < -3.0f*NxPi/4.0f || 3.0f*NxPi/4.0f < angleCohesion)) {
            dir = 1;
        }
        if (dir == 1 && -NxHalfPi < angleCohesion && angleCohesion < NxHalfPi) {
            dir = 0;
        }
        if (
            mAvoidance != Vec3(0.0f, 0.0f, 0.0f) && 
            ((dir == 0 && (angleAvoidance < -NxHalfPi || NxHalfPi < angleAvoidance)) ||
            (dir == 1 && -NxHalfPi < angleAvoidance && angleAvoidance < NxHalfPi))
        ) {
            mInput->aKey[KEY_FORWARD] = dir == 0;
            mInput->aKey[KEY_BACKWARD] = dir == 1;
            mInput->aKey[KEY_LEFT ] = angleAvoidance >= 0;
            mInput->aKey[KEY_RIGHT] = angleAvoidance < 0;
            mSpeed.z = NxPi/360 + 
                (dir == 0)*(angleAvoidance < 0)*(-angleAvoidance - NxHalfPi) +
                (dir == 0)*(angleAvoidance >= 0)*(angleAvoidance - NxHalfPi) +
                (dir == 1)*(angleAvoidance < 0)*(NxHalfPi + angleAvoidance) +
                (dir == 1)*(angleAvoidance >= 0)*(NxHalfPi - angleAvoidance);
        } else {
            mInput->aKey[KEY_FORWARD] = dir == 0;
            mInput->aKey[KEY_BACKWARD] = dir == 1;
            mInput->aKey[KEY_LEFT ] = dir == 0 && angleCohesion >= 0.08f || dir == 1 && 0 <= angleCohesion && angleCohesion <= NxPi - 0.08f;
            mInput->aKey[KEY_RIGHT] = dir == 0 && angleCohesion <= -0.08f || dir == 1 && 0.08f - NxPi <= angleCohesion && angleCohesion < 0;
            mSpeed.z = NxPi/360 + 
                (dir == 0)*(angleCohesion < 0)*(-angleCohesion) +
                (dir == 0)*(angleCohesion >= 0)*(angleCohesion) +
                (dir == 1)*(angleCohesion < 0)*(NxPi + angleCohesion) +
                (dir == 1)*(angleCohesion >= 0)*(NxPi - angleCohesion);
        }
        mInput->aKey[KEY_BREAK] = false;
    } else {
        mInput->aKey[KEY_FORWARD] = false;
        mInput->aKey[KEY_BACKWARD] = false;
        mInput->aKey[KEY_LEFT] = false;
        mInput->aKey[KEY_RIGHT] = false;
        mInput->aKey[KEY_BREAK] = true;
        mSpeed.z = NxPi/360.0f;
    }
}
 
void ClientUnit::applyFire()
{
    mInput->aAxe[0]     = 0;
    mInput->aAxe[1]     = 0;
    mInput->aButton[0]  = false;

    // Apply fire strategie as inputs
    if (mTarget) {
        Referentiel*    refChassis      = mSaladin->getReferentiel(Saladin::PART_CHASSIS);
        Referentiel*    refTurrel       = mSaladin->getReferentiel(Saladin::PART_TURREL);
        Referentiel*    refBarrel       = mSaladin->getReferentiel(Saladin::PART_BARREL);

        // Normal N to Turret plan
        Vec3            normal          = Vec3(0, 1, 0).rot(refTurrel->orientation);

        // Normal projection Tproj of target T on Turret plan
        // Tproj = T + N.proj(TC)
        // where C is Turret rotation center
        // and   N.proj(TC) is the projection of TC on N
        Vec3            projTarget      = mTarget->coord + normal.proj(mTarget->coord - refTurrel->position);

        // Turret input based on angle from Turret orientation to CTproj
        Real            angleTurrel     = Vec3(0, 1, 0).angle(refTurrel->orientation, projTarget - refTurrel->position);

        if (angleTurrel > -0.01f && angleTurrel < 0.01f) {
            angleTurrel = 0;
        }

        // Barrel input based on angle from Barrel orienation to BT
        // where B is Barrel rotation center computed
        // by translating Barrel position to Barrel rotation center and
        // by rotating according to just calculated Turret rotation input
        NxOgre::Quat    rotation        = normal.getRotQuat(angleTurrel);
        Vec3            position        =
            refChassis->position + 
            Vec3( 0.10f,  0.70f,  0.70f).rot(refChassis->orientation) + 
            Vec3(-0.20f, -0.30f,  1.40f).rot(rotation*refTurrel->orientation);
        Real            angleBarrel     = Vec3(1, 0, 0).angle(rotation*refBarrel->orientation, mTarget->coord - position);
        
        if (angleBarrel > -0.01f && angleBarrel < 0.01f) {
            angleBarrel = 0;
        }

        // Apply rotations
        mInput->aAxe[0]                 =
            std::max<Real>(
              -50, std::min<Real>(
              -angleTurrel/mScale.y,
               50
            ));
        mInput->aAxe[1]                 =
            std::max<Real>(
              -50, std::min<Real>(
               angleBarrel/mScale.y,
               50
            ));
        
        // Apply fire button
        mInput->aButton[0] = mInput->aAxe[0] == 0 && mInput->aAxe[1] == 0;
    }
}

void ClientUnit::find1TargetUnit()
{
    if (!mTarget || !isFireAble(mTarget)) {
        bool    isFound     = false;

        mTarget             = 0;

        for (
            PlayerDynDataVec::iterator iaPlayerDynData = mSquadData->currentTargetVec->begin() ;
            iaPlayerDynData != mSquadData->currentTargetVec->end() && !isFound ;
            ++iaPlayerDynData
        ) {
            if ((*iaPlayerDynData)[0]->unitId != mLeader->mId) {
                for (IntCode index = 0 ; index < NB_OF_UNITS && !isFound ; ++index) {
                    if (isFireAble((*iaPlayerDynData)[index])) {
                        mTarget = (*iaPlayerDynData)[index];
                        isFound = true;
                    }
                }
            }
        }
    }
}

bool ClientUnit::isFireAble(PlayerDynData* target)
{
    // Raycast
    bool                ret             = false;
    Referentiel*        referentiel     = mSaladin->getReferentiel(Saladin::PART_BARREL);
    Vec3                position        = referentiel->position + Vec3(0, 5, 0).rot(referentiel->orientation);
    NxOgre::Ray         ray             = NxOgre::Ray(position, (target->coord - position).unit());
    NxOgre::RaycastHit  hit             = mWrapper->scene->raycastClosestShape(ray, ShapesType_Dynamic);

    if (hit.mRigidBody) {
        ClientUnit*        hitUnit = (ClientUnit*)hit.mRigidBody;
        ret                        = (hitUnit->mId == target->unitId);
    }
    return ret;
}

bool ClientUnit::isFireActive()
{
    return mFireTime <= TIME_TO_CHANGE;
}

bool ClientUnit::isMoving()
{
    return mMoveTime <= TIME_TO_CHANGE;
}

void ClientUnit::setHit(Bullet* bullet)
{
    maPlayerDynData[mId]->aRessource[HEALTH]   -= 0; //1;

    if (bullet->mUnit->mLeader->mId != mLeader->mId) {
        bullet->mUnit                               ->setTarget(this);
        mAgressor                                   = maPlayerDynData[bullet->mUnit->mId];
        mAgressorTime                               = 0;
    }
}

void ClientUnit::setTarget(ClientUnit* unit)
{
    mTarget                                     = maPlayerDynData[unit->mId];
    mTargetTime                                 = 0;
}

} // END namespace
