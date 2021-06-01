#include "Mage.h"
#include "Game.h"

Mage::Mage() { 
    setType(MAGE); // Fighter type game component
    setHp(MAGE_MAX_HP); // init full health
    maxHp = MAGE_MAX_HP;
    setAttackDamage(MAGE_ATTACK_DAMAGE);
    setAcceleration(MAGE_ACCELERATION);
    setMaxSpeed(MAGE_MAX_SPEED);
}

Mage::Mage(PlayerPosition position) : GamePlayer(position) {
    setType(MAGE); // Fighter type game component
    setHp(MAGE_MAX_HP); // init full health
    maxHp = MAGE_MAX_HP;
    setAttackDamage(MAGE_ATTACK_DAMAGE);
    setAcceleration(MAGE_ACCELERATION);
    setMaxSpeed(MAGE_MAX_SPEED);
}

// overide GamePlayer's attack
void Mage::attack(Game* game, float angle) {
    // two consecutive attacks must have a tiem interval of at least FIGHTER_ATTACK_TIME_INTERVAL
    // otherwise, the second attack will not be initiated
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastAttackTime;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            < MAGE_ATTACK_TIME_INTERVAL) {
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
    p->maxDistance = MAGE_ATTACK_DISTANCE;
    p->deltaX = MAGE_SHOOT_SPEED * cos(angle);
    p->deltaY = -1 * MAGE_SHOOT_SPEED * sin(angle);
    p->ownerID = getID();
    p->type = MAGE_SHOOT;
    p->damage = getAttackDamage();
    game->projectiles[game->nextProjectileId] = p;
    game->nextProjectileId = (game->nextProjectileId + 1) % MAX_PROJECTILE_ID;

    // Send an update to the clients: HEALING_OBJECTIVE_TAKEN
    GameUpdate attackUpdate;
    attackUpdate.updateType = PLAYER_ATTACK;
    attackUpdate.id = this->id;                        // id of player attacking
    attackUpdate.attackAmount = getAttackDamage();     // attack damage amount
    attackUpdate.roleClaimed = MAGE;
    game->addUpdate(attackUpdate);
}

void Mage::uniqueAttack(Game* game, float angle) {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastUniqueAttackTime;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            < FIREBALL_TIME_INTERVAL) {
        return;
    }

    lastUniqueAttackTime = currentTime; // update the lastUniqueAttackTime as this attack

    ProjectilePosition position = {
        getPosition().x,
        getPosition().y,
    };
    Projectile* p = new Projectile();
    p->origin = position;
    p->currentPosition = position; 
    p->deltaX = FIREBALL_SPEED * cos(angle);
    p->deltaY = -1 * FIREBALL_SPEED * sin(angle);
    p->maxDistance = FIREBALL_DISTANCE;
    p->ownerID = getID();
    p->type = MAGE_FIREBALL;
    p->damage = 0;
    game->projectiles[game->nextProjectileId] = p;
    game->nextProjectileId = (game->nextProjectileId + 1) % MAX_PROJECTILE_ID;

    // Send an update to the clients: PLAYER_UNIQUE_ATTACK
    GameUpdate attackUpdate;
    attackUpdate.updateType = PLAYER_UNIQUE_ATTACK;
    attackUpdate.id = this->id;                        // id of player attacking
    attackUpdate.attackAmount = getAttackDamage();     // attack damage amount
    attackUpdate.roleClaimed = MAGE;
    game->addUpdate(attackUpdate);
}

void Mage::interact(Game* game) {
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