#ifndef _CLERIC_H
#define _CLERIC_H
#include "GamePlayer.h"
#include <chrono>

class Cleric : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;
    std::chrono::steady_clock::time_point lastUniqueAttackTime;

public:
    Cleric(); // Constructor for GameComponent
    Cleric(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game, float angle);
    void uniqueAttack (Game* game, float angle);
    void interact (Game* game);
};

#endif