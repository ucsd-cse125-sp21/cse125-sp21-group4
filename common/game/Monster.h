#ifndef _MONSTER_H
#define _MONSTER_H

#include "GamePlayer.h"
#include <chrono>

class Monster : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;
    float evo;
public:
    Monster(); // Constructor for GameComponent
    Monster(PlayerPosition position); // Constructor for GameComponent
    void attack (Game* game);
    void uniqueAttack (Game* game);
    void interact (Game* game);

    void updateEvo(Game* game, float evoLevel);
    void setEvo(float evo);
    float getEvo();
};

#endif