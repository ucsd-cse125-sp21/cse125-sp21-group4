#include "Rock.h"

Rock::Rock() { setType(ROCK); }

Rock::Rock(GridPosition position) {
    setType(ROCK);
    setPosition(position.x, position.y);
 }