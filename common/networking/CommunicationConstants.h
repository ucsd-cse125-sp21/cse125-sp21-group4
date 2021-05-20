#ifndef CONSTANTS_HEADER
#define CONSTANTS_HEADER

#include "../game/GridComponent.h"
#include "../game/GameConstants.h"
#include "../game/Objective.h"

#include <chrono>

#define DEFAULT_PORT "27168"

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
    UNIQUE_ATTACK, // mage fireball and cleric healing aura
    // MONSTER_SPIT_RANGED_ATTACK,
    // MONSTER_MELEE_ATTACK,
    // HUNTER_SWORD_ATTACK,
    INTERACT, // used to interact with objectives

    // These inputs are so players can claim a role
    CLAIM_FIGHTER,
    CLAIM_CLERIC,
    CLAIM_MAGE, 
    CLAIM_ROGUE,

    // Used to tell the server to start the select screen timer
    DONE_RENDERING,
};

// Type of update sent to the server
enum UPDATE_TYPE {
    PLAYER_DAMAGE_TAKEN,
    PLAYER_HP_INCREMENT,
    PLAYER_ATTACK,
    PLAYER_MOVE,
    PROJECTILE_MOVE,

    BEACON_BEING_TAKEN,
    BEACON_DECAYING,
    BEACON_CAPTURED,
    BEACON_PING_PLAYER,

    SPAWN_OBJECTIVE,
    HEAL_OBJECTIVE_TAKEN,
    ARMOR_OBJECTIVE_TAKEN,
    EVO_OBJECTIVE_TAKEN,
    MONSTER_EVO_UP,

    ROLE_CLAIMED, // Role claimed
    GAME_STARTED, // Game Start
    ALL_PLAYERS_JOINED, // Tells the client that all players have joined


    GAME_END, // endStatus can be 1(hunters win), 2(monster win), 3(tie)
    INVALID_UPDATE, // this will be sent if people forget to update the updateType


};

// GameUpdate packet payload
struct GameUpdate {
    UPDATE_TYPE updateType = INVALID_UPDATE;
    int id = -1; // -1 instead of 0 because 0 is a valid player id.
    int objectiveID = -1; // -1 instead of 0 for valid objectiveID.
    int damageTaken = 0; // Used for a damage taken event
    int attackAmount = 0; // amount of damage per attack
    int healAmount = 0; // Used for a healing event
    int endStatus = 0;
    GridPosition gridPos = {0,0}; // Used for objectives and obstacles
    PlayerPosition playerPos = {0.f, 0.f}; // Used for player positions
    float newEvoLevel = -1.f; // Used for monster stage tracking
    float floatDeltaX = 0; // used for player/projectile movement
    float floatDeltaY = 0; // used for player/projectile movement
    float beaconCaptureAmount = -9999;
    PlayerType roleClaimed = UNKNOWN;
    std::chrono::steady_clock::time_point selectTimerStartTime; // Used for an accurate countdown timer
    ObjectiveType objectiveSpawnType = INVALID_OBJ;
    Restriction objRestrictionType = R_NEUTRAL;
};



#endif