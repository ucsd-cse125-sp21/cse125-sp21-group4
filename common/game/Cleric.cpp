#include "Cleric.h"
#include "Game.h"

Cleric::Cleric() { 
    setType(CLERIC); // Fighter type game component
    setHp(CLERIC_MAX_HP); // init full health
    setAttackDamage(CLERIC_ATTACK_DAMAGE);
    maxHp = CLERIC_MAX_HP;
}

Cleric::Cleric(PlayerPosition position) : GamePlayer(position) {
    setType(CLERIC); // Fighter type game component
    setHp(CLERIC_MAX_HP); // init full health
    setAttackDamage(CLERIC_ATTACK_DAMAGE);
    maxHp = CLERIC_MAX_HP;
}

// overide GamePlayer's attack
void Cleric::attack(Game* game) {
    // two consecutive attacks must have a tiem interval of at least FIGHTER_ATTACK_TIME_INTERVAL
    // otherwise, the second attack will not be initiated
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastAttackTime;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            < CLERIC_ATTACK_TIME_INTERVAL) {
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
    p->maxDistance = CLERIC_ATTACK_DISTANCE;
    p->ownerID = getID();
    p->type = CLERIC_SHOOT;
    p->speed = CLERIC_SHOOT_SPEED;
    p->direction = getFaceDirection();
    p->damage = getAttackDamage();
    game->projectiles.push_back(p);


    // Send an update to the clients: HEALING_OBJECTIVE_TAKEN
    GameUpdate attackUpdate;
    attackUpdate.updateType = PLAYER_ATTACK;
    attackUpdate.id = this->id;                        // id of player attacking
    attackUpdate.attackAmount = getAttackDamage();     // attack damage amount
    attackUpdate.roleClaimed = CLERIC;
    game->addUpdate(attackUpdate);

}

void Cleric::uniqueAttack(Game* game) {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastUniqueAttackTime;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            < HEALING_AURA_TIME_INTERVAL) {
        return;
    }

    lastUniqueAttackTime = currentTime; // update the lastUniqueAttackTime as this attack


    // draw the healing aura region
    PlayerPosition healingRegion = PlayerPosition();
    healingRegion.x = position.x;
    healingRegion.y = position.y;

    healingRegion.width = HEALING_AURA_WIDTH;
    healingRegion.height = HEALING_AURA_HEIGHT;

    // for every hunter, if their bounding box overlaps the healingRegion,
    // increase their hp
    float p1ULX = getUpperLeftCoordinateX(healingRegion, false);
    float p1ULY = getUpperLeftCoordinateY(healingRegion, false);
    float p1BRX = getBottomRightCoordinateX(healingRegion, false);
    float p1BRY = getBottomRightCoordinateY(healingRegion, false);

    for (int i = 0; i < PLAYER_NUM; i++) {
        // skip the player itself
        if (game->players[i] == this) continue;
        GamePlayer* otherPlayer = game->players[i];
        float p2ULX = getUpperLeftCoordinateX(otherPlayer->getPosition(), true);
        float p2ULY = getUpperLeftCoordinateY(otherPlayer->getPosition(), true);
        float p2BRX = getBottomRightCoordinateX(otherPlayer->getPosition(), true);
        float p2BRY = getBottomRightCoordinateY(otherPlayer->getPosition(), true);

        // https://www.geeksforgeeks.org/find-two-rectangles-overlap/
        // if one box is on left side of other
        if (p1ULX >= p2BRX || p2ULX >= p1BRX) continue;
        // if one box is above the other
        if (p1ULY >= p2BRY || p2ULY >= p1BRY) continue;

        if (!canAttack(game->players[i])) {
            int oldHp = game->players[i]->getHp();
            game->players[i]->hpIncrement(HEALING_AURA_HEALING_AMOUNT);
            int incAmount = game->players[i]->getHp() - oldHp;


            // queue this update to be send to other players
            GameUpdate gameUpdate;
            gameUpdate.updateType = PLAYER_HP_INCREMENT;
            gameUpdate.id = i;
            gameUpdate.damageTaken = incAmount;
            game->addUpdate(gameUpdate);
        }
    }
}

void Cleric::interact(Game* game) {
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