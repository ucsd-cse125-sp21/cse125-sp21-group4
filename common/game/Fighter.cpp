#include "Fighter.h"
#include "Game.h"

Fighter::Fighter() { 
    setType(FIGHTER); // Fighter type game component
    setHp(FIGHTER_MAX_HP); // init full health
    setAttackDamage(FIGHTER_ATTACK_DAMAGE);
}

Fighter::Fighter(PlayerPosition position) : GamePlayer(position) {
    setType(FIGHTER); // Fighter type game component
    setHp(FIGHTER_MAX_HP); // init full health
    setAttackDamage(FIGHTER_ATTACK_DAMAGE);
}

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
void Fighter::attack(Game* game) {

    // two consecutive attacks must have a tiem interval of at least FIGHTER_ATTACK_TIME_INTERVAL
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
    if (faceDirection == NORTH || faceDirection == SOUTH) {
        attackRegion.x = position.x;
        attackRegion.width = position.width;
        attackRegion.height = FIGHTER_ATTACK_DISTANCE;
        if (faceDirection == NORTH)
            attackRegion.y = position.y - position.height/2 - FIGHTER_ATTACK_DISTANCE / 2;
        else
            attackRegion.y = position.y + position.height/2 + FIGHTER_ATTACK_DISTANCE / 2;
    } else {
        attackRegion.y = position.y;
        attackRegion.width = FIGHTER_ATTACK_DISTANCE;
        attackRegion.height = position.width;
        if (faceDirection = EAST)
            attackRegion.x = position.x + position.height / 2 + FIGHTER_ATTACK_DISTANCE / 2;
        else
            attackRegion.x = position.x - position.height / 2 - FIGHTER_ATTACK_DISTANCE / 2;
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

            // queue this update to be send to other players
            GameUpdate gameUpdate;
            gameUpdate.updateType = PLAYER_DAMAGE_TAKEN;
            gameUpdate.id = i;
            gameUpdate.damageTaken = attackDamage;
            game->addUpdate(gameUpdate);
        }
    }
}
