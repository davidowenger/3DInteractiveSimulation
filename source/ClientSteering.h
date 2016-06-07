#ifndef __CLIENTSTEERING_H__
#define __CLIENTSTEERING_H__

namespace Demo
{

class ClientSteering
{
public:
    ClientSteering(
        Rendering* const rendering, Wrapper* const wrapper,
        GameStaticData* const gameStaticData, GameDynData* const gameDynData,
        ClientSquad* const squad
    );

    CellQueue*                      getPath(Cell* start, Cell* dest, Cell* avoid = 0);

    void                            advanceAI();
    void                            advance();

private:
    static const double             maConstR[3][4][2];
    
    void                            computeForces(IntCode vUnitId, Real vLimit);
    
    Real                            mDistance;

    Rendering* const                mRendering;
    Wrapper* const                  mWrapper;
    GameStaticData* const           mGameStaticData;
    GameDynData* const              mGameDynData;
    ClientSquad* const              mSquad;

    const IntVal                    mLeaderId;
    SquadData* const                mSquadData;
    ClientUnitLeader* const         mLeader;
    ClientUnit** const              maUnit;
    PlayerDynData** const           maPlayerDynData;
};

} // END namespace

#endif
