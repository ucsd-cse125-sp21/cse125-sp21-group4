#include "Beacon.h"

Beacon::Beacon(GridPosition position, Restriction r, int freq) {
    setType(OBJECTIVE);
    setRestriction(r);
    setObjective(BEAC);
    setPosition(position.x, position.y);
    setInteractionRange(BEACON_INTERACTION_RANGE);

    frequency = freq;
    captured = false;
    tickCounter = 0;
    captureAmount = 0;
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

void Beacon::updateCaptureAmount(float captureDelta){
    captureAmount += captureDelta;
}
void Beacon::decayCaptureAmount(){
    if(captureAmount < 0) {
        captureAmount += DECAY_CAPTURE_RATE;
    } else if (captureAmount > 0) {
        captureAmount -= DECAY_CAPTURE_RATE;
    }
}

void Beacon::incrementTickCounter(){
    tickCounter++;
}
void Beacon::resetTickCounter(){
    tickCounter = 0;
}