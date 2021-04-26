#ifndef _MAGE_H
#define _MAGE_H
#include "GamePlayer.h"
#include <chrono>

class Mage : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;

public:
    Mage(); // Constructor for GameComponent
    Mage(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game);
};

#endif