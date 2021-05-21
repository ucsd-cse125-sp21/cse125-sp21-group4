#include "Beacon.h"

Beacon::Beacon(GridPosition position, Restriction r, int freq) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(BEACON);
    setPosition(position.x, position.y);
    setInteractionRange(BEACON_INTERACTION_RANGE);

    frequency = freq;
    captured = false;
    tickCounter = 0;
    captureAmount = 0;
    objectiveID = Objective::getNextValidID(); 
}

int Beacon::getFrequency() {
    return frequency;
}

bool Beacon::isCaptured(){
    return captured;
}
bool Beacon::canPing(){
    return tickCounter >= frequency;
}

void Beacon::updateCaptureAmount(Game* game, float captureDelta){

    // If it's not captured, update the capture amount
    if(!captured) {
        captureAmount += captureDelta;

        // If it's past the threshold, update captured boolean.
        if(captureAmount <= MONSTER_BEACON_CAPTURE_THRESHOLD || captureAmount >= HUNTER_BEACON_CAPTURE_THRESHOLD) {
            captured = true;
            GameUpdate beaconCapturedUpdate;
            beaconCapturedUpdate.updateType = BEACON_CAPTURED;
            beaconCapturedUpdate.beaconCaptureAmount = captureAmount;
            game->addUpdate(beaconCapturedUpdate);

            // allows the map to update immediately instead of sending just the spawn positions
            tickCounter = frequency; 
        }
    }
}
void Beacon::decayCaptureAmount(){
    if(captureAmount < 0) {
        captureAmount += DECAY_CAPTURE_RATE;
    } else if (captureAmount > 0) {
        captureAmount -= DECAY_CAPTURE_RATE;
    }
}

float Beacon::getCaptureAmount() {
    return captureAmount;
}

void Beacon::incrementTickCounter(){
    tickCounter++;
}
void Beacon::resetTickCounter(){
    tickCounter = 0;
}