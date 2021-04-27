#include "Monster.h"
#include "Game.h"


Monster::Monster() { 
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    maxHP = MONSTER_MAX_HP;
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
    setEvo(MONSTER_FIRST_STAGE_THRESHOLD);
}

Monster::Monster(PlayerPosition position) : GamePlayer(position){
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    maxHP = MONSTER_MAX_HP;
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
    setEvo(MONSTER_FIRST_STAGE_THRESHOLD);
}

void Monster::attack(Game* game) {

}

// To update the stage of the monster.
void Monster::updateEvo(Game* game, float evoLevel) {

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

    // Checks if the evo has changed from one level to the next (int's truncate the decimal values)
    if(abs((int) evoLevel - (int) this->evo) >= 1) {
        GameUpdate evoUpdate;
        evoUpdate.updateType = MONSTER_EVO_UP;
        evoUpdate.newEvoLevel = evoLevel;
        evoUpdate.id = this->id;
        game->addUpdate(evoUpdate);
    }
    
    // Update evolution of the monster.
    this->evo = evoLevel;
}

float Monster::getEvo() {
    return evo;
}


void Monster::setEvo(float evoLevel) {
    evo =  evoLevel;
}


void Monster::interact(Game* game) {
    // Go through each objective and check if it's within a range
    for(int i = 0; i < game->objectives.size(); i++) {

        // If player is close enough to interact with this objective and can interact with objective
        Objective * obj = game->objectives[i];
        if(isWithinObjective(obj) && canInteractWithObjective(obj)) {
            switch(obj->getObjective()) {
                case EVO:{
                    // Monster: Update Evo
                    Evolve* evoObj = (Evolve*) obj;
                    updateEvo(game, evo + evoObj->getEvoAmount());

                    // Clean up the evo grid.
                    game->consumeObj(evoObj);
                    break;
                }
                case HEAL:
                    // use GamePlayer's interactHeal.
                    interactHeal(game, (Heal*) obj);
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