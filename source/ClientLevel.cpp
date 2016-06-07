#include "Common.h"

namespace Demo
{

ClientLevel::ClientLevel(Rendering* const rendering, Wrapper* const wrapper) :
    mRendering(rendering), mWrapper(wrapper)
{
       setupData();
}

ClientLevel::~ClientLevel()
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

void ClientLevel::init(int type, Real worldSize, uint16 size, Real minHeight, Real maxHeight, const Vec3& position)
{
    mWorldSize     = worldSize;
    mSize          = size;
    mMinHeight     = minHeight;
    mMaxHeight     = maxHeight;
    mPosition      = position;

    setupTerrain();
    setupHeightField();
}

void ClientLevel::setupTerrain()
{
    // Configure global
    mTerrainGlobals         = OGRE_NEW TerrainGlobalOptions();

    // Tell the terrain what to use for derived (non-realtime) data
    mTerrainGlobals         ->setLightMapDirection(mWrapper->light->getDerivedDirection());
    mTerrainGlobals         ->setMaxPixelError(8);
}

void ClientLevel::setupHeightField()
{
    Image                                       img;
    long                                        offset;
    Vec3                                        origin;
    Terrain*                                    terrain;

    mTerrainGroup                               = OGRE_NEW TerrainGroup(mRendering->sceneManager, Terrain::ALIGN_X_Z, mSize, mWorldSize);
    mTerrainGroup                               ->setOrigin(mPosition.as<Vector3>()); // origin for all terrain in the group

    // Configure default import settings for imported image
    mImportSettings                             = &mTerrainGroup->getDefaultImportSettings();
    mImportSettings->terrainSize                = mSize;
    mImportSettings->worldSize                  = mWorldSize;
    mImportSettings->inputScale                 = 100;
    mImportSettings->minBatchSize               = 17;
    mImportSettings->maxBatchSize               = 65;


    // Textures
    mImportSettings->layerList                  .resize(1);
    mImportSettings->layerList[0].worldSize     = mWorldSize;
    mImportSettings->layerList[0].textureNames  .push_back("terrain-tex.jpg");

    img.load("terrain.jpg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    mTerrainGroup->defineTerrain(0, 0, &img);
    mTerrainGroup->loadAllTerrains(true);

    //HACK: Fix position translation in CritterRenderSystem::createTerrain()
    offset                                      = float(mWorldSize)/2.0f - float(mSize)/2.0f;
    origin                                      = mPosition - Vec3(offset, 0, offset);
    TerrainGroup::TerrainIterator ti            = mTerrainGroup->getTerrainIterator();

    while (ti.hasMoreElements()) {
        terrain                  = ti.getNext()->instance;
        terrain                  ->setPosition(origin.as<Vector3>());
        mWrapper->renderSystem   ->createTerrain(terrain);
        terrain                  ->setPosition(mPosition.as<Vector3>());
    }
    mTerrainGroup->freeTemporaryResources();
}

void ClientLevel::setupData()
{
    // BEGIN GENERATED LINES
    #include "ClientLevel1Data.h"
    // END GENERATED LINES

    for (IntCode ressource = 0 ; ressource < NB_OF_RESSOURCES ; ++ressource) {
        maRessourceStaticData[ressource] = new RessourceStaticData(maCell[126], maCell[126]->center);
    }
}

} // END namespace
