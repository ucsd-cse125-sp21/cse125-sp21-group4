#ifndef _MONSTER_H
#define _MONSTER_H
#define _USE_MATH_DEFINES

#include "GamePlayer.h"
#include <chrono>
#include <cmath>

class Monster : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;
    float evo;
public:
    Monster(); // Constructor for GameComponent
    Monster(PlayerPosition position); // Constructor for GameComponent
    void attack (Game* game, float angle);
    void uniqueAttack (Game* game, float angle);
    void interact (Game* game);

    void updateEvo(Game* game, float evoLevel);
    void setEvo(float evo);
    float getEvo();
};

#endif