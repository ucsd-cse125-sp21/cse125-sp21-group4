#ifndef _OBJECTIVE_
#define _OBJECTIVE_

#include "GridComponent.h"

// restrict objective to Restriction
enum Restriction {
    R_FIGHTER, // 1
    R_MONSTER, // 2
    R_NEUTRAL, // 3
};

enum ObjectiveType {
    EVO,     // evolution (monster)
    HEAL,    // healing
    BEAC,    // vision beacon
    ARMOR,   // armor
};

class Objective : public GridComponent {
    protected:
        Restriction restrict;
        ObjectiveType type;

    public:
        Objective();
        Objective(GridPosition position);

        void setRestriction(Restriction r);
        Restriction getRestriction();

        void setObjective(ObjectiveType t);
        ObjectiveType getObjective();
};

#endif