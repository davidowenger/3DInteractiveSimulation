#include "Common.h"

namespace Demo
{

// Base class which manages the standard startup of an Ogre application
ClientInterfaceGame::ClientInterfaceGame(Rendering* const rendering) :
    ClientInterface(rendering),
    mLastEvent(0), mShowVisualDebugger(false)
{
}

ClientInterfaceGame::~ClientInterfaceGame()
{
}

bool ClientInterfaceGame::resume()
{
    // Setup if needed
    if (!mIsStarted) {
        setupModules();
        setupContent();
        setupCom();
        setupIA();

        mIsStarted          = true;
        mWrapper->scene     ->registerMachine(this);
    }
    return mIsRunning;
}


void ClientInterfaceGame::setupModules()
{
    // PhysX simulation parameters
    NxOgre::SceneDescription        scene_description;
    scene_description.mGravity      = NxOgre::Constants::MEAN_EARTH_GRAVITY;
    scene_description.mUseHardware  = false;

    NxOgre::Scene* scene            = mRendering->world->createScene(scene_description);

    // Wrapper instance
    mWrapper                        = new Wrapper(
        mRendering->sceneManager->createLight(),
        scene,                                                      // Create the PhysX scene
        new Critter::RenderSystem(scene, mRendering->sceneManager)  // Create an NxOgre rendering RenderSystem
    );

    // Game level
    ClientLevel*    level           = new ClientLevel(mRendering, mWrapper);

    // Game Data
    mGameStaticData                 = new GameStaticData(
        &level->maCell[0],
        &level->maRessourceStaticData[0]
    );
    mGameDynData                    = new GameDynData(
        &mRendering->com->maPlayerDynData[0],
        &mRendering->com->maRessourceDynData[0]
    );

    Coord aCoord[4] = {Coord(-200, 6, 200), Coord(-240, 6, -240), Coord(240, 6, -240), Coord(240, 6, 240)};

    // Game Modules
    for (IntVal playerId = 0 ; playerId < NB_OF_PLAYERS ; ++playerId) {
        ClientSquad*        squad       = new ClientSquad(mRendering, mWrapper, mGameStaticData, mGameDynData, aCoord[playerId], playerId);
        ClientSteering*     steering    = new ClientSteering(mRendering, mWrapper, mGameStaticData, mGameDynData, squad);
        ClientStrategie*    strategie   = new ClientStrategie(mRendering, mWrapper, mGameStaticData, mGameDynData, squad, steering);

        maPlayerModules[playerId]       = new PlayerModules(
            squad,
            steering,
            strategie
        );
    };
    mGameModules                    = new GameModules(
        level,
        &maPlayerModules[0]
    );

    // Game Input
    mInterfaceInput                 = maPlayerModules[0]->squad->mLeader->mInterfaceInput;
    mInput                          = maPlayerModules[0]->squad->mLeader->mInput;
}

void ClientInterfaceGame::setupContent()
{
    // Set default material properties
    mWrapper->scene             ->getMaterial(0)->setAll(0.1, 0.9, 0.5);

    // Plane creation
    mWrapper->scene             ->createSceneGeometry(NxOgre::PlaneGeometryDescription());

    // Shadow properties
    mRendering->sceneManager    ->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);
    mRendering->sceneManager    ->setShadowColour(ColourValue(0.5, 0.5, 0.5));
    mRendering->sceneManager    ->setShadowTextureSize(1024);
    mRendering->sceneManager    ->setShadowTextureCount(1);

    // use a small amount of ambient lighting
    mRendering->sceneManager    ->setAmbientLight(ColourValue(0.8, 0.8, 0.8));

    // Add a bright light above the scene
    mWrapper->light             ->setType(Light::LT_POINT);
    mWrapper->light             ->setPosition(-10, 40, 300);
    mWrapper->light             ->setSpecularColour(ColourValue::White);

    // Init objects
    mGameModules->level         ->init(2, 1400, 129, 0, 20, Vec3(0, 0, 0));
}

void ClientInterfaceGame::setupCom()
{
    mRendering->com->signIn(maPlayerModules);
}

