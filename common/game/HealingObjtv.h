#ifndef _HEALING_OBJTV_
#define _HEALING_OBJTV_

#include "Objective.h"

class Heal : public Objective {
    private:
        int amount; // amount to heal

    public:
        Heal(GridPosition position, Restriction r);
};

#endif