#ifndef _MONSTER_H
#define _MONSTER_H
#define _USE_MATH_DEFINES

#include "GamePlayer.h"
#include <chrono>
#include <cmath>

class Monster : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;
    float clientSideEvo;
    float evo;
    bool onFire;
public:
    Monster(); // Constructor for GameComponent
    Monster(PlayerPosition position); // Constructor for GameComponent
    void attack (Game* game, float angle);
    void uniqueAttack (Game* game, float angle);
    void interact (Game* game);

    void updateEvo(Game* game, float evoLevel);
    void setEvo(float evo);
    float getEvo();
    void setFire(bool onFire);
    bool isOnFire();
};

#endif