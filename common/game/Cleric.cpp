#include "Cleric.h"
#include "Game.h"

Cleric::Cleric() { 
    setType(CLERIC); // Fighter type game component
    setHp(CLERIC_MAX_HP); // init full health
    maxHP = CLERIC_MAX_HP;
    setAttackDamage(CLERIC_ATTACK_DAMAGE);
}

Cleric::Cleric(PlayerPosition position) : GamePlayer(position) {
    setType(CLERIC); // Fighter type game component
    setHp(CLERIC_MAX_HP); // init full health
    maxHP = CLERIC_MAX_HP;
    setAttackDamage(CLERIC_ATTACK_DAMAGE);
}

// overide GamePlayer's attack
void Cleric::attack(Game* game) {

}

void Cleric::interact(Game* game) {
    // Go through each objective and check if it's within a range
    for(int i = 0; i < game->objectives.size(); i++) {

        // If player is close enough to interact with this objective and can interact with objective
        Objective * obj = game->objectives[i];
        if(isWithinObjective(obj) && canInteractWithObjective(obj)) {
            switch(obj->getType()) {
                case EVO:
                    printf("Interacted with Invalid Evo Obj Type.\n");
                    break;
                case HEAL:
                    // use GamePlayer's interactHeal.
                    interactHeal(game, (Heal*) obj);
                    break;
                case BEAC:
                    // Beacon requires zero interaction, so do nothing.
                    break;
                case ARMOR:
                    // Armor is just extra health (does not care about maxHP)
                    interactArmor(game, (Armor*) obj);
                    break;
                default:
                    printf("Interacted with Invalid Objective Type.\n");
                    break;
            }

            // Players should only interact with one objective at a time
            // so this return statement saves computation time from running across all objectives
            return;
        }
    }
}