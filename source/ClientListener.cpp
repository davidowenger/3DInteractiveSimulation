#include "common.h"
#include <ctime>

// ******************************************************
// ********************* FRAME FLOW *********************
// ******************************************************
//
// Listener::frameQueued(evt)
// - - ClientInterfaceGame : Machine::applyInput()
// - - - apply FORWARD, UP, ... (drive, steer, brake, setRotation, ...)
// - NxOgre::World::advance()
// - - foreach Scene
// - - - foreach Machine simulate()
// - - - *** PHYSX SIMULATION ***
// - - - physx::advance()
// - - - - updates mActor and mShapes
// - - foreach Scene
// - - - foreach Machine render()
// - - - - ClientInterfaceGame : Machine::render()  !!update frome physx HERE (mActor, mShape, ...)!!
// - - - - - *** NON-PHYSX SIMULATION ***
// - - - - - calls setReferentiels()
// - - - - - - reads chassis referentiel from mActor
// - - - - - - reads wheels referentiel from mShapes
// - - - - - - reads rotations from maReferentiels
// - - - - - - sets chassis, turret, barrel and wheels referentiels to maReferentiel
// - - - - - *** NON-PHYSX SIMULATION FOR HUMAN LEADER ***
// - - - - - reads MODE, FORWARD, UP, ...
// - - - - - sets ghost camera referentiel to maReferentiel                     *** GAME LOGIC FOR GHOST MODE ***
// - - - - - sets current camera mode referentiel to mrCamera
// - - - - *** OGRE RENDERING ***
// - - - - Actor::render(), PointRenderable::render(), ... by taking values in maReferentiel when needed
// - Demo::ClientCore::advance()
// - - reads player inputs from mInput
// - - resumes ClientInterfaceGame or ClientInterfaceUI and copy its mInput pointer
// - - ClientInterfaceGame : Machine::advance()
// - - - *** STATE UPDATE ***
// - - - calls setPlayerState()
// - - - - sets chassis referentiel from maReferentiel to maPlayerDynData
// - - - - sets computed cell value from maReferentiel to maPlayerDynData
// - - - - send player dyn data when remote                                     *** NET COM OUT ***
// - - - - get other players dyn data when remote                               *** NET COM IN ***
// - - - *** GAME LOGIC ***
// - - - reads units referentiel (chassis, turret, barrel and wheels position and orientation) from maReferentiel
// - - - reads units dyn data (chassis position and orientation, cell, ressources) from maPlayerDynData
// - - - get FORWARD, UP, ...                                                   *** GAME LOGIC FOR HUMAN LEADER ***
// - - - compute FORWARD, UP, ...                                               *** GAME LOGIC FOR NON-HUMAN ***


namespace Demo
{

ClientListener::ClientListener(Rendering* rendering, ClientCore* core) :
    mRendering(rendering), mCore(core), mIsFirst(true)
{
    mRendering->timeStart = clock()/(CLOCKS_PER_SEC/1000);
    mRendering->timeTotal = mRendering->timeStart;
    mRendering->timeSimulated = mRendering->timeStart;
}

ClientListener::~ClientListener()
{
    // Remove from the Ogre Frame listener
    mRendering->root        ->removeFrameListener(this);

    // Remove from the Window listeners
    WindowEventUtilities    ::removeWindowEventListener(mRendering->window, this);

    windowClosed(mRendering->window);
}

void ClientListener::init()
{
    // Set initial mouse clipping size
    windowResized(mRendering->window);

    // Register as a Window listener
    WindowEventUtilities    ::addWindowEventListener(mRendering->window, this);

    // Register as an Ogre Frame listener
    mRendering->root        ->addFrameListener(this);
}

// Overridind Ogre FrameListener
bool ClientListener::frameStarted(const FrameEvent& evt)
{
    mRendering->timeTotal = clock()/(CLOCKS_PER_SEC/1000);
    mRendering->evt = &evt;

    if (mRendering->timeTotal >= mRendering->timeSimulated) {
        // Advance game
        mCore                   ->simulateCore();                       // Simulate Core separately to be able to add machines here
        mRendering->world       ->advance(TIME_PER_FRAME);              // This will simulate and render all machines
        mCore                   ->advance();                            // Advance logic

        mIsFirst = false;
        mRendering->timeSimulated += TIME_PER_FRAME*1000;
    }
    return mCore            ->isRunning();
}

bool ClientListener::frameRenderingQueued(const FrameEvent& evt)
{
    // Opportunity to use queued GPU time
    // GPU load is low, at lowest

    // GPU swap work will take place now
    return true;
}

bool ClientListener::frameEnded(const FrameEvent& evt)
{
    // GPU load is high, at highest
    // frameStarted for next frame will be called now
    return true;
}

// Adjust mouse clipping area
void ClientListener::windowResized(RenderWindow* window)
{
    unsigned int    width, height, depth;
    int             left, top;
}

// Clean before window shutdown
void ClientListener::windowClosed(RenderWindow* window)
{
    // Assert the window is the one we created
    if (window == mRendering->window) {
        mCore     ->windowUpdated(false);
    }
}

} // END namespace
