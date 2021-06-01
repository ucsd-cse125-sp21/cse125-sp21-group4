#ifndef _FIGHTER_H
#define _FIGHTER_H
#include "GamePlayer.h"
#include <chrono>

class Fighter : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;

public:
    Fighter(); // Constructor for GameComponent
    Fighter(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game);
    void interact (Game* game);
    void revive(Game* game);
};

#endif