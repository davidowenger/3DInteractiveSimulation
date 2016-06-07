#ifndef __CLIENTINPUTMANAGER_H__
#define __CLIENTINPUTMANAGER_H__

namespace Demo
{

class ClientInputManager
{
friend class ClientInput;

public:
    ClientInputManager(Ogre::RenderWindow* const window);
   ~ClientInputManager();

    void                        setMouseClipping(unsigned int width, unsigned int height);

protected:
    OIS::InputManager*          getInputManager();

    Ogre::RenderWindow* const   mWindow;

    OIS::InputManager*          mOISInputManager;
    OIS::Keyboard*              mKeyboard;
    OIS::Mouse*                 mMouse;
    const OIS::MouseState*      mMouseState;

};

} // END namespace

#endif
