#ifndef __CLIENTBASE_H__
#define __CLIENTBASE_H__

#include "OgreStringConverter.h"
#include "OgreException.h"

// Activate OIS dll import/export macros
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

namespace Demo
{

class Strategie;
class ClientLevel;
class ClientInputManager;
class ClientInput;
class ClientInputGame;
class Bullet;
class Saladin;
class ClientUnit;
class ClientUnitLeader;
class ClientSquad;
class ClientSteering;
class ClientStrategie;
class ClientCom;
class ClientInterfaceGame;
class ClientInterfaceUI;

typedef struct RenderingType {
    const RenderingType(
        const bool isServer,
        NxOgre::World* const world, Root* const root,
        SceneManager* const sceneManager, RenderWindow* const window,
        Camera* const camera, Viewport* const viewport,
        ClientInputManager* const inputManager,
        ClientCom* const com, ServerMain* const serverMain
    ) :
        isServer(isServer),
        world(world), root(root), sceneManager(sceneManager),
        window(window), camera(camera), viewport(viewport),
        inputManager(inputManager),
        com(com), serverMain(serverMain)
    {
    }
    const bool                      isServer;
    NxOgre::World* const            world;
    Root* const                     root;
    SceneManager* const             sceneManager;
    RenderWindow* const             window;
    Camera* const                   camera;
    Viewport* const                 viewport;
    const FrameEvent*               evt;
    ClientInputManager* const       inputManager;
    ClientCom* const                com;
    ServerMain* const               serverMain;
    Real                            timeStart;
    Real                            timeTotal;
    Real                            timeSimulated;
} Rendering;

typedef struct WrapperType {
    const WrapperType(
        Light* const light, NxOgre::Scene* const scene, Critter::RenderSystem* const renderSystem
    ) :
        light(light), scene(scene), renderSystem(renderSystem)
    {
    }
    Light* const                    light;
    NxOgre::Scene* const            scene;
    Critter::RenderSystem* const    renderSystem;
} Wrapper;

typedef struct SquadDataType {
    const SquadDataType(
        PlayerDynDataVec* const currentTargetVec,
        Strategie* const currentSquadStrat, Strategie* const currentLeaderStrategie
    ) :
        currentTargetVec(currentTargetVec),
        currentSquadStrat(currentSquadStrat), currentLeaderStrat(currentLeaderStrat)
    {}
    PlayerDynDataVec*   currentTargetVec;
    Strategie*          currentSquadStrat;
    Strategie*          currentLeaderStrat;
} SquadData;

typedef struct PlayerModulesType {
    const PlayerModulesType(
        ClientSquad* const squad, ClientSteering* const steering, ClientStrategie* const strategie
    ) :
        squad(squad), steering(steering), strategie(strategie)
    {
    }
    ClientSquad* const              squad;
    ClientSteering* const           steering;
    ClientStrategie* const          strategie;
} PlayerModules;

typedef struct GameModulesType {
    const GameModulesType(
        ClientLevel* const level, PlayerModules** const aPlayerModules
    ) :
        level(level), aPlayerModules(aPlayerModules)
    {
    }
    ClientLevel* const      level;
    PlayerModules** const   aPlayerModules;
} GameModules;

// Units ressources strats index
const IntCode               AMMO                    = 0;
const IntCode               HEALTH                  = 1;
const IntCode               FUEL                    = 2;

// Static data strats index
const IntCode               WIN                     = 3;

// Other strats
const IntCode               WON                     = 4;
const IntCode               ALT                     = 5;
const IntCode               ATTACK                  = 6;
const IntCode               FOLLOW                  = 7;
const IntCode               GOTO                    = 8;
const IntCode               ATTACK_AND_DEFEND       = 9;

const IntCode               STATE_INIT              = 0;
const IntCode               STATE_PRESTABLE         = 1;
const IntCode               STATE_STABLE            = 2;

const float                 TIME_PER_FRAME          = 0.02f;
const float                 TIME_TO_CHANGE          = 25.0f;
const float                 TIME_TO_PEACE           = 7.0f;
const float                 TIME_TO_RELOAD          = 1.0f;

const int                   FORCE_POINT             = 10000000;
const int                   FORCE_LINE              =     1000;
const int                   FORCE_GOAL              =      100;
const int                   FORCE_STABLE            =     1000;
const int                   FORCE_UNSTABLE          =   100000;

const   IntCode             NUMBER_OF_KEYS          =    28;
const   IntCode             NUMBER_OF_BUTTONS       =     8;
const   IntCode             NUMBER_OF_AXES          =     2;
const   Real                MAX_AXE_VALUE           =   100;

const   KeyCode             KEY_QUIT                =     0;
const   KeyCode             KEY_GAME                =    20;
const   KeyCode             KEY_UI                  =    21;
const   KeyCode             KEY_PAUSE               =    18;
const   KeyCode             KEY_STOP                =    19;
const   KeyCode             KEY_CTRL                =    22;
const   KeyCode             KEY_PLAYER              =    26;
const   KeyCode             KEY_CAMERA              =    27;
const   KeyCode             KEY_LEFT                =     1;
const   KeyCode             KEY_RIGHT               =     2;
const   KeyCode             KEY_FORWARD             =     3;
const   KeyCode             KEY_BACKWARD            =     4;
const   KeyCode             KEY_BREAK               =    25;
const   KeyCode             KEY_UP                  =     5;
const   KeyCode             KEY_DOWN                =     6;
const   KeyCode             KEY_ROTATECW            =     7;
const   KeyCode             KEY_ROTATECCW           =     8;
const   KeyCode             KEY_ROTATEUP            =     9;
const   KeyCode             KEY_ROTATEDOWN          =    10;
const   KeyCode             KEY_FIRE                =    11;
const   KeyCode             KEY_ZOOM                =    12;
const   KeyCode             KEY_SCREENSHOTS         =    13;
const   KeyCode             KEY_POLYGONEMODE        =    14;
const   KeyCode             KEY_FILTERING           =    15;
const   KeyCode             KEY_OVERLAY             =    16;
const   KeyCode             KEY_CAMERADETAILS       =    17;
const   KeyCode             KEY_DEBUGGER            =    23;
const   KeyCode             KEY_STEERING            =    24; // PRIVATE
} // END namespace

#endif
