#ifndef __CLIENTMAIN_H__
#define __CLIENTMAIN_H__

namespace Demo
{

// Base class which manages the standard startup of an Ogre application
class ClientMain
{
public:
    ClientMain(LPSTR strCmdLine);
   ~ClientMain();

    void                        start();

protected:
    void                        setupComponents();
    void                        setupResources();
    void                        setupRendering();
    void                        initComponents();

    String const                mCommandLine;

    Rendering*                  mRendering;
    ClientInterface*            maInterface[3];
    ClientCore*                 mCore;
    ClientListener*             mListener;
    String                      mRessourcePath;
};

} // END namespace

#endif
