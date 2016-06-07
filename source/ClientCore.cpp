#include "Common.h"

namespace Demo
{

ClientCore::ClientCore(Rendering* const rendering, ClientInterface** const aInterface) :
    mRendering(rendering), maInterface(aInterface),
    mDebugOverlay(OverlayManager::getSingleton().getByName("Core/DebugOverlay")),
    mIsRunning(true), mUpdated(false),
    mShowVisualDebugger(false), mShowDebugOverlay(false), mShowDebugText(false),
    mNumScreenShots(0), mFilteringMode(0), mPolygoneMode(0), mLastEvent(0)
{
}

void ClientCore::init()
{
    // Initialize client player state
    toggleInterfaceState(INTERFACE_GAME);
}

ClientCore::~ClientCore()
{
    // delete ...
}

bool ClientCore::isRunning()
{
    return mIsRunning && !mUpdated  && !mRendering->window->isClosed();
}

bool ClientCore::isUpdated()
{
    return mUpdated;
}

void ClientCore::simulateCore()
{
    return maInterface[mInterfaceState]->simulateInterface();
}

void ClientCore::renderCore()
{
    return maInterface[mInterfaceState]->renderInterface();
}

void ClientCore::advance()
{
    mLastEvent += TIME_PER_FRAME;

    maInterface[mInterfaceState]->advance();

    if (mLastEvent > 0.3 &&
        (mInput->aKey[KEY_SCREENSHOTS] ||
         mInput->aKey[KEY_FILTERING] ||
         mInput->aKey[KEY_POLYGONEMODE] ||
         mInput->aKey[KEY_DEBUGGER] ||
         mInput->aKey[KEY_OVERLAY] ||
         mInput->aKey[KEY_CAMERADETAILS] ||
         mInput->aKey[KEY_UI] ||
         mInput->aKey[KEY_GAME] ||
         mInput->aKey[KEY_QUIT]))
    {
        mLastEvent = 0;

        // Screenshots control
        if (mInput->aKey[KEY_SCREENSHOTS]) {
            takeScreenShot();
        }

        // Filtering option control
        if (mInput->aKey[KEY_FILTERING]) {
            toggleFilteringMode();
        }

        // Polygone mode option control
        if (mInput->aKey[KEY_POLYGONEMODE]) {
            togglePolygoneMode();
        }

        // Debug overlay option control
        if (mInput->aKey[KEY_OVERLAY]) {
            toggleDebugOverlay();
        }

        // Camera details option control
        if (mInput->aKey[KEY_CAMERADETAILS]) {
            toggleDebugText();
        }

        // Interface switch control
        if (mInput->aKey[KEY_QUIT] ||
            mInput->aKey[KEY_UI] ||
            mInput->aKey[KEY_GAME])
        {
            toggleInterfaceState(
                mInput->aKey[KEY_UI  ]*INTERFACE_UI+
                mInput->aKey[KEY_GAME]*INTERFACE_GAME+
                mInput->aKey[KEY_QUIT]*INTERFACE_QUIT
            );
        }
    }
    if (mShowDebugOverlay) {
        updateStats();
    }
}

void ClientCore::toggleInterfaceState(InterfaceState nextState)
{
    if (nextState >= INTERFACE_UI) {                        // Resume the UI
        mInterfaceState             = INTERFACE_UI;
    } else if (nextState >= INTERFACE_GAME) {               // Resume the game
        mInterfaceState             = INTERFACE_GAME;
    } else {                                                // Application stopped
        mInterfaceState             = INTERFACE_UI;
        maInterface[INTERFACE_UI  ] ->stop();
        maInterface[INTERFACE_GAME] ->stop();
    }
    mIsRunning              = maInterface[mInterfaceState]->resume();
    mInput                  = maInterface[mInterfaceState]->mInterfaceInput->mInput;
}

void ClientCore::takeScreenShot()
{
    std::ostringstream ss;
    ss << "screenshot_" << ++mNumScreenShots << ".png";
    mRendering->window->writeContentsToFile(ss.str());
    mDebugText = "Saved: " + ss.str();
}

void ClientCore::toggleFilteringMode()
{
    TextureFilterOptions filtering;
    unsigned int         aniso;

    mFilteringMode = (mFilteringMode + 1)%3;

    if (mFilteringMode + 1 == TFO_ANISOTROPIC) {
        filtering   = TFO_ANISOTROPIC;
        aniso       = 8;
    } else if (mFilteringMode + 1 == TFO_TRILINEAR) {
        filtering   = TFO_TRILINEAR;
        aniso       = 1;
    } else {
        filtering   = TFO_BILINEAR;
        aniso       = 1;
    }
    MaterialManager::getSingleton().setDefaultTextureFiltering(filtering);
    MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
}

void ClientCore::togglePolygoneMode()
{
    mPolygoneMode = (mPolygoneMode + 1)%3;

    if (mPolygoneMode == 2) {
        mRendering->camera->setPolygonMode(PM_POINTS);
    } else if (mPolygoneMode == 1) {
        mRendering->camera->setPolygonMode(PM_WIREFRAME);
    } else {
        mRendering->camera->setPolygonMode(PM_SOLID);
    }
}

void ClientCore::toggleDebugOverlay()
{
    mShowDebugOverlay = !mShowDebugOverlay;

    if (mShowDebugOverlay) {
        mDebugOverlay->show();
    } else {
        mDebugOverlay->hide();
    }
}

void ClientCore::toggleDebugText()
{
    mShowDebugText = !mShowDebugText;
}

void ClientCore::updateStats()
{
    //Core/DebugOverlay
    //    Core/StatPanel
    //        Core/CurrFps
    //        Core/AverageFps
    //        Core/WorstFps
    //        Core/BestFps
    //        Core/NumTris
    //        Core/NumBatches
    //        Core/DebugText
    //    Core/LogoPanel
    OverlayElement* vElement;

    try {
        // Update stats
        const RenderTarget::FrameStats& stats = mRendering->window->getStatistics();
        Real vSimuTime = mRendering->timeSimulated - mRendering->timeStart;
        Real vSimuRate = vSimuTime/TIME_PER_FRAME/vSimuTime;
   
        vElement = OverlayManager::getSingleton().getOverlayElement("Core/LogoPanel");
        vElement->hide();
        
        vElement = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
        vElement->setCaption("Frames per second");
        
        vElement = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
        vElement->setCaption("Rendering: " + StringConverter::toString((int)stats.lastFPS));
        
        vElement = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
        vElement->setCaption("Simulation: " + StringConverter::toString((int)vSimuRate));
        
        vElement = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
        vElement->setCaption("");
        
        vElement = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
        vElement->setCaption("");
        
        vElement = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
        vElement->setCaption("");
        
        vElement = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
        vElement->setCaption("");
    }
    catch(...) { /* ignore */ }
}

void ClientCore::windowUpdated(bool isAlive, unsigned int width, unsigned int height)
{
    if (isAlive) {
        mRendering->inputManager->setMouseClipping(width, height);
    } else {
        // Some cleaning to do HERE on window close
    }
}

} // END namespace
