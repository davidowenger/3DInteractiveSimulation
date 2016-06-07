#include "Common.h"

namespace Demo
{

//TODO
// Client with bots/threads
// - 1 uniq aPlayerDynData instance
// - Server signIn/Out/validation
// Server standalone
// - Server signIn/Out/validation
// - Server receive/transmitt of a copy of aPlayerDynData
// Server with bots/threads
// - 1 aPlayerDynData instance for the bots/threads
// - Server signIn/Out/validation
// - Server receive/transmitt of a copy of aPlayerDynData

// Main class which manages the standard startup the application
ServerCore::ServerCore(ServerModules* serverModules) :
    mServerModules(serverModules)
{
}

ServerCore::~ServerCore()
{
}

void ServerCore::init()
{
    //mServerModules->com->start();
}

void ServerCore::advance()
{
    //mServerModules->com->start();
}

} // END namespace
