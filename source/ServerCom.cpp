#include "Common.h"

namespace Demo
{

ServerCom::ServerCom()
{
    for (IntVal unitId = 0 ; unitId < NB_OF_PLAYERS*NB_OF_UNITS ; ++unitId)  {
        maPlayerDynData[unitId] = new PlayerDynData();
    }
    for (IntVal ressourceId = 0 ; ressourceId < NB_OF_RESSOURCES ; ++ressourceId) {
        maRessourceDynData[ressourceId] = new RessourceDynData(100, true);
    }
}

ServerCom::~ServerCom()
{
}

bool ServerCom::signPlayerIn(const String& name, const bool isRemote, const bool isThread, const bool isHuman, ClientSquad* const squad)
{
    Cell*           cell;
    PlayerDynData*  aPlayerDynData[NB_OF_UNITS];

    bool            success     = false;
    IntVal          leaderId    = 0;

    while (maPlayerDynData[leaderId]->isActive && leaderId < NB_OF_PLAYERS*NB_OF_UNITS) {
        leaderId += NB_OF_UNITS;
    }
    if (leaderId < NB_OF_PLAYERS*NB_OF_UNITS) {
        cell = squad->mLeader->getCell();

        for (IntCode unitId = 0 ; unitId < NB_OF_UNITS ; ++unitId) {
            maPlayerDynData[leaderId + unitId] = new PlayerDynData(
                leaderId + unitId, &maPlayerDynData[leaderId],
                isRemote, isThread,
                name, isHuman && !unitId, true, squad->maUnit[unitId]->mCoord, cell
            );
            for (IntCode ressourceId = 0 ; ressourceId < NB_OF_RESSOURCES ; ++ressourceId) {
                maPlayerDynData[leaderId + unitId]->aRessource[ressourceId] = 100;
            }
        }
        success = true;
    }
    return success;
}

bool ServerCom::signPlayerOut(IntVal playerId)
{
    maPlayerDynData[playerId*NB_OF_UNITS]->isActive = false;

    return true;
}

void ServerCom::receive(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData)
{
    validate(sourcePlayerId, aPlayerDynData);
    arbitrate(sourcePlayerId, aPlayerDynData);

    memcpy(maPlayerDynData[sourcePlayerId*NB_OF_UNITS], aPlayerDynData, NB_OF_UNITS*sizeof(PlayerDynData));
}

void ServerCom::transmitt(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData)
{
    for (
        IntVal destPlayerId = 0 ;
        destPlayerId < NB_OF_PLAYERS ;
        ++destPlayerId
    ) {
        if (
            destPlayerId != sourcePlayerId &&
            maPlayerDynData[destPlayerId*NB_OF_UNITS]->isRemote
        ) {
            send(destPlayerId, aPlayerDynData);
        }
    }
}

void ServerCom::send(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData, const IntVal signal)
{

}

void ServerCom::validate(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData)
{

}

void ServerCom::arbitrate(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData)
{

}

} // END namespace
