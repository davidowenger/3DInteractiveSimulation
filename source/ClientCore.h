#ifndef __CLIENTCORE_H__
#define __CLIENTCORE_H__

namespace Demo
{

class ClientCore
{
public:
    ClientCore(Rendering* const rendering, ClientInterface** const aInterface);
   ~ClientCore();

    void                        init();
    void                        simulateCore();
    void                        renderCore();
    void                        advance();

    void                        windowUpdated(bool isAlive, unsigned int width = 0, unsigned int height = 0);
    bool                        isRunning();
    bool                        isUpdated();

    bool                        mIsRunning;
    bool                        mUpdated;

protected:
    void                        toggleInterfaceState(InterfaceState nextState);

    void                        takeScreenShot();
    void                        toggleFilteringMode();
    void                        togglePolygoneMode();
    void                        toggleDebugOverlay();
    void                        toggleDebugText();
    void                        updateStats();

    Rendering* const            mRendering;
    ClientInterface** const     maInterface;

    Overlay* const              mDebugOverlay;
    Input*                      mInput;
    Real                        mLastEvent;
    InterfaceState              mInterfaceState;
    int                         mNumScreenShots;
    IntCode                     mFilteringMode;
    IntCode                     mPolygoneMode;
    String                      mDebugText;
    bool                        mShowVisualDebugger;
    bool                        mShowDebugOverlay;
    bool                        mShowDebugText;
};

} // END namespace

#endif
