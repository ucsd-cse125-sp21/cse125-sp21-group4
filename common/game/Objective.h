#ifndef _OBJECTIVE_
#define _OBJECTIVE_

#include "GridComponent.h"

// restrict objective to Restriction
enum Restriction {
    R_HUNTER, // 1
    R_MONSTER, // 2
    R_NEUTRAL, // 3
};

enum ObjectiveType {
    EVO,     // evolution (monster)
    HEAL,    // healing
    BEACON,    // vision beacon
    ARMOR,   // armor
    INVALID_OBJ,
};

class Objective : public GridComponent {
    protected:
        Restriction restrict;
        ObjectiveType type;
        float interactionRange;
        int objectiveID;

    public:
        static int globalObjectiveIDCounter;
        Objective();
        Objective(GridPosition position);

        void setRestriction(Restriction r);
        Restriction getRestriction();

        void setObjective(ObjectiveType t);
        ObjectiveType getObjective();

        void setInteractionRange(float range);
        float getInteractionRange();

        int getObjectiveID();

        static int getNextValidID() {
            Objective::globalObjectiveIDCounter++;
            return Objective::globalObjectiveIDCounter;
        }

};


#endif