#ifndef CONSTANTS_HEADER
#define CONSTANTS_HEADER



#define DEFAULT_PORT "27015"

// #define ERICS_LOCAL_SERVER "192.168.0.110"
#define ERICS_LOCAL_SERVER "127.0.0.1"

#define TICK_TIME 500

enum CLIENT_INPUT{
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    NO_MOVE,
    ATTACK, // general attack for now
    // MONSTER_SPIT_RANGED_ATTACK,
    // MONSTER_MELEE_ATTACK,
    // HUNTER_SWORD_ATTACK,
};




#endif