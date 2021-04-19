#include "Evolve.h"

// using monster health as metric for evolution
Evolve::Evolve(GridPosition position, Restriction r) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(EVO);
    setPosition(position.x, position.y);

    evo_amount = MONSTER_MAX_HP / 10;
}

int Evolve::getEvoAmount() {
    return evo_amount;
}