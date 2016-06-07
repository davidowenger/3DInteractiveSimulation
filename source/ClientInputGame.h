#ifndef __CLIENTINPUTGAME_H__
#define __CLIENTINPUTGAME_H__

namespace Demo
{
class ClientInputGame : public ClientInput
{
public:
    ClientInputGame(
        Rendering* const rendering, Input* const input
    );
  //~ClientInputGame();

protected:
    void                    setupKeysAssignement();
};

} // END namespace

#endif
