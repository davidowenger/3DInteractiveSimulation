#ifndef __CLIENTINTERFACEGAME_H__
#define __CLIENTINTERFACEGAME_H__

namespace Demo
{

class ClientInterfaceGame : public ClientInterface
{
public:
    ClientInterfaceGame(Rendering* const rendering);
   ~ClientInterfaceGame();

    bool                        resume();
    void                        simulateInterface();
    void                        render(float userDeltaTime);
    void                        advance();
    PlayerModules*              maPlayerModules[NB_OF_PLAYERS];

protected:
    void                        setupModules();
    void                        setupContent();
    void                        setupCom();
    void                        setupIA();
    
    void                        toggleVisualDebugger();

    Wrapper*                    mWrapper;
    GameStaticData*             mGameStaticData;
    GameDynData*                mGameDynData;
    GameModules*                mGameModules;
    ClientCom*                  mCom;
    
    float                       mLastEvent;
    bool                        mShowVisualDebugger;
};

} // END namespace

#endif
