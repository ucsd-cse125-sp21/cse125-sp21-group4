#include "Space.h"

Space::Space() { setType(SPACE); }

Space::Space(ComponentPosition position) {

    setType(SPACE);
    setPosition(position.x, position.y);
    
}

