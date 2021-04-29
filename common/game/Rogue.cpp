#include "Rogue.h"
#include "Game.h"

Rogue::Rogue() { 
    setType(ROGUE); // Fighter type game component
    setHp(ROGUE_MAX_HP); // init full health
    maxHp = ROGUE_MAX_HP;
    setAttackDamage(ROGUE_ATTACK_DAMAGE);
}

Rogue::Rogue(PlayerPosition position) : GamePlayer(position) {
    setType(ROGUE); // Fighter type game component
    setHp(ROGUE_MAX_HP); // init full health
    maxHp = ROGUE_MAX_HP;
    setAttackDamage(ROGUE_ATTACK_DAMAGE);
}

// overide GamePlayer's attack
void Rogue::attack(Game* game) {
    // two consecutive attacks must have a time interval of at least FIGHTER_ATTACK_TIME_INTERVAL
    // otherwise, the second attack will not be initiated
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastAttackTime;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            < ROGUE_ATTACK_TIME_INTERVAL) {
        return;
    }

    lastAttackTime = currentTime; // update the lastAttackTime as this attack

    ProjectilePosition position = {
        getPosition().x,
        getPosition().y,
    };
    Projectile* p = new Projectile();
    p->origin = position;
    p->currentPosition = position; 
    p->maxDistance = ROGUE_ATTACK_DISTANCE;
    p->ownerID = getID();
    p->type = ROGUE_ARROW;
    p->speed = ROGUE_ARROW_SPEED;
    p->direction = getFaceDirection();
    p->damage = getAttackDamage();
    game->projectiles.push_back(p);
}

void Rogue::interact(Game* game) {
    // Go through each objective and check if it's within a range
    for(int i = 0; i < game->objectives.size(); i++) {

        // If player is close enough to interact with this objective and can interact with objective
        Objective * obj = game->objectives[i];
        if(isWithinObjective(obj) && canInteractWithObjective(obj)) {
            switch(obj->getObjective()) {
                case EVO:
                    printf("Interacted with Invalid Evo Obj Type.\n");
                    break;
                case HEAL:
                    // use GamePlayer's interactHeal.
                    interactHeal(game, (Heal*) obj);
                    break;
                case BEACON:
                    // Beacon requires zero interaction, so do nothing.
                    break;
                case ARMOR:
                    // Armor is just extra health (does not care about maxHp)
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