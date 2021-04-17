#include "Space.h"

Space::Space() { setType(SPACE); }

Space::Space(GridPosition position) {

    setType(SPACE);
    setPosition(position.x, position.y);
    
}

