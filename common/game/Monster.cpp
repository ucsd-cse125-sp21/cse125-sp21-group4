#include "Monster.h"
#include "Game.h"


Monster::Monster() { 
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
}

Monster::Monster(PlayerPosition position) : GamePlayer(position){
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
}

// monster ranged attack
void Monster::attack(Game* game) {
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
    p->ownerType = getType();
    p->type = MONSTER_RANGED; 
    p->speed = MONSTER_RANGED_SPEED; //this?
    p->direction = getFaceDirection();
    p->damage = getAttackDamage();
    game->projectiles.push_back(p);
}