#include "Beacon.h"

Beacon::Beacon(GridPosition position, Restriction r, int freq) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(BEAC);
    setPosition(position.x, position.y);

    frequency = freq;
}

int Beacon::getFrequency() {
    return frequency;
}