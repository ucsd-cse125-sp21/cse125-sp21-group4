#ifndef _FIGHTER_H
#define _FIGHTER_H
#define _USE_MATH_DEFINES
#include "GamePlayer.h"
#include <chrono>
#include <cmath>

class Fighter : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;
    std::chrono::steady_clock::time_point lastShieldOnTime;
    bool shieldOn;

public:
    Fighter(); // Constructor for GameComponent
    Fighter(PlayerPosition position); // Constructor for GameComponent

    void hpDecrement(int damage, bool fromSystem);

    bool getShieldOn ();
    void setShieldOn (bool newShieldOn);

    void attack (Game* game, float angle);
    void uniqueAttack (Game* game, float angle);
    void interact (Game* game);
};

#endif