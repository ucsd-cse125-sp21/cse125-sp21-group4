#include "Healing.h"

Heal::Heal(GridPosition position, Restriction r) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(HEAL);
    setPosition(position.x, position.y);
    
    if (r == R_FIGHTER)
        h_amount = FIGHTER_MAX_HP / 2;
    else
        h_amount = (MONSTER_MAX_HP / 10); // 5 evolutions (health chunks), heal half a chunk
}

int Heal::getHealAmount() {
    return h_amount;
}