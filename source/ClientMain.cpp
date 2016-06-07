#include "Common.h"

#define  WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" {
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
    {
        // Create application object
        ClientMain main = ClientMain(strCmdLine);
        main.start();

      //try {
      //    main.start();
      //} catch (Exception& e) {
      //    MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred:", MB_OK|MB_ICONERROR|MB_TASKMODAL);
      //} catch (...) {
      //    MessageBoxA(NULL, "Unknown, not Ogre, error", "An exception has occurred:", MB_OK|MB_ICONERROR|MB_TASKMODAL);
      //}
        return 0;
    }
}

namespace Demo
{

// Main class which manages the standard startup the application
ClientMain::ClientMain(LPSTR commandLine) :
    mCommandLine(String((char*)&commandLine))
{
    // Initialize main ressource path
    ClientConfig::getSingle()->init(
        String(""),
        String("demo.cfg"),
        String("com.cfg")
    );
}

// Specific destructor
ClientMain::~ClientMain()
{
    if (mRendering->world)
        NxOgre::World::destroyWorld();
    if (mRendering->root)
        OGRE_DELETE mRendering->root;
}

// Start the application
void ClientMain::start()
{
    setupComponents();
    setupRendering();
    initComponents();
}

void ClientMain::setupComponents()
{
    Root* const                     root            = OGRE_NEW Root(
        ClientConfig::getSingle()->getPath(String("plugins.cfg")),
        ClientConfig::getSingle()->getPath(String("ogre.cfg")),
        ClientConfig::getSingle()->getPath(String("Ogre.log"))
    );
    root->showConfigDialog();

    SceneManager* const             sceneManager    = root->createSceneManager(ST_EXTERIOR_CLOSE, "ClientSceneManager");
    Ogre::RenderWindow* const       window          = root->initialise(true);
    Ogre::Camera* const             camera          = sceneManager->createCamera("ClientCamera");
    ServerMain* const               serverMain      = new ServerMain();

    // ressources
    setupResources();

    // Create a struct for the Ogre framework objects
    mRendering                = new Rendering(
        ClientConfig::getSingle()->getBool("isServer", CONFIG_COM),
        NxOgre::World::createWorld(),                                   // Create NxOgre rendering world
        root,                                                           // Create Ogre framework root instance
        sceneManager,                                                   // Create NxOgre rendering SceneManager
        window,                                                         // Create an Ogre OS default window
        camera,                                                         // Create an Ogre camera
        window->addViewport(camera),                                    // Create an Ogre viewport, entire window
        new ClientInputManager(window),
        new ClientCom(serverMain->mServerModules->com),
        serverMain
    );
    mRendering->viewport      ->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f));

    // Alter the camera aspect ratio to match the viewport
    mRendering->camera        ->setAspectRatio(
        Real(mRendering->viewport->getActualWidth()) /
        Real(mRendering->viewport->getActualHeight())
    );
    mRendering->camera        ->setNearClipDistance(1);
}

// Get the resources sources
void ClientMain::setupResources()
{
    ConfigFile                              cf;
    ConfigFile::SettingsMultiMap*           settings;
    ConfigFile::SettingsMultiMap::iterator  i;
    String                                  secName, typeName, archName;

    // Load other resource paths from config file
    cf.load(ClientConfig::getSingle()   ->getPath(String("resources.cfg")));

    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci    = cf.getSectionIterator();

    while (seci.hasMoreElements()) {
        secName  = seci.peekNextKey();
        settings = seci.getNext();

        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;

            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    // Initialise and parse ressources
    TextureManager::getSingleton()       .setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton() .initialiseAllResourceGroups();
}

// Startup display
void ClientMain::setupRendering()
{
    // Allow NxOgre rendering to use Ogre resource system, via the Critter component
    NxOgre::ResourceSystem::getSingleton()->openProtocol(new Critter::OgreResourceProtocol());

    // Create Interface components
    maInterface[INTERFACE_GAME]     = new ClientInterfaceGame(mRendering);
    maInterface[INTERFACE_UI]       = new ClientInterfaceUI(mRendering);
}


void ClientMain::initComponents()
{
    // Create a Core component
    mCore                     = new ClientCore(mRendering, maInterface);

    // Create a listener
    mListener                 = new ClientListener(mRendering, mCore     );

    // Start game interface and rendering listener
    mCore                     ->init();
    mListener                 ->init();

    // Start Ogre framework rendering
    mRendering->root          ->startRendering();

    if (mCore->mUpdated) {
        // clean
        delete mListener;
        delete mCore;
        delete mRendering;
        // ...
        
        delete maInterface[1];
        delete maInterface[2];

        // start over
        //start();
    }

    // Clean on exit
    // delete
    // exit
}

} // END namespace
