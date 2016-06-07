#include "Common.h"

namespace Demo
{

ClientHelper* const ClientHelper::singleton = new ClientHelper();

ClientHelper::ClientHelper()
{
    // Init LOD settings
    mvDistance = new Mesh::LodValueList();
    mvDistance->push_back(50);
    mvDistance->push_back(125);
    mvDistance->push_back(225);
}

ClientHelper* const ClientHelper::getSingle()
{
    return singleton;
}

NxOgre::Quat ClientHelper::asCameraOrientation(NxOgre::Quat q) const
{
    // Translate PhysX orientation into Ogre orientation
    return NxOgre::Quat(-q.y, -q.z, q.w, q.x);
}

MeshPtr ClientHelper::loadMesh(String &basenameMesh) const
{
    MeshPtr ptrMesh = MeshManager::getSingleton().load(basenameMesh + ".mesh", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    return ptrMesh;
}

} // END namespace
