#ifndef __SERVERBASE_H__
#define __SERVERBASE_H__

namespace Demo
{
class ServerCom;
class ClientSquad;

typedef struct ServerModulesType {
    const ServerModulesType(
        ServerCom* const com
    ) :
        com(com)
    {
    }
    ServerCom* const       com;
} ServerModules;

} // END namespace

#endif
