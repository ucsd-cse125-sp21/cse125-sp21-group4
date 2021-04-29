#include "Rogue.h"
#include "Game.h"

Rogue::Rogue() { 
    setType(ROGUE); // Fighter type game component
    setHp(ROGUE_MAX_HP); // init full health
    setAttackDamage(ROGUE_ATTACK_DAMAGE);
    maxHp = ROGUE_MAX_HP;
}

Rogue::Rogue(PlayerPosition position) : GamePlayer(position) {
    setType(ROGUE); // Fighter type game component
    setHp(ROGUE_MAX_HP); // init full health
    setAttackDamage(ROGUE_ATTACK_DAMAGE);
    maxHp = ROGUE_MAX_HP;
}

// overide GamePlayer's attack
void Rogue::attack(Game* game) {
    // two consecutive attacks must have a tiem interval of at least FIGHTER_ATTACK_TIME_INTERVAL
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
