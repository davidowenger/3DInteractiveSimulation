#ifndef __SERVERCORE_H__
#define __SERVERCORE_H__

namespace Demo
{
class ServerCore
{
friend class ServerMain;

public:
    ServerCore(ServerModules* serverModules);
   ~ServerCore();

private:
    void init();
    void advance();

    ServerModules* const        mServerModules;
};

} // END namespace

#endif
