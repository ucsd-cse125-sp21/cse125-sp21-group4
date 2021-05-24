#include "Evolve.h"

// using monster health as metric for evolution
Evolve::Evolve(GridPosition position, Restriction r) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(EVO);
    setPosition(position.x, position.y);
    setInteractionRange(EVO_INTERACTION_RANGE);

    evo_amount = EVO_AMOUNT;
    objectiveID = Objective::getNextValidID(); 
}

float Evolve::getEvoAmount() {
    return evo_amount;
}