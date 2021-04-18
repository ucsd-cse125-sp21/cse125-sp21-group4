#ifndef _ROCK_H_
#define _ROCK_H_

#include "Obstacle.h"

class Rock : public Obstacle {
    public:
        Rock();
        Rock(GridPosition position);
};

#endif