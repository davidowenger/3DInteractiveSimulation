#ifndef __CLIENTINTERFACEUI_H__
#define __CLIENTINTERFACEUI_H__

namespace Demo
{

class ClientInterfaceUI : public ClientInterface
{
public:
    ClientInterfaceUI(Rendering* const rendering);
   ~ClientInterfaceUI();

    bool                        resume();

protected:
    bool                        showClientConfiguration();
};

} // END namespace

#endif
