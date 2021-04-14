#ifndef _MONSTER_H
#define _MONSTER_H

#include "GamePlayer.h"

class Monster : public GamePlayer {

public:
    Monster(); // Constructor for GameComponent
    Monster(ComponentPosition position); // Constructor for GameComponent
    void attack (Game* game);
};

#endif