#ifndef __CLIENTSQUAD_H__
#define __CLIENTSQUAD_H__

namespace Demo
{

class ClientSquad
{
public:
    ClientSquad(
        Rendering* const rendering, Wrapper* const wrapper,
        GameStaticData* const gameStaticData, GameDynData* const gameDynData,
        const Coord& coord, const IntVal playerId
    );
  //~ClientSquad();

    void                            advance();
    bool                            hasPeace();
    bool                            isFireActive(bool includeLeaderTarget);
    bool                            hasRessource(IntCode type);
    bool                            hasRessource(Coord coord);
    bool                            hasRessource4Attack(Cell* cell, IntCode ressource);
    IntVal                          getStrongestLeaderInCell(Cell* cell, IntCode ressource = HEALTH);
    float                           getStrongestRessourceInCell(Cell* cell, IntCode ressource = HEALTH);
    float                           getRessource(PlayerDynData** aPlayerDynData, IntCode ressource, bool isOpponent = false);
    bool                            isReachable(IntCode ressource);
    bool                            isReachable(Cell* cell);

    bool                            isFree(IntCode ressource);
    bool                            isFree(Cell* cell);

    const IntVal                    mPlayerId;
    const IntVal                    mLeaderId;

    ClientUnit*                     maUnit[NB_OF_UNITS + NB_OF_VIRTUAL_UNITS];
    ClientUnitLeader*               mLeader;
    SquadData*                      mSquadData;
    Cell*                           mCell;

protected:
    Rendering* const            mRendering;
    Wrapper* const              mWrapper;
    GameStaticData* const       mGameStaticData;
    GameDynData* const          mGameDynData;

    RessourceStaticData** const maRessourceStaticData;
    PlayerDynData** const       maPlayerDynData;
    bool                        mIsActive;
};

} // END namespace

#endif
