#include "Obstacle.h"

Obstacle::Obstacle() { setType(OBSTACLE); }

Obstacle::Obstacle(ComponentPosition position) {

    setType(OBSTACLE);
    setPosition(position.x, position.y);
    
}

