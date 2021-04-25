#ifndef _CLERIC_H
#define _CLERIC_H
#include "GamePlayer.h"

class Cleric : public GamePlayer {

public:
    Cleric(); // Constructor for GameComponent
    Cleric(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game);
};

#endif