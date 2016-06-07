#include "Common.h"

namespace Demo
{

ClientInterface::ClientInterface(Rendering* const rendering) :
    mRendering(rendering),
    mIsStarted(false), mIsRunning(true), mIsPaused(false),
    NxOgre::Machine()
{
}

void ClientInterface::start()
{
    mIsRunning  = true;
    mIsPaused   = false;

    resume();
}

void ClientInterface::pause()
{
    mIsPaused   = true;
}

bool ClientInterface::resume()
{
    if (!mIsStarted) {
        mIsStarted  = true;
    }
    return mIsRunning;
}

void ClientInterface::stop()
{
    mIsStarted  = true;
    mIsRunning  = false;
}

void ClientInterface::simulateInterface()
{
}

void ClientInterface::renderInterface()
{
}

void ClientInterface::advance()
{
    mInterfaceInput->advance();
}

} // END namespace
