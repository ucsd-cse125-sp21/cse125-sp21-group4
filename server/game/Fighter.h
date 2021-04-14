#ifndef _FIGHTER_H
#define _FIGHTER_H
#include "GamePlayer.h"

class Fighter : public GamePlayer {

public:
    Fighter(); // Constructor for GameComponent
    Fighter(ComponentPosition position); // Constructor for GameComponent

    void attack (Game* game);
};

#endif