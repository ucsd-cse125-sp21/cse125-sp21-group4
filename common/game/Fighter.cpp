#include "Fighter.h"
#include "Game.h"

using namespace std;

Fighter::Fighter() { 
    setType(FIGHTER); // Fighter type game component
    setHp(FIGHTER_MAX_HP); // init full health
    maxHp = FIGHTER_MAX_HP;
    setAttackDamage(FIGHTER_ATTACK_DAMAGE);
    setAcceleration(FIGHTER_ACCELERATION);
    setMaxSpeed(FIGHTER_MAX_SPEED);
    setShieldOn(false);
}

Fighter::Fighter(PlayerPosition position) : GamePlayer(position) {
    setType(FIGHTER); // Fighter type game component
    setHp(FIGHTER_MAX_HP); // init full health
    maxHp = FIGHTER_MAX_HP;
    setAttackDamage(FIGHTER_ATTACK_DAMAGE);
    setAcceleration(FIGHTER_ACCELERATION);
    setMaxSpeed(FIGHTER_MAX_SPEED);
    setShieldOn(false);
}

void Fighter::hpDecrement (int damage, bool fromSystem) {
    // if shield is on, fighter cannot be attacked by the monster
    if (getShieldOn() && !fromSystem) return;
    if (hp == 0) return;

    hp = std::max(0, hp - damage);
    if (hp == 0) {
        deathTime = std::chrono::steady_clock::now();
    }
}

bool Fighter::getShieldOn () { return shieldOn; }

void Fighter::setShieldOn (bool newShieldOn)  { shieldOn = newShieldOn; }


/*
    Fighter's attack function
    Player can attack the enemy whose bounding box has overlapped with the
    following region.
        ----------
        | attack | distance
        ---------
        |Bounding|
        | Box    | player height
        ----------
        Player width
*/
void Fighter::attack(Game* game, float angle) {

    // two consecutive attacks must have a time interval of at least FIGHTER_ATTACK_TIME_INTERVAL
    // otherwise, the second attack will not be initiated
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastAttackTime;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            < FIGHTER_ATTACK_TIME_INTERVAL) {
        return;
    }

    lastAttackTime = currentTime; // update the lastAttackTime as this attack

    // draw the attack region
    PlayerPosition attackRegion = PlayerPosition();

    if ((angle >= M_PI/4 && angle <= M_PI /2) || (angle <= -5*M_PI/4 && angle >= -3*M_PI/2)) {
        attackRegion.width = position.width + FIGHTER_ATTACK_EXTRA_WIDTH;
        attackRegion.height = FIGHTER_ATTACK_DISTANCE;
        attackRegion.x = position.x;
        attackRegion.y = position.y - position.height/2 - FIGHTER_ATTACK_DISTANCE / 2;
    } else if ((angle >= 0 && angle <= M_PI /4) || (angle <= 0 && angle >= -1*M_PI/4)) {
        attackRegion.height = position.height + FIGHTER_ATTACK_EXTRA_WIDTH;
        attackRegion.width = FIGHTER_ATTACK_DISTANCE;
        attackRegion.y = position.y;
        attackRegion.x = position.x + position.width/2 + FIGHTER_ATTACK_DISTANCE / 2;
    } else if (angle >= -3*M_PI/4 && angle <= -1*M_PI /4) {
        attackRegion.width = position.width + FIGHTER_ATTACK_EXTRA_WIDTH;
        attackRegion.height = FIGHTER_ATTACK_DISTANCE;
        attackRegion.x = position.x;
        attackRegion.y = position.y + position.height/2 + FIGHTER_ATTACK_DISTANCE / 2;
    } else if (angle >= -5*M_PI/4 && angle <= -3*M_PI /4) {
        attackRegion.height = position.height + FIGHTER_ATTACK_EXTRA_WIDTH;
        attackRegion.width = FIGHTER_ATTACK_DISTANCE;
        attackRegion.y = position.y;
        attackRegion.x = position.x - position.width/2 - FIGHTER_ATTACK_DISTANCE / 2;
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
        // skip the dead player
        if (game->players[i]->isDead()) continue;
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
            game->players[i]->hpDecrement(attackDamage, false);
            // cancel all the prescheduled damage overtime
            std::vector<GameEvent*> newEvents;
            for (auto iter = game->events.begin(); iter != game->events.end(); iter++) {
                GameEvent* event = *iter;
                if (event->ownerID == getID() && event->type == HP_DEC) delete event;
                else newEvents.push_back(event);
            }   
            game->events = newEvents;    
            
            // schedule overtime damage
            for (int n = 0; n < FIGHTER_OVERTIME_DAMAGE_NUM; n++) {
                GameEvent* event = new GameEvent();
                event->type = HP_DEC;
                event->ownerID = getID();
                event->targetID = i;
                event->amount = attackDamage;
                event->time = std::chrono::steady_clock::now() + 
                            std::chrono::milliseconds((n+1)*FIGHTER_OVERTIME_DAMAGE_INTERVAL);
                game->events.push_back(event);
            }


            // queue this update to be send to other players
            GameUpdate gameUpdate;
            gameUpdate.updateType = PLAYER_DAMAGE_TAKEN;
            gameUpdate.id = i;
            gameUpdate.damageTaken = attackDamage;
            game->addUpdate(gameUpdate);
        }
    }

    // Send an update to the clients: HEALING_OBJECTIVE_TAKEN
    GameUpdate attackUpdate;
    attackUpdate.updateType = PLAYER_ATTACK;
    attackUpdate.id = this->id;                        // id of player attacking
    attackUpdate.attackAmount = getAttackDamage();     // attack damage amount
    attackUpdate.roleClaimed = FIGHTER;
    game->addUpdate(attackUpdate);
}


void Fighter::uniqueAttack(Game* game, float angle) {
    std::vector<GameEvent*> newEvents;
    for (auto iter = game->events.begin(); iter != game->events.end(); iter++) {
        GameEvent* event = *iter;
        if (event->ownerID == getID() && event->targetID == getID() && event->type == TAKE_FIGHTER_SHIELD_DOWN) delete event;
        else newEvents.push_back(event);
    }   
    game->events = newEvents;   

    // if shield is already up, no need send update to client, simply delay the down time
    // if shield is not up, send update to client, and calculate down time
    if (!getShieldOn()) {
        setShieldOn(true);
        // Send an update to the clients: PLAYER_UNIQUE_ATTACK
        GameUpdate update;
        update.updateType = FIGHTER_SHIELD_UP;
        update.id = this->id;                      
        game->addUpdate(update);
    }

    GameEvent* event = new GameEvent();
    event->type = TAKE_FIGHTER_SHIELD_DOWN;
    event->ownerID = getID();
    event->targetID = getID();
    event->time = std::chrono::steady_clock::now() + std::chrono::milliseconds(FIGHTER_SHIELD_HOLD_TIME);
    game->events.push_back(event);
}




void Fighter::interact(Game* game) {
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
