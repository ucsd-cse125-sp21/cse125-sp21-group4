#include "Mage.h"
#include "Game.h"

Mage::Mage() { 
    setType(MAGE); // Fighter type game component
    setHp(MAGE_MAX_HP); // init full health
    setAttackDamage(MAGE_ATTACK_DAMAGE);
}

Mage::Mage(PlayerPosition position) : GamePlayer(position) {
    setType(MAGE); // Fighter type game component
    setHp(MAGE_MAX_HP); // init full health
    setAttackDamage(MAGE_ATTACK_DAMAGE);
}

// overide GamePlayer's attack
void Mage::attack(Game* game) {
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
    p->ownerType = getType();
    p->type = MAGE_SHOOT;
    p->speed = MAGE_SHOOT_SPEED;
    p->direction = getFaceDirection();
    p->damage = getAttackDamage();
    game->projectiles.push_back(p);
}
