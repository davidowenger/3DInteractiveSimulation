#include "Common.h"

namespace Demo
{

ClientInputUI::ClientInputUI(
    Rendering* const rendering, Input* const input
) :
    ClientInput(rendering, input)
{
    // Initialize keys assignement
    setupKeysAssignement();
    setupCommonKeysAssignement();
}

void ClientInputUI::setupKeysAssignement()
{
    IntMap* mapClientKeyCode = new IntMap();

     // Get keys assignement configuration
    (*mapClientKeyCode)["KEY_GAME"] = KEY_GAME;
    (*mapClientKeyCode)["KEY_LEFT"] = KEY_LEFT;
    (*mapClientKeyCode)["KEY_RIGHT"] = KEY_RIGHT;
    (*mapClientKeyCode)["KEY_FORWARD"] = KEY_FORWARD;
    (*mapClientKeyCode)["KEY_BACKWARD"] = KEY_BACKWARD;
    (*mapClientKeyCode)["KEY_FIRE"] = KEY_FIRE;
    (*mapClientKeyCode)["KEY_ZOOM"] = KEY_ZOOM;

    ClientConfig::getSingle()->getKeysAssignement("keyUI.cfg", maKeyAssignement, mapClientKeyCode);

    delete mapClientKeyCode;
}

} // END namespace
