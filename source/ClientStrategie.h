#ifndef __CLIENTSTRATEGIE_H__
#define __CLIENTSTRATEGIE_H__

namespace Demo
{

/*************************/
/****** Strategie ********/
/*************************/

class Strategie
{
public:
    Strategie();
    Strategie(IntCode type, bool isFireActive = false, Cell* dest = 0, CellQueue* path = 0);

    IntCode                 mType;
    IntCode                 mState;
    Coord                   mCoord;
    Cell*                   mDest;
    CellQueue*              mPath;
    bool                    mIsFireActive;
};

/*******************************/
/****** ClientStrategie ********/
/*******************************/

class ClientStrategie
{
public:
    ClientStrategie(
        Rendering* const rendering, Wrapper* const wrapper,
        GameStaticData* const gameStaticData, GameDynData* const gameDynData,
        ClientSquad* const squad, ClientSteering* const steering
    );
   ~ClientStrategie();
   
    void                            init();

    void                            advance();
    void                            advanceAI();
    void                            advanceSquad();
    void                            collectTargetUnits();
    
    Strategie*                      getStrategie(IntCode type, bool isFireActive = false, Cell* dest = 0, CellQueue* path = 0);
    Strategie*                      getLeaderStrategie(Strategie* Strategie);
    Strategie*                      getSquadStrategie(Strategie* Strategie);
    
    bool                            hasWon();
    bool                            isLeaderCanceled(Strategie* strat);
    bool                            isLeaderDelayed(Strategie* strat);
    bool                            isLeaderOk(Strategie* strat);
    bool                            isSquadCanceled(Strategie* strat);
    bool                            isSquadDelayed(Strategie* strat);
    bool                            isSquadOk(Strategie* strat);

    Rendering* const                mRendering;
    Wrapper* const                  mWrapper;
    GameStaticData* const           mGameStaticData;
    GameDynData* const              mGameDynData;
    ClientSquad* const              mSquad;
    ClientSteering* const           mSteering;
    
    const IntVal                    mLeaderId;
    SquadData* const                mSquadData;
    ClientUnitLeader* const         mLeader;
    ClientUnit** const              maUnit;
    PlayerDynData** const           maPlayerDynData;
    RessourceStaticData** const     maRessourceStaticData;
    std::stack<Strategie*>          mSquadStack;
    std::stack<Strategie*>          mLeaderStack;
};

} // END namespace

#endif
