#ifndef _GAMECONSTANT_H_
#define _GAMECONSTANT_H_

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define GRID_WIDTH 1 // GRID_WIDTH should be able to fully divide MAP_WIDTH
#define GRID_HEIGHT 1 // GRID_HEIGHT should be able to fully divide MAP_HEIGHT
#define PLAYER_NUM 4

#define FIGHTER_MAX_HP 100
#define MONSTER_MAX_HP 1000
#define FIGHTER_ATTACK_HARM 2
#define MONSTER_ATTACK_HARM 10

#define FIGHTER_WIDTH 4
#define FIGHTER_HEIGHT 4
#define MONSTER_WIDTH 6
#define MONSTER_HEIGHT 6

#define MOVE_DISTANCE 1
#define ATTACK_DISTANCE 2

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




#endif