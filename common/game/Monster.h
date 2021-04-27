#ifndef _MONSTER_H
#define _MONSTER_H

#include "GamePlayer.h"

class Monster : public GamePlayer {
protected:
    float evo;
public:
    Monster(); // Constructor for GameComponent
    Monster(PlayerPosition position); // Constructor for GameComponent
    void attack (Game* game);
    void interact (Game* game);

    void updateEvo(Game* game, float evoLevel);
    void setEvo(float evo);
    float getEvo();
};

#endif