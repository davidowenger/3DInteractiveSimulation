#ifndef __CLIENTINTERFACE_H__
#define __CLIENTINTERFACE_H__

namespace Demo
{

class ClientInterface : public NxOgre::Machine
{
friend class ClientCore;
friend class ClientMain;

public:
    ClientInterface(Rendering* const rendering);
 //~ClientInterface();

    virtual bool                resume();
    virtual void                simulateInterface();
    void                        renderInterface();
    virtual void                advance();

protected:
    void                        start();
    void                        pause();
    void                        stop();

    Rendering* const            mRendering;
    ClientInput*                mInterfaceInput;
    Input*                      mInput;

    bool                        mIsRunning;
    bool                        mIsStarted;
    bool                        mIsPaused;
    String                      mDebugText;
};

} // END namespace

#endif
