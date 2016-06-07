#ifndef __SERVERCOM_H__
#define __SERVERCOM_H__

namespace Demo
{

class ServerCom
{
friend class ClientCom;

public:
    ServerCom();
   ~ServerCom();

    bool signPlayerIn(const String& name, const bool isRemote, const bool isThread, const bool isHuman, ClientSquad* const squad);
    bool signPlayerOut(const IntVal playerId);
    void receive(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData);
    void transmitt(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData);
    void send(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData, const IntVal signal = 0);
    void validate(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData);
    void arbitrate(const IntVal sourcePlayerId, PlayerDynData** const aPlayerDynData);

private:
    PlayerDynData*              maPlayerDynData[NB_OF_PLAYERS*NB_OF_UNITS];
    RessourceDynData*           maRessourceDynData[NB_OF_RESSOURCES];
};

} // END namespace

#endif
