#include "Rogue.h"
#include "Game.h"

Rogue::Rogue() { 
    setType(ROGUE); // Fighter type game component
    setHp(ROGUE_MAX_HP); // init full health
    setAttackDamage(ROGUE_ATTACK_DAMAGE);
}

Rogue::Rogue(PlayerPosition position) : GamePlayer(position) {
    setType(ROGUE); // Fighter type game component
    setHp(ROGUE_MAX_HP); // init full health
    setAttackDamage(ROGUE_ATTACK_DAMAGE);
}

// overide GamePlayer's attack
void Rogue::attack(Game* game) {
    ProjectilePosition position = {
        getPosition().x,
        getPosition().y,
    };
    Projectile* p = new Projectile();
    p->origin = position;
    p->currentPosition = position; 
    p->maxDistance = ROGUE_ATTACK_DISTANCE;
    p->ownerType = getType();
    p->type = ROGUE_ARROW;
    p->speed = ROGUE_ARROW_SPEED;
    p->direction = getFaceDirection();
    p->damage = getAttackDamage();
    game->projectiles.push_back(p);
}
