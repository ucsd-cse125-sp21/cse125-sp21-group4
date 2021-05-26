#include "Monster.h"
#include "Game.h"


Monster::Monster() { 
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    maxHp = MONSTER_MAX_HP;
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
    setEvo(MONSTER_FIRST_STAGE_THRESHOLD);
    setAcceleration(MONSTER_ACCELERATION);
    setMaxSpeed(MONSTER_MAX_SPEED);
}

Monster::Monster(PlayerPosition position) : GamePlayer(position){
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    maxHp = MONSTER_MAX_HP;
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
    setEvo(MONSTER_FIRST_STAGE_THRESHOLD);
    setAcceleration(MONSTER_ACCELERATION);
    setMaxSpeed(MONSTER_MAX_SPEED);
}

// monster ranged attack
void Monster::attack(Game* game, float angle) {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastAttackTime;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            < MONSTER_ATTACK_TIME_INTERVAL) {
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
    p->maxDistance = MONSTER_RANGED_ATTACK_DISTANCE; 
    p->deltaX = FIREBALL_SPEED * cos(angle);
    p->deltaY = -1 * FIREBALL_SPEED * sin(angle);
    p->ownerID = getID();
    p->type = MONSTER_RANGED; 
    p->damage = getAttackDamage();
    game->projectiles[game->nextProjectileId] = p;
    game->nextProjectileId = (game->nextProjectileId + 1) % MAX_PROJECTILE_ID;

    // Send an update to the clients: MONSTER HAS ATTACKED
    GameUpdate attackUpdate;
    attackUpdate.updateType = PLAYER_ATTACK;
    attackUpdate.id = this->id;                        // id of player attacking
    attackUpdate.attackAmount = getAttackDamage();     // attack damage amount
    attackUpdate.roleClaimed = MONSTER;
    game->addUpdate(attackUpdate);
}

void Monster::uniqueAttack(Game* game, float angle) {
    // two consecutive attacks must have a time interval of at least MONSTER_ATTACK_TIME_INTERVAL
    // otherwise, the second attack will not be initiated
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastAttackTime;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            < MONSTER_ATTACK_TIME_INTERVAL) {
        return;
    }

    lastAttackTime = currentTime; // update the lastAttackTime as this attack

    // draw the attack region
    PlayerPosition attackRegion = PlayerPosition();
    if ((angle >= M_PI/4 && angle <= M_PI /2) || (angle <= -5*M_PI/4 && angle >= -3*M_PI/2)) {
        attackRegion.width = position.width + MONSTER_ATTACK_EXTRA_WIDTH;
        attackRegion.height = MONSTER_ATTACK_DISTANCE;
        attackRegion.x = position.x;
        attackRegion.y = position.y - position.height/2 - MONSTER_ATTACK_DISTANCE / 2;
    } else if ((angle >= 0 && angle <= M_PI /4) || (angle <= 0 && angle >= -1*M_PI/4)) {
        attackRegion.height = position.height + MONSTER_ATTACK_EXTRA_WIDTH;
        attackRegion.width = MONSTER_ATTACK_DISTANCE;
        attackRegion.y = position.y;
        attackRegion.x = position.x + position.width/2 + MONSTER_ATTACK_DISTANCE / 2;
    } else if (angle >= -3*M_PI/4 && angle <= -1*M_PI /4) {
        attackRegion.width = position.width + MONSTER_ATTACK_EXTRA_WIDTH;
        attackRegion.height = MONSTER_ATTACK_DISTANCE;
        attackRegion.x = position.x;
        attackRegion.y = position.y + position.height/2 + MONSTER_ATTACK_DISTANCE / 2;
    } else if (angle >= -5*M_PI/4 && angle <= -3*M_PI /4) {
        attackRegion.height = position.height + MONSTER_ATTACK_EXTRA_WIDTH;
        attackRegion.width = MONSTER_ATTACK_DISTANCE;
        attackRegion.y = position.y;
        attackRegion.x = position.x - position.width/2 - MONSTER_ATTACK_DISTANCE / 2;
    }

    // for every player, if their bounding box overlaps the attackRegion, and
    // they are enemies of this player, decrement their hp
    float p1ULX = getUpperLeftCoordinateX(attackRegion, false);
    float p1ULY = getUpperLeftCoordinateY(attackRegion, false);
    float p1BRX = getBottomRightCoordinateX(attackRegion, false);
    float p1BRY = getBottomRightCoordinateY(attackRegion, false);

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

        if (canAttack(game->players[i])) {
            game->players[i]->hpDecrement(attackDamage);
            // cancel all the prescheduled damage overtime
            std::vector<GameEvent*> newEvents;
            for (auto iter = game->events.begin(); iter != game->events.end(); iter++) {
                GameEvent* event = *iter;
                if (event->ownerID == getID() && event->type == HP_DEC) delete event;
                else newEvents.push_back(event);
            }   
            game->events = newEvents;    

            // queue this update to be send to other players
            GameUpdate gameUpdate;
            gameUpdate.updateType = PLAYER_DAMAGE_TAKEN;
            gameUpdate.id = i;
            gameUpdate.damageTaken = attackDamage;
            game->addUpdate(gameUpdate);
        }
    }
}
// To update the stage of the monster.
void Monster::updateEvo(Game* game, float evoLevel) {
    
    // For now update the attack damage of the monster, maybe speed another time?
    if (evoLevel >= MONSTER_FIFTH_STAGE_THRESHOLD) {
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

                    // Send an update to the clients: HEALING_OBJECTIVE_TAKEN
                    GameUpdate evoObjectiveTakenUpdate;
                    evoObjectiveTakenUpdate.updateType = EVO_OBJECTIVE_TAKEN;
                    evoObjectiveTakenUpdate.id = this->id;                        // id of player being healed
                    evoObjectiveTakenUpdate.objectiveID = evoObj->getObjectiveID();
                    evoObjectiveTakenUpdate.gridPos = evoObj->getPosition();     // obj location
                    game->addUpdate(evoObjectiveTakenUpdate);

                    // Clean up the evo grid.
                    game->consumeObj(evoObj);
                    break;
                }
                case HEAL:
                    // use GamePlayer's interactHeal.
                    interactHeal(game, (Heal*) obj);
                    break;
                case BEACON:
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