#ifndef __CLIENTINPUT_H__
#define __CLIENTINPUT_H__

namespace Demo
{

class ClientInput
{
public:
    ClientInput(
        Rendering* const rendering, Input* const input
    );
   ~ClientInput();

    void                        advance();
    void                        setMouseClipping(unsigned int width, unsigned int height);

    Input* const                mInput;

protected:
    OIS::InputManager*          getInputManager();
    void                        setupCommonKeysAssignement();

    static bool                 maEmptyBool[NUMBER_OF_KEYS];
    static IntCode              maEmptyKey[NUMBER_OF_KEYS];
    static Real                 maEmptyReal[NUMBER_OF_AXES];;

    Rendering* const            mRendering;
    ClientInputManager* const   mInputManager;

    OIS::InputManager* const    mOISInputManager;
    OIS::Keyboard* const        mKeyboard;
    OIS::Mouse* const           mMouse;
    const OIS::MouseState*      mMouseState;

    IntCode                     maKeyAssignement[NUMBER_OF_KEYS];
    bool                        maKey[NUMBER_OF_KEYS];
    bool                        maButton[NUMBER_OF_BUTTONS];
    Real                        maAxe[NUMBER_OF_AXES];
};

} // END namespace

#endif
