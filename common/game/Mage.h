#ifndef _MAGE_H
#define _MAGE_H
#include "GamePlayer.h"

class Mage : public GamePlayer {

public:
    Mage(); // Constructor for GameComponent
    Mage(PlayerPosition position); // Constructor for GameComponent

    void attack (Game* game);
    void interact (Game* game);
};

#endif