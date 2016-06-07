#include "Common.h"

namespace Demo
{

ClientInterfaceUI::ClientInterfaceUI(Rendering* const rendering) :
    ClientInterface(rendering)
{
    mInput          = new Input();
    mInterfaceInput = new ClientInputUI(mRendering, mInput);
}

ClientInterfaceUI::~ClientInterfaceUI()
{
}

bool ClientInterfaceUI::resume()
{
    if (!mIsStarted) {
        mIsStarted  = true;
        mIsRunning  = !showClientConfiguration();

        if (!mIsRunning) {
            stop();
        }
    }
    return mIsRunning;
}

bool ClientInterfaceUI::showClientConfiguration()
{
    // Show graphics configuration window
    LogManager::getSingletonPtr()->logMessage("Displaying config dialog");
    mRendering->root->showConfigDialog();
    return false;
}

} // END namespace
