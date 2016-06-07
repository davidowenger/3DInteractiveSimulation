#ifndef __CLIENTINPUTUI_H__
#define __CLIENTINPUTUI_H__

namespace Demo
{

class ClientInputUI : public ClientInput
{
public:
    ClientInputUI(
        Rendering* const rendering, Input* const input
    );
  //~ClientInputUI();

protected:
    void                    setupKeysAssignement();
};

} // END namespace

#endif
