#ifndef _GAMECONSTANT_H_
#define _GAMECONSTANT_H_

#define MAP_WIDTH 600
#define MAP_HEIGHT 600
#define GRID_WIDTH 1 // GRID_WIDTH should be able to fully divide MAP_WIDTH
#define GRID_HEIGHT 1 // GRID_HEIGHT should be able to fully divide MAP_HEIGHT
#define PLAYER_NUM 4 // used to change how many clients the server will accept
#define MAX_PLAYERS 4  // used to determine the game's absolute maximum player count

// MAX HP Constants
#define FIGHTER_MAX_HP 100
#define MAGE_MAX_HP 100
#define CLERIC_MAX_HP 85
#define ROGUE_MAX_HP 100
#define MONSTER_MAX_HP 1000

// Attack Harm Constants
#define FIGHTER_ATTACK_DAMAGE 2
#define MAGE_ATTACK_DAMAGE 1.5
#define CLERIC_ATTACK_DAMAGE 1
#define ROGUE_ATTACK_DAMAGE 2
#define MONSTER_ATTACK_DAMAGE 10

// Atack Rate Constants
#define FIGHTER_ATTACK_TIME_INTERVAL 500 // 0.5s = 500 ms

// Player Bouding Box Size Constants
#define FIGHTER_WIDTH 4
#define FIGHTER_HEIGHT 4
#define MAGE_WIDTH 4
#define MAGE_HEIGHT 4
#define CLERIC_WIDTH 4
#define CLERIC_HEIGHT 4
#define ROGUE_WIDTH 4
#define ROGUE_HEIGHT 4
#define MONSTER_WIDTH 6
#define MONSTER_HEIGHT 6

#define MOVE_DISTANCE 1

// Player attack distance constants
#define FIGHTER_ATTACK_DISTANCE 2
#define ROGUE_ATTACK_DISTANCE 20

// Player attack speed (projectile moving speed)
#define ROGUE_ARROW_SPEED 2 // 2 units per tick


#define DELTA 0.001

// Spawn position for players 1,2,3,4 in format of {x, y}
// It is required that these spawn regions will not collide with
// each other or obstacles at the start
const float P1_SPAWN_POSITION[2] = {5.0, 5.0};
const float P2_SPAWN_POSITION[2] {15.0, 5.0};
const float P3_SPAWN_POSITION[2] {5.0, 15.0};
const float P4_SPAWN_POSITION[2] {15.0, 15.0};


#define MOVE_DISTANCE 1

enum Direction {
    NORTH, // 0
    EAST, // 1
    SOUTH, // 2
    WEST, // 3
};

enum PlayerType {
    UNKNOWN,
    MONSTER,
    FIGHTER,
    MAGE,
    CLERIC,
    ROGUE,
};


enum ProjectileType {
    ROGUE_ARROW,
    MAGE_FIREBALL,
    // MAGE_SHOOT,
    // CLERIC_SHOOT,
    // MONSTER_SHOOT,
};


struct PlayerPosition {
    float x; // x coordinate of center
    float y; // y coordinate of center
    float width; // width of the bounding box
    float height; // height of the bounding box
};
typedef PlayerPosition PlayerPosition;

struct ProjectilePosition {
    float x;
    float y;
};
typedef PlayerPosition PlayerPosition;


#endif