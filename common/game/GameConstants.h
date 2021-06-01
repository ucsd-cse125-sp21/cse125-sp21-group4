#ifndef _GAMECONSTANT_H_
#define _GAMECONSTANT_H_

#define MAP_WIDTH 500
#define MAP_HEIGHT 600

#define GRID_WIDTH 1 // GRID_WIDTH should be able to fully divide MAP_WIDTH
#define GRID_HEIGHT 1 // GRID_HEIGHT should be able to fully divide MAP_HEIGHT
#define PLAYER_NUM 4 // used to change how many clients the server will accept
#define MAX_PLAYERS 4  // used to determine the game's absolute maximum player count
#define SELECT_SCREEN_TIME 20 // 20 seconds to select jobs

// safeRegion should be a circle with center in the middle of the map
#define SAFE_REGION_X 250
#define SAFE_REGION_Y 300
#define SAFE_REGION_START_RADIUS 250 
#define SAFE_REGION_RADIUS_DEC 100
#define SAFE_REGION_MIN_RADIUS 100.0
#define SAFE_REGION_DEC_TIME 120 // circle shrinks every 2 minutes 120s = 2min
#define SAFE_REGION_DAMAGE_TIME 10 // system will attack players outside of safe region every 10 seconds
#define SAFE_REGION_DAMAGE 10 

// Tile IDs
#define SPACE_ID 0
#define OBST_ID 1
#define HUNTER_HP_ID  2
#define HUNTER_ARMOR_ID 3
#define MONSTER_HP_ID 4
#define MONSTER_EVOLVE_ID 5
#define BEAC_ID 6

// Interaction Ranges for Objectives
#define BEACON_INTERACTION_RANGE 15
#define ARMOR_INTERACTION_RANGE 3
#define EVO_INTERACTION_RANGE 3
#define HEALING_INTERACTION_RANGE 3

// BEACON THRESHOLDS
#define MONSTER_BEACON_CAPTURE_THRESHOLD -30.f
#define HUNTER_BEACON_CAPTURE_THRESHOLD 30.f
#define DECAY_UPPER_THRESHOLD 1.f
#define DECAY_LOWER_THRESHOLD -1.f

// Delay between inputs in miliseconds
#define SPECTATE_INPUT_DELAY 100

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

// Attack Rate Constants
#define FIGHTER_ATTACK_TIME_INTERVAL 500 // 0.5s = 500 ms
#define ROGUE_ATTACK_TIME_INTERVAL 500 // 0.5s = 500 ms
#define MAGE_ATTACK_TIME_INTERVAL 300 // 0.3s = 300 ms
#define CLERIC_ATTACK_TIME_INTERVAL 300 // 0.3s = 300 ms
#define MONSTER_ATTACK_TIME_INTERVAL 500 // 0.5s = 500 ms

// Revive Time Constant
#define REVIVE_TIME_INTERVAL 20000 // 20s = 20000 ms

// Player Bouding Box Size Constants
#define FIGHTER_WIDTH 2
#define FIGHTER_HEIGHT 3
#define MAGE_WIDTH 2
#define MAGE_HEIGHT 3
#define CLERIC_WIDTH 2
#define CLERIC_HEIGHT 3
#define ROGUE_WIDTH 2
#define ROGUE_HEIGHT 3
#define MONSTER_WIDTH 6
#define MONSTER_HEIGHT 3

#define INIT_SPEED 1

// Player attack distance constants
#define FIGHTER_ATTACK_DISTANCE 4
#define FIGHTER_ATTACK_EXTRA_WIDTH 4
#define MONSTER_ATTACK_DISTANCE 6
#define MONSTER_ATTACK_EXTRA_WIDTH 4
#define ROGUE_ATTACK_DISTANCE 20
#define MAGE_ATTACK_DISTANCE 40
#define CLERIC_ATTACK_DISTANCE 15

//Revive distance constant
#define REVIVE_DISTANCE 4

// Player movement constants
#define FIGHTER_ACCELERATION 0.05
#define FIGHTER_MAX_SPEED 1
#define MAGE_ACCELERATION 0.05
#define MAGE_MAX_SPEED 1
#define CLERIC_ACCELERATION 0.05
#define CLERIC_MAX_SPEED 1
#define ROGUE_ACCELERATION 0.05
#define ROGUE_MAX_SPEED 1
#define MONSTER_ACCELERATION 0.05
#define MONSTER_MAX_SPEED 1



// Player attack speed (projectile moving speed)
#define ROGUE_ARROW_SPEED 2.5 // 2 units per tick
#define MAGE_SHOOT_SPEED 2 // 1 units per tick
#define CLERIC_SHOOT_SPEED 2 // 1 units per tick


// Player unique skill constants
// Fighter sword attack will apply damage overtime:
// 2 damages over 4 seconds. Timer resets on the next hit
#define FIGHTER_OVERTIME_DAMAGE_INTERVAL 2000 // 2000ms = 2s
#define FIGHTER_OVERTIME_DAMAGE_NUM 2
// ------------------------------------------------------
#define FIREBALL_SPEED_DEC 0.5 // decrease the speed by  0.5 unit per tick
#define FIREBALL_EFFECT_TIME 3000 // 3s = 3000ms
#define FIREBALL_TIME_INTERVAL 10000 // 10s = 10,000ms
#define FIREBALL_DISTANCE 20
#define FIREBALL_SPEED 1

#define HEALING_AURA_TIME_INTERVAL 10000 // 10s = 10,000ms
#define HEALING_AURA_WIDTH 14
#define HEALING_AURA_HEIGHT 14
#define HEALING_AURA_HEALING_AMOUNT 20

#define MONSTER_RANGED_ATTACK_DISTANCE 20
#define MONSTER_RANGED_SPEED 2

// Monster specific constants
#define MONSTER_FIRST_STAGE_THRESHOLD 1.f
#define MONSTER_SECOND_STAGE_THRESHOLD 2.f
#define MONSTER_THIRD_STAGE_THRESHOLD 3.f
#define MONSTER_FOURTH_STAGE_THRESHOLD 4.f
#define MONSTER_FIFTH_STAGE_THRESHOLD 5.f

// Increase Monster Evo every tick by this amount
#define MONSTER_EVO_TICK_INCREMENT 0.0005f

#define EVO_AMOUNT 1.5f

#define MAX_PROJECTILE_ID 10000

#define DELTA 0.001

// Spawn position for players 1,2,3,4 in format of {x, y}
// It is required that these spawn regions will not collide with
// each other or obstacles at the start
// const float P1_SPAWN_POSITION[2] = {5.0, 5.0};
// const float P2_SPAWN_POSITION[2] {15.0, 5.0};
// const float P3_SPAWN_POSITION[2] {5.0, 15.0};
// const float P4_SPAWN_POSITION[2] {15.0, 15.0};

const float SPAWN_POSITIONS [4][2] = {
    {25.0, 25.0},
    {35.0, 25.0},
    {25.0, 35.0},
    {65.0, 65.0}
};

#define MOVE_DISTANCE 1

enum Direction {
    NORTH, // 0
    EAST, // 1
    SOUTH, // 2
    WEST, // 3
    NORTH_WEST,
    NORTH_EAST, // 5
    SOUTH_WEST,
    SOUTH_EAST,
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
    MAGE_SHOOT,
    CLERIC_SHOOT,
    MAGE_FIREBALL,
    MONSTER_RANGED,
    UNKNOW_PROJECTILE,
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
typedef ProjectilePosition ProjectilePosition;


#endif