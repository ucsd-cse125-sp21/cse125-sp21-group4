#ifndef _GAMECONSTANT_H_
#define _GAMECONSTANT_H_

#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define PLAYER_NUM 4

#define FIGHTER_MAX_HP 100
#define MONSTER_MAX_HP 1000
#define FIGHTER_ATTACK_HARM 2
#define MONSTER_ATTACK_HARM 10

enum GameComponentType {
    SPACE, // 0
    FIGHTER, // 1
    GUNNER, // 2
    SUPPORT, // 3
    TRAPPER, // 4
    MONSTER, // 5
    OBSTACLE, // 6
};

enum Direction {
    NORTH, // 0
    EAST, // 1
    SOUTH, // 2
    WEST, // 3
};




#endif