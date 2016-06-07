#include "Common.h"

namespace Demo
{
    
//****************************
//*** Bullet               ***
//****************************

Bullet::Bullet(Rendering* const rendering, Wrapper* const wrapper, ClientUnit* unit) :
    mRendering(rendering), mWrapper(wrapper), mUnit(unit),
    mCallback(new BulletCallback()), mIsStarted(true),
    mReferentiel(unit->getReferentiel(Saladin::PART_BARREL)),
    NxOgre::Actor(wrapper->scene)
{
    NxOgre::Matrix44                position        = NxOgre::Matrix44(
        mReferentiel->position + Vec3(0, 0, 2.37f).rot(mReferentiel->orientation),
        mReferentiel->orientation
    );
    
    NxOgre::ShapeFlags              sflags;
    NxOgre::BoxDescription          shape           = NxOgre::BoxDescription(0.50f, 0.50f, 0.5f);
    shape.mFlags                                    = sflags;
    shape.mGroup                                    = NB_OF_PLAYERS*NB_OF_UNITS;
    shape.mGroupsMask                               = NxOgre::GroupsMask(0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF);
    
    NxOgre::RigidbodyFlags          flags;
    NxOgre::RigidBodyDescription    description;
    
    description.mRigidbodyFlags                     = flags;
    description.mContactReportFlags                 = ContactPairFlags_All;
    description.mGroup                              = NB_OF_PLAYERS*NB_OF_UNITS;
    description.mName                               = "Bullet";
    description.mMass                               = 0.5f;
    description.mWakeUpCounter                      = 1E12;
    mName                                           = description.mName;
    mNameHash                                       = NxOgre::Strings::hash(mName);

    this                            ->createDynamic(position, description, shape);
    this                            ->setContactCallback(mCallback);
    this                            ->addForce(Vec3(0, 0, 1).rot(mReferentiel->orientation).unit()*12000.0f/NxPi*0.25f*0.25f, ForceMode_Impulse);
    
    mMachine                        = new BulletMachine(rendering, wrapper, this);
}
    
Bullet::~Bullet()
{
    delete mMachine;
    delete mCallback;
}

void Bullet::stop()
{
    mIsStarted  = false;
}


//****************************
//*** Bullet Machine       ***
//****************************

BulletMachine::BulletMachine(Rendering* const rendering, Wrapper* const wrapper, Bullet* bullet) :
    mRendering(rendering), mWrapper(wrapper), mBullet(bullet),
    NxOgre::Machine()
{
    mPointRenderable        = mWrapper->renderSystem->createPointRenderable(ClientHelper::getSingle()->loadMesh(String("Saladin.Bullet"))->getName());
    mWrapper->scene         ->registerMachine((NxOgre::Machine*)this);
}
    
BulletMachine::~BulletMachine()
{
    mWrapper->renderSystem  ->destroyPointRenderable((Critter::PointRenderable*)mPointRenderable);
    mWrapper->scene         ->unregisterMachine((NxOgre::Machine*)this);

    delete mPointRenderable;
}

void BulletMachine::render(float user_deltaTime)
{
    mPointRenderable->render(mBullet->getGlobalPosition(), mBullet->getGlobalOrientationQuat());
    
    if (!mBullet->mIsStarted) {
        mPointRenderable->render(Vec3(0, -100, 0), NxOgre::Quat::IDENTITY);
    } 
}


//****************************
//*** Bullet Callback      ***
//****************************

void BulletCallback::onContact(const NxOgre::ContactPair& pair)
{
    if (pair.mFirst->getName() == String("Bullet")) {
        ((Bullet*)pair.mFirst)->stop();

        if (pair.mSecond->getName() == String("Unit")) {
            ((ClientUnit*)pair.mSecond)->setHit((Bullet*)pair.mFirst);
        }
    } else if (pair.mSecond->getName() == String("Bullet")) {
        ((Bullet*)pair.mSecond)->stop();

        if (pair.mFirst->getName() == String("Unit")) {
            ((ClientUnit*)pair.mFirst)->setHit((Bullet*)pair.mSecond);
        }
    }
}

} // END namespace
