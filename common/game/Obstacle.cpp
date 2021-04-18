#include "Obstacle.h"

Obstacle::Obstacle() { setType(OBSTACLE); }

Obstacle::Obstacle(GridPosition position) {
    setType(OBSTACLE);
    setPosition(position.x, position.y);
}

