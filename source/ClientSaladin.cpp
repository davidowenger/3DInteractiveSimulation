#include "Common.h"

namespace Demo
{
    
//****************************
//*** Saladin              ***
//****************************

const Real Saladin::MAX_STEER_ANGLE = Math::PI/20;

Saladin::Saladin(Rendering* const rendering, Wrapper* const wrapper) :
    mRendering(rendering), mWrapper(wrapper), NxOgre::Machine()
{
}

Saladin::~Saladin()
{
    mWrapper->scene->unregisterMachine(this);
}

void Saladin::init(ClientUnit* unit)
{
    mUnit = unit;
    
    mUnit->setGlobalOrientationQuat(0, 0, 1, 0);
    
    for (short i = 0 ; i < NB_OF_P_AND_W + NB_OF_MODES ; i++) {
        maReferentiel[i] = new Referentiel(unit->mCoord);
    }
    this            ->addSaladinPart(PART_CHASSIS, "body");
    this            ->addSaladinPart(PART_TURREL,  "turrel"); 
    this            ->addSaladinPart(PART_BARREL,  "barrel");
    this            ->addSaladinPart(PART_WHEEL,   "wheel.right", 1, 1, 1, 0);
    this            ->addSaladinPart(PART_WHEEL,   "wheel.right", 1, 1, 0, 1);
    this            ->addSaladinPart(PART_WHEEL,   "wheel.right", 1, 1, 1, 2);
    this            ->addSaladinPart(PART_WHEEL,   "wheel.left",  1, 1, 1, 3);
    this            ->addSaladinPart(PART_WHEEL,   "wheel.left",  1, 1, 0, 4);
    this            ->addSaladinPart(PART_WHEEL,   "wheel.left",  1, 1, 1, 5);

    mWrapper->scene ->registerMachine(this);
}

void Saladin::addSaladinPart(IntCode type, String mesh, float isBreaking, float isTorquing, float isSteering, unsigned short wheelIndex)
{
    if (mesh == "turrel") {
        mesh = Helper::getSingle()->appendNum<long>(String("turrel"), (int)((float)mUnit->mId/(float)NB_OF_UNITS));
    }
    IntVal                      index       = type + wheelIndex;
    MeshPtr                     meshPtr     = ClientHelper::getSingle()->loadMesh(String("Saladin.") + mesh);
    Critter::PointRenderable*   renderable  = mWrapper->renderSystem->createPointRenderable(meshPtr->getName());
    NxOgre::Shape*              shape       = mUnit->getShape(mUnit->getNbShapes() - 1 - index);

    if (type == PART_WHEEL) {
        NxOgre::Matrix44 vLocalPose(shape->getGlobalPose());
        NxOgre::Vec3 vLocalTranslation(vLocalPose);
        NxOgre::Quat vLocalOrientation(vLocalPose);
        Real angle = (1.0f - 2.0f*((float)wheelIndex > 2.0f))*NxPi/50.0f;
        NxOgre::Quat q = Vec3(0.0f, 0.0f, 1.0f).getRotQuat(angle);
        shape->setGlobalPose(NxOgre::Matrix44(vLocalTranslation, q*vLocalOrientation*NxOgre::Quat::invert(q)));
    }
    maSaladinMachinePart[index]             = new SaladinMachinePart(this, type, renderable, shape, isBreaking, isTorquing, isSteering, wheelIndex);
    mMachineParts                            .push_back(maSaladinMachinePart[index]);
}

Vec3 Saladin::getPosition()
{
    return mUnit->getGlobalPosition();
}

NxOgre::Quat Saladin::getOrientation()
{
    return mUnit->getGlobalOrientationQuat();
}

Referentiel* Saladin::getReferentiel(IntCode type, unsigned short wheelIndex)
{
    return maReferentiel[type + wheelIndex];
}

void Saladin::simulateRotation(Real radTurrel, Real radBarrel)
{
    maReferentiel[PART_TURREL]->rotation += radTurrel;
    maReferentiel[PART_BARREL]->rotation = h::clip(maReferentiel[PART_BARREL]->rotation + radBarrel, -0.17, 0.17);
}

void Saladin::simulateTorque(float dirBrake, float dirTorque, float dirSteer, NxOgre::Vec4 speed, NxOgre::Vec4 scale, NxOgre::Vec4 diff)
{
    float vRotation  = maReferentiel[PART_CHASSIS]->rotation;
    float vRotationSign  = 1.0f - (vRotation < 0.0f)*2.0f;
    float vOffsetDir  = (dirSteer == vRotationSign)*dirSteer;
    float vOffset = vOffsetDir*speed.z - (!vOffsetDir)*vRotationSign*speed.z; 

    maReferentiel[PART_CHASSIS]->rotation = h::clip(
        vRotation + vOffset, 
        -MAX_STEER_ANGLE*(dirSteer || vRotationSign < 0), 
        MAX_STEER_ANGLE*(dirSteer || vRotationSign > 0)
    );
    NxOgre::Vec4 torque = NxOgre::Vec4(
        (-maReferentiel[PART_CHASSIS]->speed/2.0f - 3.0f)*(maReferentiel[PART_CHASSIS]->speed - 500.0f),
        ((maReferentiel[PART_CHASSIS]->speed < 6.0f)*maReferentiel[PART_CHASSIS]->speed + 12.0f)*speed.x,
        maReferentiel[PART_CHASSIS]->rotation,
        0
    );
    for (IntCode i = PART_WHEEL ; i < PART_WHEEL + NB_OF_WHEELS ; ++i) {
        maSaladinMachinePart[i]->simulateTorque(dirBrake, dirTorque, dirSteer, torque, diff);
    }
}

void Saladin::render(float userDeltaTime)
{
    // - - - - - *** NON-PHYSX SIMULATION ***
    // - - - - - calls setReferentiels()
    // - - - - - - reads chassis referentiel from mUnit
    // - - - - - - reads wheels referentiel from mShapes
    // - - - - - - reads rotations from maReferentiels
    // - - - - - - sets client, chassis, turret, barrel and wheels referentiels to maReferentiel

    Vec3 tChassis   = getPosition();
    Real time       = TIME_PER_FRAME; //mRendering->evt->timeSinceLastFrame;
    Real nzTime     = TIME_PER_FRAME;
    Real nzSpeed    = (tChassis - maReferentiel[PART_CHASSIS]->position).magnitude()/nzTime + 0.00000001;

    Vec3 tTurrel    = Vec3( 0.10f,  0.70f,  0.70f);
    Vec3 tBarrel    = Vec3(-0.20f, -0.30f,  1.40f);
    Vec3 tBarrelCM  = Vec3( 0.00f,  0.00f,  2.00f);
    Vec3 tPlayer    = Vec3( 0.00,   0.80f,  0.10f);

    NxOgre::Quat rChassis   = getOrientation();
    NxOgre::Quat rTurrel    = rChassis*Vec3(0, 1, 0).getRotQuat(maReferentiel[PART_TURREL]->rotation);
    NxOgre::Quat rBarrel    = rTurrel*Vec3(1, 0, 0).getRotQuat(maReferentiel[PART_BARREL]->rotation);

    maReferentiel[PART_CHASSIS               ]->orientation = rChassis;
    maReferentiel[PART_TURREL                ]->orientation = rTurrel;
    maReferentiel[PART_BARREL                ]->orientation = rBarrel;
    maReferentiel[NB_OF_P_AND_W + MODE_PLAYER]->orientation = rTurrel;

    maReferentiel[PART_CHASSIS               ]->speed       = nzSpeed;

    maReferentiel[PART_CHASSIS               ]->position    = tChassis;
    maReferentiel[PART_TURREL                ]->position    = maReferentiel[PART_CHASSIS]->position + tTurrel.rot(rChassis);
    maReferentiel[PART_BARREL                ]->position    = maReferentiel[PART_TURREL ]->position + tBarrel.rot(rTurrel) + tBarrelCM.rot(rBarrel);
    maReferentiel[NB_OF_P_AND_W + MODE_PLAYER]->position    = maReferentiel[PART_TURREL ]->position + tPlayer.rot(rTurrel);

    SaladinMachinePart* vPart;
    NxWheelShape* vWheel;
    NxMat34 vWheelPosition;
    NxQuat vQuat;

    for (unsigned short vWheelId = PART_WHEEL ; vWheelId < PART_WHEEL + NB_OF_WHEELS ; ++vWheelId)
    {
        NxWheelContactData vWheelContactData;
        NxMat33 vHeading, vPitch;

        vWheelContactData.contactPosition = 0.0f;
        vPart = maSaladinMachinePart[vWheelId];
        vWheel = ((NxOgre::Wheel*)vPart->getShape())->getWheelShape();
        vWheelPosition = vWheel->getGlobalPose();
        maReferentiel[vWheelId]->rotation += vWheel->getAxleSpeed()*time;

        while (maReferentiel[vWheelId]->rotation > NxTwoPi) {
            maReferentiel[vWheelId]->rotation -= NxTwoPi;
        }
        while (maReferentiel[vWheelId]->rotation < -NxTwoPi) {
            maReferentiel[vWheelId]->rotation += NxTwoPi;
        }
        if (vWheel->getContact(vWheelContactData)) {
            vWheelPosition.t += vWheel->getLocalOrientation()*vWheelPosition.M*NxVec3(0.0f, vWheel->getRadius() - vWheelContactData.contactPosition, 0.0f);
        } else {
            vWheelPosition.t += vWheel->getLocalOrientation()*vWheelPosition.M*NxVec3(0.0f, -vWheel->getSuspensionTravel(), 0.0f);
        }
        vHeading.rotY(vWheel->getSteerAngle());
        vPitch.rotX(maReferentiel[vWheelId]->rotation);
        vWheelPosition.M = vWheelPosition.M*vHeading*vPitch;
        vWheelPosition.M.toQuat(vQuat);

        maReferentiel[vWheelId]->position = Vec3(vWheelPosition.t);
        maReferentiel[vWheelId]->orientation = NxOgre::Quat(vQuat);
    }
    for (mMachinePartIterator = mMachineParts.elements() ; mMachinePartIterator != mMachinePartIterator.end() ; ++mMachinePartIterator) {
        mMachinePartIterator->render(userDeltaTime);
    }
}

//****************************
//*** Saladin Machine Part ***
//****************************

SaladinMachinePart::SaladinMachinePart(Saladin* saladin, IntCode type, NxOgre::PointRenderable* renderable, NxOgre::Shape* shape, float isBreaking, float isTorquing, float isSteering, unsigned short wheelIndex)
:
    mSaladin(saladin), mType(type), mPointRenderable(renderable), mShape(shape),
    mIsSteering(isSteering), mIsTorquing(isTorquing), mIsBraking(isBreaking), mWheelIndex(wheelIndex)
{
}

SaladinMachinePart::~SaladinMachinePart()
{
}

NxOgre::Shape* SaladinMachinePart::getShape()
{
    return mShape;
}

void SaladinMachinePart::simulateTorque(float dirBrake, float dirTorque, float dirSteer, NxOgre::Vec4 torque, NxOgre::Vec4 diff)
{
    NxOgre::Wheel*  shape           = (NxOgre::Wheel*)mShape;
    short           signSens        = 1 - (mWheelIndex%3);
    short           signSide        = 1 - (mWheelIndex < 3)*2;
    Real            motor           = 0;

    if (dirSteer) {
        motor   = torque.x*dirTorque*(
            diff.y -
            signSide*dirSteer*diff.z
        );
    } else if (dirTorque) {
        motor   = torque.x*dirTorque*(
                !(bool)dirSteer + (bool)dirSteer*diff.x -
                signSide*dirSteer*diff.z
            );
    }
    if (motor) {
        shape->setMotorTorque(mIsTorquing*motor);
    }
    if (dirBrake) {
        shape->setMotorTorque(0);
    }
    shape->setBrakeTorque(mIsBraking*dirBrake*torque.w);
    shape->setSteeringAngle(mIsSteering*signSens*torque.y);
}

void SaladinMachinePart::render(float userDeltaTime)
{
    Referentiel* r = mSaladin->getReferentiel(mType, mWheelIndex);
    mPointRenderable->render(r->position, r->orientation);
}

} // END namespace
