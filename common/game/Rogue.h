#ifndef _ROGUE_H
#define _ROGUE_H
#include "GamePlayer.h"
#include <chrono>

class Rogue : public GamePlayer {
private:
    std::chrono::steady_clock::time_point lastAttackTime;
    
public:
    Rogue(); // Constructor for GameComponent
    Rogue(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game);
    void interact (Game* game);
};

#endif