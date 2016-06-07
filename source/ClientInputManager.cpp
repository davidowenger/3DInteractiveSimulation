#include "Common.h"

namespace Demo
{

// Static values initialization
ClientInputManager::ClientInputManager(Ogre::RenderWindow* const window) :
    mWindow(window)
{
    // Initialize members
    LogManager::getSingletonPtr()   ->logMessage("Initializing OS Client Inputs");

    // Initialize OIS input manager
    mOISInputManager                   = getInputManager();

    // Create keyboard and mouse devices
    try {
        mKeyboard   = static_cast<OIS::Keyboard*>(
            mOISInputManager->createInputObject(OIS::OISKeyboard, false)
        );
    } catch (...) {
        mKeyboard   = 0;
    }
    try {
        mMouse      = static_cast<OIS::Mouse*>(
            mOISInputManager->createInputObject(OIS::OISMouse, false)
        );
    } catch (...) {
        mMouse      = 0;
    }
    if (!mMouse || !mKeyboard) {
        throw Ogre::ExceptionFactory::create(
            Ogre::ExceptionCodeType<0>(),
            String("Device(s) not detected: ") +
                String( mMouse    ? "" : "mouse "   ) +
                String( mKeyboard ? "" : "keyboard "),
            "Input Devices Initialization",
            __FILE__,
            __LINE__
        );
    }
    LogManager::getSingletonPtr()   ->logMessage("Client OS Inputs OK");

    // Initialize mouse state
    const OIS::MouseState&   ms     = mMouse->getMouseState();
    mMouseState                     = &ms;
}

ClientInputManager::~ClientInputManager()
{
    if (mOISInputManager) {
        mOISInputManager    ->destroyInputObject(mMouse);
        mOISInputManager    ->destroyInputObject(mKeyboard);
        OIS::InputManager   ::destroyInputSystem(mOISInputManager);
    }
}

OIS::InputManager* ClientInputManager::getInputManager()
{
    OIS::ParamList      paramList;
    size_t              windowHnd   = 0;

    mWindow             ->getCustomAttribute("WINDOW", &windowHnd);
    paramList            .insert(std::make_pair(
        std::string("WINDOW"),
        Helper::getSingle()->appendNum<long>(String(""), windowHnd)
    ));
    return              OIS::InputManager::createInputSystem(paramList);
}

// Adjust mouse clipping area
void ClientInputManager::setMouseClipping(unsigned int width, unsigned int height)
{
    mMouseState->width  = width;
    mMouseState->height = height;
}

} // END namespace
