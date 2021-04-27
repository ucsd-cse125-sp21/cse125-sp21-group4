#ifndef CONSTANTS_HEADER
#define CONSTANTS_HEADER

#include "../game/GridComponent.h"

#define DEFAULT_PORT "31545"

//#define ERICS_LOCAL_SERVER "192.168.0.110"
#define ERICS_LOCAL_SERVER "127.0.0.1"

#define TICK_TIME 33 // 33ms == 30 tick server. 1 second / 30 ticks = 33 ms

enum CLIENT_INPUT{
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    NO_MOVE,
    ATTACK, // general attack for now
    INTERACT, // used to interact with objectives
};

// Type of update sent to the server
enum UPDATE_TYPE {
    PLAYER_DAMAGE_TAKEN,
    PLAYER_MOVE,
    PROJECTILE_MOVE,
    BEACON_BEING_TAKEN,
    BEACON_DECAYING,
    BEACON_CAPTURED,
    BEACON_PING_PLAYER,
    HEAL_OBJECTIVE_TAKEN,
    ARMOR_OBJECTIVE_TAKEN,
    EVO_OBJECTIVE_TAKEN,
    MONSTER_EVO_UP,
    INVALID_UPDATE, // this will be sent if people forget to update the updateType

};

// GameUpdate packet payload
struct GameUpdate {
    UPDATE_TYPE updateType = INVALID_UPDATE;
    int id = -1; // -1 instead of 0 because 0 is a valid player id.
    int damageTaken = 0; // Used for a damage taken event
    int healAmount = 0; // Used for a healing event
    GridPosition gridPos = {0,0}; // Used for objectives and obstacles
    PlayerPosition playerPos = {0.f, 0.f}; // Used for player positions
    float newEvoLevel = -1.f; // Used for monster stage tracking
    float floatDeltaX = 0; // used for player/projectile movement
    float floatDeltaY = 0; // used for player/projectile movement
    float beaconCaptureAmount = -9999;
};



#endif