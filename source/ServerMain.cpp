#include "Common.h"

namespace Demo
{

// Main class which manages the standard startup the application
ServerMain::ServerMain()
{
    setupComponents();
}

ServerMain::~ServerMain()
{
}

// Start the application
void ServerMain::start()
{
   initComponents();
}

void ServerMain::setupComponents()
{
    mServerModules  = new ServerModules(
        new ServerCom()
    );
    mServerCore     = new ServerCore(mServerModules);
}

void ServerMain::initComponents()
{
    mServerCore->init();
}

} // END namespace
