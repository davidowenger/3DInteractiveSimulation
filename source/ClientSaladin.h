#ifndef __SALADIN_H__
#define __SALADIN_H__

namespace Demo
{

class SaladinMachinePart : public NxOgre::MachinePart
{
public:
    SaladinMachinePart(Saladin* saladin, IntCode type, NxOgre::PointRenderable* renderable, NxOgre::Shape* shape, float isBreaking, float isTorquing, float isSteering, unsigned short wheelIndex);
   ~SaladinMachinePart();
   
    NxOgre::Shape*                              getShape();

    void                                        simulateTorque(float dirBrake, float dirTorque, float dirSteer, NxOgre::Vec4 torque, NxOgre::Vec4 diff);
    void                                        render(float userDeltaTime);

protected:
    Saladin* const                              mSaladin;
    NxOgre::PointRenderable* const              mPointRenderable;
    NxOgre::Shape* const                        mShape;

    const IntCode                               mType;
    const IntCode                               mWheelIndex;
    float                                       mIsSteering;
    float                                       mIsTorquing;
    float                                       mIsBraking;

};

class Saladin : public NxOgre::Machine
{
friend class ClientUnit;
friend class ClientUnitLeader;
friend class ClientInterfaceGame;

public:
    Saladin(Rendering* const rendering, Wrapper* const wrapper);
   ~Saladin ();
   
    void            init(ClientUnit* unit);
    void            addSaladinPart(
                        IntCode                     type,
                        String                      mesh,
                        float                       isBreaking      = 0,
                        float                       isTorquing      = 0,
                        float                       isSteering      = 0,
                        unsigned short              wheelIndex      = 0
                    );
    void            createSaladinMachineParts();
    
    Vec3            getPosition();
    NxOgre::Quat    getOrientation();
    Referentiel*    getReferentiel(IntCode type = PART_CHASSIS, unsigned short wheelIndex = 0);
    
    void            simulateRotation(Real radTurrel, Real radBarrel);
    void            simulateTorque(float dirBrake, float dirTorque, float dirSteer,  NxOgre::Vec4 speed, NxOgre::Vec4 scale, NxOgre::Vec4 diff);
    void            render(float userDeltaTime);

    static const Real          MAX_STEER_ANGLE;

    static const IntCode       NB_OF_PARTS          = 3;
    static const IntCode       NB_OF_WHEELS         = 6;
    static const IntCode       NB_OF_P_AND_W        = NB_OF_PARTS + NB_OF_WHEELS;

    static const IntCode       PART_CHASSIS         = 0;
    static const IntCode       PART_TURREL          = 1;
    static const IntCode       PART_BARREL          = 2;
    static const IntCode       PART_WHEEL           = 3;
    
    Referentiel*                    maReferentiel[NB_OF_P_AND_W + NB_OF_MODES];

protected:
    Wrapper* const                  mWrapper;
    Rendering* const                mRendering;

    ClientUnit*                     mUnit;
    SaladinMachinePart*             maSaladinMachinePart[NB_OF_P_AND_W];
};

} // END namespace

#endif
