#ifndef _CLERIC_H
#define _CLERIC_H
#include "GamePlayer.h"
#include <chrono>

class Cleric : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;

public:
    Cleric(); // Constructor for GameComponent
    Cleric(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game);
};

#endif