void ClientInterfaceGame::setupIA()
{
    for (IntVal playerId = 0 ; playerId < NB_OF_PLAYERS ; ++playerId) {
        maPlayerModules[playerId]->strategie->init();
    }
}

void ClientInterfaceGame::simulateInterface()
{
    // Apply preceding inputs just before simulation takes place
    // Apply FORWARD, UP, ... (drive, steer, brake, setRotation, ...)
    // Just before simulation takes place
    PlayerModules*  modules;
    IntVal          playerId = 0;

    for (
        PlayerDynData** player = &mGameDynData->aPlayerDynData[0] ;
        (*player)->unitId < NB_OF_PLAYERS*NB_OF_UNITS && !((*player)->unitId && (*player)->isRemote) ;
        player += NB_OF_UNITS
    ) {
        modules = maPlayerModules[playerId++];

        for (IntCode unitId = 0 ; unitId < NB_OF_UNITS ; ++unitId) {
            modules->squad->maUnit[unitId]->simulateUnit();
        }
    }
}

void ClientInterfaceGame::render(float userDeltaTime)
{
    // update from physx HERE (mActor, mShape, ...)
    // *** NON-PHYSX SIMULATION ***
    // calls setReferentiels()
    // *** NON-PHYSX SIMULATION FOR HUMAN LEADER ***
    // calls applyHumanInput()

    PlayerModules*  modules;
    IntVal          playerId = 0;

    for (
        PlayerDynData** player = &mGameDynData->aPlayerDynData[0] ;
        (*player)->unitId < NB_OF_PLAYERS*NB_OF_UNITS && !((*player)->unitId && (*player)->isRemote) ;
        player += NB_OF_UNITS
    ) {
        modules = maPlayerModules[playerId++];

        if ((*player)->isHuman) {
            modules->squad->mLeader ->renderHuman();
        }
    }
}

void ClientInterfaceGame::advance()
{
    // - - - *** STATE UPDATE ***
    // - - - calls updatePlayersState()
    // - - - *** GAME LOGIC ***
    // - - - reads units referentiel (chassis, turret, barrel and wheels position and orientation) from maReferentiel
    // - - - reads units dyn data (chassis position and orientation, cell, ressources) from maPlayerDynData
    // - - - get FORWARD, UP, ...                                                   *** GAME LOGIC FOR HUMAN LEADER ***
    // - - - compute FORWARD, UP, ...                                               *** GAME LOGIC FOR NON-HUMAN ***
    //mTime += mRendering->evt->timeSinceLastFrame;

    PlayerModules*  modules;
    IntVal          playerId = 0;

    mRendering->com->updatePlayersState(maPlayerModules);

    for (
        PlayerDynData** player = &mGameDynData->aPlayerDynData[0] ;
        (*player)->unitId < NB_OF_PLAYERS*NB_OF_UNITS && !((*player)->unitId && (*player)->isRemote) ;
        player += NB_OF_UNITS
    ) {
        modules             = maPlayerModules[playerId++];

        if ((*player)->isHuman) {
            mInterfaceInput         ->advance();
            modules->squad->mLeader ->advanceHuman();
        } else {
            modules->strategie      ->advanceAI();
            modules->steering       ->advanceAI();
            modules->squad->mLeader ->advance();
        }
        modules->strategie  ->advance();
        modules->steering   ->advance();
        modules->squad      ->advance();
        
        if ((*player)->isHuman) {
            mLastEvent             += TIME_PER_FRAME;
    
            if (mInput->aKey[KEY_DEBUGGER] && mLastEvent > 0.3) {
                mLastEvent = 0;

                // Visual debugger option control
                toggleVisualDebugger();
            }
        }
    }
}

void ClientInterfaceGame::toggleVisualDebugger()
{
    mShowVisualDebugger = !mShowVisualDebugger;

    if (mShowVisualDebugger) {
        mWrapper->renderSystem->setVisualisationMode(VisualDebugger_ShowAll);
    } else {
        mWrapper->renderSystem->setVisualisationMode(VisualDebugger_ShowNone);
    }
}

} // END namespace
