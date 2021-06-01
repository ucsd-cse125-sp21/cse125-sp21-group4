#ifndef _FIGHTER_H
#define _FIGHTER_H
#define _USE_MATH_DEFINES
#include "GamePlayer.h"
#include <chrono>
#include <cmath>

class Fighter : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;

public:
    Fighter(); // Constructor for GameComponent
    Fighter(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game, float angle);
    // void uniqueAttack (Game* game, float angle);
    void interact (Game* game);
};

#endif