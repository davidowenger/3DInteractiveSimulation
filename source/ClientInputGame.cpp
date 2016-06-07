/*
-----------------------------------------------------------------------------
Description: Defines a frame listener which responds to frame events.

Mouse:     Freelook
W or Up:   Forward
S or Down: Backward
A:         Step left
D:         Step right
PgUp:      Move upwards
PgDown:    Move downwards
F:         Toggle frame rate stats on/off
R:         Render mode
T:         Cycle texture filtering
           Bilinear, Trilinear, Anisotropic(8)
P:         Toggle on/off display of camera position / orientation
-----------------------------------------------------------------------------
*/

#include "Common.h"

namespace Demo
{

ClientInputGame::ClientInputGame(
    Rendering* const rendering, Input* const input
) :
    ClientInput(rendering, input)
{
    // Initialize keys assignement
    setupKeysAssignement();
    setupCommonKeysAssignement();
}

void ClientInputGame::setupKeysAssignement()
{
    IntMap* mapClientKeyCode = new IntMap();

     // Get keys assignement configuration
    (*mapClientKeyCode)["KEY_PAUSE"] = KEY_PAUSE;
    (*mapClientKeyCode)["KEY_STOP"] = KEY_STOP;
    (*mapClientKeyCode)["KEY_UI"] = KEY_UI;
    (*mapClientKeyCode)["KEY_CTRL"] = KEY_CTRL;
    (*mapClientKeyCode)["KEY_PLAYER"] = KEY_PLAYER;
    (*mapClientKeyCode)["KEY_CAMERA"] = KEY_CAMERA;
    (*mapClientKeyCode)["KEY_LEFT"] = KEY_LEFT;
    (*mapClientKeyCode)["KEY_RIGHT"] = KEY_RIGHT;
    (*mapClientKeyCode)["KEY_FORWARD"] = KEY_FORWARD;
    (*mapClientKeyCode)["KEY_BACKWARD"] = KEY_BACKWARD;
    (*mapClientKeyCode)["KEY_BREAK"] = KEY_BREAK;
    (*mapClientKeyCode)["KEY_UP"] = KEY_UP;
    (*mapClientKeyCode)["KEY_DOWN"] = KEY_DOWN;
    (*mapClientKeyCode)["KEY_ROTATECW"] = KEY_ROTATECW;
    (*mapClientKeyCode)["KEY_ROTATECCW"] = KEY_ROTATECCW;
    (*mapClientKeyCode)["KEY_ROTATEUP"] = KEY_ROTATEUP;
    (*mapClientKeyCode)["KEY_ROTATEDOWN"] = KEY_ROTATEDOWN;
    (*mapClientKeyCode)["KEY_FIRE"] = KEY_FIRE;
    (*mapClientKeyCode)["KEY_ZOOM"] = KEY_ZOOM;

    ClientConfig::getSingle()->getKeysAssignement("keyGame.cfg", maKeyAssignement, mapClientKeyCode);

    delete mapClientKeyCode;
}

} // END namespace
