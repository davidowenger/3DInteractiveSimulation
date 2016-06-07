#ifndef __CLIENTCOM_H__
#define __CLIENTCOM_H__

namespace Demo
{

class ClientCom
{
friend class ClientInterfaceGame;

public:
    ClientCom(ServerCom* const serverCom);
   ~ClientCom();

    void signIn(PlayerModules** const aPlayerModules);
    void setPlayerConfig(PlayerModules* const playerModules, bool isHuman = true, String& name = String(""));
    void updatePlayersState(PlayerModules** const aPlayerModules);

private:
    ServerCom* const                mServerCom;
    PlayerDynData** const           maPlayerDynData;
    RessourceDynData** const        maRessourceDynData;
};

} // END namespace

#endif
