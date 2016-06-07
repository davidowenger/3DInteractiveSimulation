#ifndef __CLIENTLISTENER_H__
#define __CLIENTLISTENER_H__

namespace Demo
{

class ClientListener : public FrameListener, public WindowEventListener
{
public:
    ClientListener(Rendering* const rendering, ClientCore* const core);
   ~ClientListener();

    void            init();

    // Overriding Ogre FrameListener
    bool            frameStarted(const FrameEvent& evt);
    bool            frameRenderingQueued(const FrameEvent& evt);
    bool            frameEnded(const FrameEvent& evt);

    // Overriding WindowListener
    void            windowResized(RenderWindow* window);
    void            windowClosed(RenderWindow* window);

protected:
    Rendering* const    mRendering;
    ClientCore* const   mCore;
    bool                mIsFirst;
};

} // END namespace

#endif
