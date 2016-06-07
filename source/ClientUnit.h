#ifndef __CLIENTUNIT_H__
#define __CLIENTUNIT_H__

namespace Demo
{

class ClientUnit : public NxOgre::Actor
{
friend class ClientSteering;
friend class ClientInterfaceGame;
friend class Bullet;

public:
    ClientUnit(
        Rendering* const rendering = 0, Wrapper* const wrapper = 0,
        GameStaticData* const gameStaticData = 0, GameDynData* const gameDynData = 0,
        ClientSquad* const squad = 0, const IntCode id = 0, const Coord& coord = Coord(0, 0, 0), Input* const input = 0
    );
    
    NxOgre::BoxDescription*         createBox(Real width, Real height, Real depth, Real x, Real y, Real z);
    NxOgre::WheelDescription*       createWheel(Real x, Real y, Real z);

    Vec3                            getPosition();
    NxOgre::Quat                    getOrientation();
    Referentiel* const              getReferentiel(IntCode type = Saladin::PART_CHASSIS);
    
    void                            simulateUnit();
    void                            advance();

    void                            simulateFire(bool isActive);
    void                            applyMove();
    void                            applyFire();

    void                            find1TargetUnit();
    bool                            isFireAble(PlayerDynData* target);
    bool                            isMoving();
    bool                            isFireActive();
    
    void                            setHit(Bullet* bullet);
    void                            setTarget(ClientUnit* unit);

    IntCode                         mId;
    Coord                           mCoord;
    PlayerDynData*                  mTarget;
    PlayerDynData*                  mAgressor;
    float                           mTargetTime;
    float                           mAgressorTime;
    float                           mFireTime;
    float                           mMoveTime;
    Saladin* const                  mSaladin;
    ClientUnitLeader*               mLeader;

protected:
    Rendering* const                mRendering;
    Wrapper* const                  mWrapper;
    GameStaticData* const           mGameStaticData;
    GameDynData* const              mGameDynData;
    ClientSquad* const              mSquad;
    Vec3                            mForce;
    Vec3                            mAvoidance;
    Vec3                            mCohesion;
    Input* const                    mInput;

    ClientInputGame* const          mInterfaceInput;
    ClientMode                      mMode;
    ClientMode                      mCtrl;
    NxOgre::Vec4                    mSpeed;
    NxOgre::Vec4                    mScale;
    NxOgre::Vec4                    mDiff;
    Real                            maState[5];

    SquadData* const                mSquadData;
    ClientUnit** const              maUnit;
    PlayerDynData** const           maPlayerDynData;

    Bullet*                         maBullet[100];
};

} // END namespace

#endif
