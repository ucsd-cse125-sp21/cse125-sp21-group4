#ifndef CONSTANTS_HEADER
#define CONSTANTS_HEADER

#include "../game/GridComponent.h"

#define DEFAULT_PORT "27015"

// #define ERICS_LOCAL_SERVER "192.168.0.110"
#define ERICS_LOCAL_SERVER "127.0.0.1"

#define TICK_TIME 33 // 33ms == 30 tick server. 1 second / 30 ticks = 33 ms

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

// Type of update sent to the server
enum UPDATE_TYPE {
    PLAYER_DAMAGE_TAKEN,
    PLAYER_MOVE,
    PROJECTILE_MOVE,
    OBJECTIVE_BEING_TAKEN,
    OBJECTIVE_TAKEN,
    INVALID_UPDATE, // this will be sent if people forget to update the updateType

};

// GameUpdate packet payload
struct GameUpdate {
    UPDATE_TYPE updateType = INVALID_UPDATE;
    int id = -1; // -1 instead of 0 because 0 is a valid player id.
    int damageTaken = 0; // Used for a damage taken evenrt
    GridPosition gridPos = {0,0}; // Used for objectives and obstacles
    float floatDeltaX = 0; // used for player/projectile movement
    float floatDeltaY = 0; // used for player/projectile movement
};



#endif