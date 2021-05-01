#include "Armor.h"

Armor::Armor(GridPosition position, Restriction r) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(ARMOR);
    setPosition(position.x, position.y);
    setInteractionRange(ARMOR_INTERACTION_RANGE);

    armor_amount = FIGHTER_MAX_HP / 2;
}

int Armor::getArmorAmount() {
    return armor_amount;
}