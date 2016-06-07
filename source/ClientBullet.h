#ifndef __BULLET_H__
#define __BULLET_H__

namespace Demo
{

class BulletCallback : public NxOgre::Callback
{
public:
    void onContact(const NxOgre::ContactPair& pair);
};

class BulletMachine : public NxOgre::Machine
{
public:
    BulletMachine(Rendering* const rendering, Wrapper* const wrapper, Bullet* bullet);
   ~BulletMachine();
    
protected:
    void                    render(float user_deltaTime);
    
    Rendering* const        mRendering;
    Wrapper* const          mWrapper;
    Bullet* const           mBullet;
};

class Bullet : public NxOgre::Actor
{
friend class BulletMachine;
friend class ClientUnit;

public:
    Bullet(Rendering* const rendering, Wrapper* const wrapper, ClientUnit* unit);
   ~Bullet();
    
    void stop();

protected:
    Rendering* const        mRendering;
    Wrapper* const          mWrapper;
    ClientUnit* const       mUnit;
    BulletCallback* const   mCallback;
    Referentiel* const      mReferentiel;
    
    BulletMachine*          mMachine;
    bool                    mIsStarted;
};

} // END namespace

#endif
