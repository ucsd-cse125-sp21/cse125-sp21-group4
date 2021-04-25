#ifndef _BEACON_
#define _BEACON_

#include "Objective.h"

#define BEACON_INTERACTION_RANGE 10

class Beacon : public Objective {
    private:
        int frequency; // frequency to ping (game ticks? seconds?)

    public:
        // time assigned to freq is a placeholder
        Beacon(GridPosition position, Restriction r = R_NEUTRAL, int freq = 30);

        int getFrequency();
};

#endif