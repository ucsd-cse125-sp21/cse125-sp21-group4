#include "HealingObjtv.h"

Heal::Heal(GridPosition position, Restriction r) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(HEAL);
    setPosition(position.x, position.y);
}
