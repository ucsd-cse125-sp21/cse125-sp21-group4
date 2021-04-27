#include "Monster.h"
#include "Game.h"


Monster::Monster() { 
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
    updateEvo(MONSTER_FIRST_STAGE_THRESHOLD);
}

Monster::Monster(PlayerPosition position) : GamePlayer(position){
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
    updateEvo(MONSTER_FIRST_STAGE_THRESHOLD);
}

void Monster::attack(Game* game) {

}

// To update the stage of the monster.
void Monster::updateEvo(float evoLevel) {

    // For now update the attack damage of the monster, maybe speed another time?
    if(evoLevel >= MONSTER_FIFTH_STAGE_THRESHOLD) {
        setAttackDamage(MONSTER_ATTACK_DAMAGE + 4);
    } else if (evoLevel >= MONSTER_FOURTH_STAGE_THRESHOLD) {
        setAttackDamage(MONSTER_ATTACK_DAMAGE + 3);
    } else if (evoLevel >= MONSTER_THIRD_STAGE_THRESHOLD) {
        setAttackDamage(MONSTER_ATTACK_DAMAGE + 2);
    } else if (evoLevel >= MONSTER_SECOND_STAGE_THRESHOLD) {
        setAttackDamage(MONSTER_ATTACK_DAMAGE + 1);
    } else if (evoLevel >= MONSTER_FIRST_STAGE_THRESHOLD) {
        setAttackDamage(MONSTER_ATTACK_DAMAGE);
    
    // Invalid levels should not update the evo variable and return
    } else {
        printf("Invalid evolution level, will not update evoLevel.\n");
        return;
    }
    
    // Update evolution of the monster.
    this->evo = evoLevel;
}

float Monster::getEvo() {
    return evo;
}

void Monster::interact(Game* game) {
    // Go through each objective and check if it's within a range
    for(int i = 0; i < game->objectives.size(); i++) {

        // If player is close enough to interact with this objective and can interact with objective
        Objective * obj = game->objectives[i];
        if(isWithinObjective(obj) && canInteractWithObjective(obj)) {
            switch(obj->getType()) {
                case EVO:
                    // Monster: Update Evo
                    Evolve* evoObj = (Evolve*) obj;
                    updateEvo(evo + evoObj->getEvoAmount());

                    // Clean up the evo grid.
                    game->consumeObj(evoObj);
                    break;
                case HEAL:

                    // If the Monster is already full hp, do not consume the healing
                    if(this->hp >= MONSTER_MAX_HP) {
                        printf("Full HP, do not consume objective.\n");
                        return;
                    }

                    // Monster: Heal 100hp
                    Heal* healObj = (Heal *) obj;
                    hpIncrement(healObj->getHealAmount());

                    // Clean up the healing grid.
                    game->consumeObj(healObj);
                    break;
                case BEAC:
                    // Beacon requires zero interaction, so do nothing.
                    break;
                case ARMOR:
                    printf("Interacted with Invalid Armor Obj Type.\n");
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