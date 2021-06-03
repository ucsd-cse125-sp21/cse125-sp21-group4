#include <chrono>

enum EventType {
    HP_DEC,
    SPEED_CHANGE,
    FIREBALL_END,
    HEAL_END,
    GAME_START,
    TAKE_FIGHTER_SHIELD_DOWN,
};

class GameEvent {
public:
    EventType type;
    int ownerID; // the action trigerer's id
    int targetID; // the target GamePlayer's id
    float amount; // the numeric value in case of the event
    std::chrono::steady_clock::time_point time; // the time it will be triggered
};