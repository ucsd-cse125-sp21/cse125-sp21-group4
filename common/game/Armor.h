#ifndef _ARMOR_
#define _ARMOR_

#include "Objective.h"

#define ARMOR_INTERACTION_RANGE 3

class Armor : public Objective {
    private:
        int armor_amount; // amount of armor

    public:
        Armor(GridPosition position, Restriction r = R_FIGHTER);

        int getArmorAmount();
};

#endif