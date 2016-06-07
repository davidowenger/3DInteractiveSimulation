#ifndef __CLIENTHELPER_H__
#define __CLIENTHELPER_H__

namespace Demo
{

class ClientHelper : Helper
{
public :
    static ClientHelper* const      getSingle();

    const ClientHelper();

    MeshPtr                         loadMesh(String &basenameMesh) const;
    NxOgre::Quat                    asCameraOrientation(NxOgre::Quat q) const;

protected :
    static ClientHelper* const      singleton;

    Mesh::LodValueList*             mvDistance;
};

} // END namespace

#endif
