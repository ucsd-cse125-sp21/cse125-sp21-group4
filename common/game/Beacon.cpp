#include "Beacon.h"

Beacon::Beacon(GridPosition position, Restriction r, int freq) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(BEAC);
    setPosition(position.x, position.y);
    setInteractionRange(BEACON_INTERACTION_RANGE);

    frequency = freq;
}

int Beacon::getFrequency() {
    return frequency;
}