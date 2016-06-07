#ifndef __SERVERMAIN_H__
#define __SERVERMAIN_H__

namespace Demo
{

class ServerMain
{
friend class ClientMain;

public:
    const ServerMain();
   ~ServerMain();

    void start();

protected:
    void setupComponents();
    void initComponents();

    ServerCore*         mServerCore;
    ServerModules*      mServerModules;
};

} // END namespace

#endif
