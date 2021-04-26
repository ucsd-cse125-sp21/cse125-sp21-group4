#ifndef _HEALING_
#define _HEALING_

#include "Objective.h"


class Heal : public Objective {
    private:
        int h_amount; // amount to heal

    public:
        Heal(GridPosition position, Restriction r);
        
        int getHealAmount();
};

#endif