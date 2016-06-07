#include "Common.h"

namespace Demo
{

// Static values initialization
Real                ClientInput::maEmptyReal[NUMBER_OF_AXES]                    =
                        {0,0};
bool                ClientInput::maEmptyBool[NUMBER_OF_KEYS]                    =
                        {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
IntCode             ClientInput::maEmptyKey[NUMBER_OF_KEYS]                     =
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

ClientInput::ClientInput(
    Rendering* const rendering, Input* const input
) :
    mRendering(rendering), mInput(input),
    mInputManager(rendering->inputManager),
    mOISInputManager(rendering->inputManager->mOISInputManager),
    mKeyboard(rendering->inputManager->mKeyboard),
    mMouse(rendering->inputManager->mMouse),
    mMouseState(rendering->inputManager->mMouseState)
{
    // Initialize keys def
    memcpy(maKeyAssignement,    maEmptyKey,     NUMBER_OF_KEYS*sizeof(IntCode));

    // Initialise states instances
    memcpy(maKey,               maEmptyBool,    NUMBER_OF_KEYS   *sizeof(bool));
    memcpy(maButton,            maEmptyBool,    NUMBER_OF_BUTTONS*sizeof(bool));
    memcpy(maAxe,               maEmptyReal,    NUMBER_OF_AXES   *sizeof(Real));

    // Initialisz state pointers
    mInput->aKey        = maKey;
    mInput->aButton     = maButton;
    mInput->aAxe        = maAxe;
}

ClientInput::~ClientInput()
{
}

void ClientInput::setupCommonKeysAssignement()
{
    IntMap* mapClientKeyCode = new IntMap();

     // Get keys assignement
    (*mapClientKeyCode)["KEY_QUIT"] = KEY_QUIT;
    (*mapClientKeyCode)["KEY_SCREENSHOTS"] = KEY_SCREENSHOTS;
    (*mapClientKeyCode)["KEY_FILTERING"] = KEY_FILTERING;
    (*mapClientKeyCode)["KEY_POLYGONEMODE"] = KEY_POLYGONEMODE;
    (*mapClientKeyCode)["KEY_OVERLAY"] = KEY_OVERLAY;
    (*mapClientKeyCode)["KEY_CAMERADETAILS"] = KEY_CAMERADETAILS;
    (*mapClientKeyCode)["KEY_DEBUGGER"] = KEY_DEBUGGER;

    ClientConfig::getSingle()->getKeysAssignement("keyCommon.cfg", maKeyAssignement, mapClientKeyCode);

    delete mapClientKeyCode;
}

void ClientInput::advance()
{
    // Need to capture/update each device
    mKeyboard               ->capture();
    mMouse                  ->capture();

    // Reset state pointers
    mInput->aKey      = maEmptyBool;
    mInput->aButton   = maEmptyBool;
    mInput->aAxe      = maEmptyReal;

    if (!mKeyboard->buffered()) {
        // Process child component specific keyboard input
        mInput->aKey                      = maKey;
        mInput->aKey[KEY_QUIT]            = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_QUIT]);
        mInput->aKey[KEY_UI]              = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_UI]);
        mInput->aKey[KEY_GAME]            = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_GAME]);
        mInput->aKey[KEY_SCREENSHOTS]     = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_SCREENSHOTS]);
        mInput->aKey[KEY_POLYGONEMODE]    = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_POLYGONEMODE]);
        mInput->aKey[KEY_FILTERING]       = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_FILTERING]);
        mInput->aKey[KEY_DEBUGGER]        = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_DEBUGGER]);
        mInput->aKey[KEY_OVERLAY]         = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_OVERLAY]);
        mInput->aKey[KEY_CAMERADETAILS]   = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_CAMERADETAILS]);
        mInput->aKey[KEY_PAUSE]           = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_PAUSE]);
        mInput->aKey[KEY_STOP]            = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_STOP]);
        mInput->aKey[KEY_CTRL]            = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_CTRL]);
        mInput->aKey[KEY_PLAYER]          = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_PLAYER]);
        mInput->aKey[KEY_CAMERA]          = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_CAMERA]);
        mInput->aKey[KEY_LEFT]            = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_LEFT]);
        mInput->aKey[KEY_RIGHT]           = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_RIGHT]);
        mInput->aKey[KEY_FORWARD]         = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_FORWARD]);
        mInput->aKey[KEY_BACKWARD]        = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_BACKWARD]);
        mInput->aKey[KEY_BREAK]           = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_BREAK]);
        mInput->aKey[KEY_UP]              = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_UP]);
        mInput->aKey[KEY_DOWN]            = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_DOWN]);
        mInput->aKey[KEY_ROTATECW]        = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_ROTATECW]);
        mInput->aKey[KEY_ROTATECCW]       = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_ROTATECCW]);
        mInput->aKey[KEY_ROTATEUP]        = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_ROTATEUP]);
        mInput->aKey[KEY_ROTATEDOWN]      = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_ROTATEDOWN]);
        mInput->aKey[KEY_FIRE]            = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_FIRE]);
        mInput->aKey[KEY_ZOOM]            = mKeyboard->isKeyDown((OIS::KeyCode)maKeyAssignement[KEY_ZOOM]);
    }

    if (!mMouse->buffered()) {
        // Fetch mouse buttons state
        mInput->aButton       = maButton;
        mInput->aButton[0]    = mMouseState->buttonDown(OIS::MB_Left);
        mInput->aButton[1]    = mMouseState->buttonDown(OIS::MB_Middle);
        mInput->aButton[2]    = mMouseState->buttonDown(OIS::MB_Right);

        // Fetch mouse axis
        mInput->aAxe          = maAxe;
        mInput->aAxe[0]       = mMouseState->X.rel;
        mInput->aAxe[1]       = mMouseState->Y.rel;
    }
}

} // END namespace
