#include "Cleric.h"
#include "Game.h"

Cleric::Cleric() { 
    setType(CLERIC); // Fighter type game component
    setHp(CLERIC_MAX_HP); // init full health
    setAttackDamage(CLERIC_ATTACK_DAMAGE);
}

Cleric::Cleric(PlayerPosition position) : GamePlayer(position) {
    setType(CLERIC); // Fighter type game component
    setHp(CLERIC_MAX_HP); // init full health
    setAttackDamage(CLERIC_ATTACK_DAMAGE);
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
    p->ownerType = getType();
    p->type = CLERIC_SHOOT;
    p->speed = CLERIC_SHOOT_SPEED;
    p->direction = getFaceDirection();
    p->damage = getAttackDamage();
    game->projectiles.push_back(p);
}
