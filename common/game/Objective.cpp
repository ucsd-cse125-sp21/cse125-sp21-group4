#include "Objective.h"

Objective::Objective() {}

// Objective::Objective(GridPosition position) {
//     setType(NEUTRAL);
//     setPosition(position.x, position.y);
// }

void Objective::setRestriction(Restriction r) {
    restrict = r;
}

Restriction Objective::getRestriction() {
    return restrict;
}

void Objective::setObjective(ObjectiveType t) {
    type = t;
}

ObjectiveType Objective::getObjective() {
    return type;
}

void Objective::setInteractionRange(float range) {
    interactionRange = range;
}

float Objective::getInteractionRange() {
    return interactionRange;
}