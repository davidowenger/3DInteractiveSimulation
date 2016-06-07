#ifndef __CLIENTUNITLEADER_H__
#define __CLIENTUNITLEADER_H__

namespace Demo
{

class ClientUnitLeader : public ClientUnit
{
public:
    ClientUnitLeader(
        Rendering* const rendering = 0, Wrapper* const wrapper = 0,
        GameStaticData* const gameStaticData = 0, GameDynData* const gameDynData = 0,
        ClientSquad* const squad = 0, const IntCode id = 0, const Coord& coord = Coord(0, 0, 0), Input* const input = 0
    );
    
    Cell* const                     getCell();

    void                            renderHuman();
    void                            advanceHuman();

    void                            toggleVisualDebugger();

    Real            mCameraMoveSpeed;
    Real            mCameraRotateSpeed;
    Real            mLastEvent;

};

} // END namespace

#endif
