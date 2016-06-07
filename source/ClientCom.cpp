#include "Common.h"

namespace Demo
{

ClientCom::ClientCom(ServerCom* serverCom) :
    mServerCom(serverCom),
    maPlayerDynData(serverCom->maPlayerDynData),
    maRessourceDynData(serverCom->maRessourceDynData)
{
}

void ClientCom::signIn(PlayerModules** aPlayerModules)
{
    if (ClientConfig::getSingle()->getBool("isRemote", CONFIG_COM)) {
        //TODO: ip call remote server
        //signIn(
        //    ClientConfig::getSingle()->getString("name", CONFIG_COM),
        //    ClientConfig::getSingle()->getBool("isHuman", CONFIG_COM),
        //    ClientConfig::getSingle()->getBool("isThread", CONFIG_COM),
        //    ClientConfig::getSingle()->getBool("isRemote", CONFIG_COM)
        //);
    } else {
        mServerCom->signPlayerIn(
            ClientConfig::getSingle()->getString("name", CONFIG_COM),
            false,
            ClientConfig::getSingle()->getBool("isThread", CONFIG_COM),
            ClientConfig::getSingle()->getBool("isHuman", CONFIG_COM),
            aPlayerModules[0]->squad
        );
        for (IntVal playerId = 1 ; playerId < NB_OF_PLAYERS ; ++playerId) {
            mServerCom->signPlayerIn(
                ClientConfig::getSingle()->getString(
                    Helper::getSingle()->appendNum<long>(String("botName"), playerId),
                    CONFIG_COM
                ),
                false,
                ClientConfig::getSingle()->getBool("isThread", CONFIG_COM),
                false,
                aPlayerModules[playerId]->squad
            );
        }
    }
}

void ClientCom::setPlayerConfig(PlayerModules* const playerModules, bool isHuman, String& name)
{
    IntVal  leaderId  = playerModules->squad->mLeaderId;

    maPlayerDynData[leaderId]->name     = name;
    maPlayerDynData[leaderId]->isHuman  = isHuman;

    if (ClientConfig::getSingle()->getBool("isRemote", CONFIG_COM)) {
        //mServerCom->setPlayerState(); // Send pleyer dyn data when remote
    }
}

void ClientCom::updatePlayersState(PlayerModules** const aPlayerModules)
{
    // sets chassis referentiel from maReferentiel to maPlayerDynData
    // sets computed cell value from maReferentiel to maPlayerDynData
    // send player dyn data when remote                                     *** NET COM OUT ***
    // get other players dyn data when remote                               *** NET COM IN ***

    Cell* cell;

    for (IntVal playerId = 0 ; playerId < NB_OF_PLAYERS ; ++playerId) {
        cell = aPlayerModules[playerId]->squad->mLeader->getCell();

        for (IntCode unitId = 0 ; unitId < NB_OF_UNITS ; ++unitId) {
            maPlayerDynData[playerId*NB_OF_UNITS + unitId]->coord       =
                aPlayerModules[playerId]->squad->maUnit[unitId]->getPosition();
            maPlayerDynData[playerId*NB_OF_UNITS + unitId]->orientation =
                aPlayerModules[playerId]->squad->maUnit[unitId]->getOrientation();
            maPlayerDynData[playerId*NB_OF_UNITS + unitId]->cell        =
                cell;
        }
    }
    if (ClientConfig::getSingle()->getBool("isRemote", CONFIG_COM)) {
        //mServerCom->setPlayerState();           // Send pleyer dyn data when remote
        //mServerCom->updatePlayersState();       // Get other players dyn data when remote
    }
}

} // END namespace
