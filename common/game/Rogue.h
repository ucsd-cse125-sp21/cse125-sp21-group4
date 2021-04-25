#ifndef _ROGUE_H
#define _ROGUE_H
#include "GamePlayer.h"

class Rogue : public GamePlayer {

public:
    Rogue(); // Constructor for GameComponent
    Rogue(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game);
};

#endif