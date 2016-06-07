#include "Common.h"

namespace Demo
{

ClientUnitLeader::ClientUnitLeader(
    Rendering* const rendering, Wrapper* const wrapper,
    GameStaticData* const gameStaticData, GameDynData* const gameDynData,
    ClientSquad* const squad, const IntCode id, const Coord& coord, Input* const input
) :
    ClientUnit(rendering, wrapper, gameStaticData, gameDynData, squad, id, coord, input),
    mCameraMoveSpeed(4), mCameraRotateSpeed(2),
    mLastEvent(0)
{
    mLeader = this;
}

Cell* const ClientUnitLeader::getCell()
{
    IntVal aCellId[7];
    
    // Approximate nearest cell
    Cell*           cell        = 0;
    bool            found       = false;
    Coord           coord       = getPosition();
    NxOgre::Vec2    coord2D     = NxOgre::Vec2(coord.z, coord.x);
    IntCode         i           = (coord2D.y + 45)/90 + 6;
    IntCode         j           = (coord2D.x + 30 + i%2*30)/60 + 9;
    IntVal          neighbor    = 0;
    
    // Find the neighbor cells
    aCellId[neighbor]           = i*20 + j;

    for (short k = -1 ; k <= 1 ; ++k) {
        for (short l = 0 ; l <= 1 ; ++l) {
            IntVal  cellId      = (i + k)*20 + (j + l) - (!k && !l);
            IntVal  baseLine    = aCellId[0] - aCellId[0]%20 + 20*k;
            bool    connected   = (cellId >= 0 && cellId <= 20*14-1 && cellId >= baseLine && cellId <= baseLine + 19);
            
            if (connected) {
                aCellId[++neighbor] = cellId;
            }
        }
    }
    ++neighbor;

    // Make exact test of the cells
    while (!found && neighbor) {
        cell                    = mGameStaticData->aCell[aCellId[--neighbor]];

        Vec3            center  = cell->center;
        NxOgre::Vec2    p1      = NxOgre::Vec2(center.z - 30, center.x + 30);
        NxOgre::Vec2    p2      = NxOgre::Vec2(center.z + 30, center.x + 30);
        NxOgre::Vec2    p3      = NxOgre::Vec2(center.z + 30, center.x - 30);
        NxOgre::Vec2    p4      = NxOgre::Vec2(center.z - 30, center.x - 30);

        found                   = 
            coord2D.x                               >= p4.x &&      // Point is in the inner square of the cell
            coord2D.y                               >= p4.y && 
            coord2D.x                               <= p2.x && 
            coord2D.y                               <= p2.y ||

            coord2D.x - coord2D.y + p1.y     - p1.x >= 0    &&      // Point is in the triangle at the top of the cell
            coord2D.x + coord2D.y - p2.y     - p2.x <= 0    &&
                        coord2D.y - center.x - 30   >= 0    ||

            coord2D.x - coord2D.y + p3.y     - p3.x <= 0    &&      // Point is in the triangle at the bottom of the cell
            coord2D.x + coord2D.y - p4.y     - p4.x >= 0    &&
                        coord2D.y - center.x + 30   <= 0;
    }
    return cell;
}

void ClientUnitLeader::renderHuman()
{
    // *** NON-PHYSX SIMULATION FOR HUMAN LEADER ***
    // reads MODE, FORWARD, UP, ...
    // sets ghost camera referentiel to maReferentiel                     *** GAME LOGIC FOR GHOST MODE ***
    // sets current camera mode referentiel to mrCamera

    // Move camera
    if (mCtrl == MODE_PLAYER) {
        mSaladin->maReferentiel[Saladin::NB_OF_P_AND_W + MODE_PLAYER]->orientation =
            ClientHelper::getSingle()->asCameraOrientation(mSaladin->maReferentiel[Saladin::NB_OF_P_AND_W + MODE_PLAYER]->orientation);
    } else {
        // Move camera
        mInput->vMove.x        *= -1;
        mInput->vMove.y        *=  1;
        mInput->vMove.z        *= -1;
        
        // YAW direction is around a fixed axis (freelook style) rather than an airplane style
        mRendering->camera      ->yaw         (Radian(mScale.y* mInput->vRotate.x)              );
        mRendering->camera      ->pitch       (Radian(mScale.y*-mInput->vRotate.y)              );
        mRendering->camera      ->moveRelative(       mScale.x * mInput->vMove     .as<Vector3>());

        // Store camera movements
        mSaladin->maReferentiel[Saladin::NB_OF_P_AND_W + MODE_GHOST]->position    =
            Vec3(mRendering->camera->getPosition());
        mSaladin->maReferentiel[Saladin::NB_OF_P_AND_W + MODE_GHOST]->orientation =
            NxOgre::Quat(mRendering->camera->getOrientation());
    }
    mRendering->camera  ->
        setOrientation(mSaladin->maReferentiel[Saladin::NB_OF_P_AND_W + mMode]->orientation .as<Quaternion>());
    mRendering->camera  ->
        setPosition(   mSaladin->maReferentiel[Saladin::NB_OF_P_AND_W + mMode]->position    .as<Vector3   >());
}

void ClientUnitLeader::advanceHuman()
{
    // *** NON-PHYSX SIMULATION FOR HUMAN LEADER ***
    // reads MODE, FORWARD, UP, ...
    // sets ghost camera referentiel to maReferentiel                     *** GAME LOGIC FOR GHOST MODE ***
    // sets current camera mode referentiel to mrCamera

    mLastEvent += TIME_PER_FRAME;
    
    if (mInput->aKey[KEY_PLAYER] && mLastEvent > 0.3) {
        mLastEvent = 0;
        mMode = 0;
        mCtrl = 0;
    }
    if (mInput->aKey[KEY_CTRL] && mLastEvent > 0.3) {
        mLastEvent = 0;
        mMode = 1;
        mCtrl = 0;
    }
    if (mInput->aKey[KEY_CAMERA] && mLastEvent > 0.3) {
        mLastEvent = 0;
        mMode = 1;
        mCtrl = 1;
    }
}

} // END namespace
