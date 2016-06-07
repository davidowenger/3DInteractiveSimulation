#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <iostream>
#include "OgreTerrainGroup.h"

namespace Demo
{

class ClientLevel
{
public:
    ClientLevel(Rendering* const rendering, Wrapper* const wrapper);
   ~ClientLevel();

    void init(int type, Real worldSize, uint16 size, Real minHeight, Real maxHeight, const Vec3& position);
    void setupTerrain();
    void setupHeightField();

    Cell*                       maCell[NB_OF_CELLS];
    RessourceStaticDataType*    maRessourceStaticData[NB_OF_RESSOURCES];

    Edge*                       maEdge[NB_OF_CELLS][6];
    Cell*                       maConnected[NB_OF_CELLS][7];

protected:
    void                        setupData();

    Rendering* const            mRendering;
    Wrapper* const              mWrapper;

    uint16                      mSize;
    float                       mWorldSize;
    float                       mMinHeight;
    float                       mMaxHeight;
    Vec3                        mPosition;

    TerrainGlobalOptions*       mTerrainGlobals;
    TerrainGroup*               mTerrainGroup;
    Terrain::ImportData*        mImportSettings;
};

} // END namespace

#endif
