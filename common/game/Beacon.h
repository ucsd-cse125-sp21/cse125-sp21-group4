#ifndef _BEACON_
#define _BEACON_

#include "Objective.h"

class Beacon : public Objective {
    private:
        int frequency; // frequency to ping (game ticks).
        int tickCounter; // counter which will be used to compare to frequency
        float captureAmount; // -30 (M) ----- 0 (neutral) ------ 30 (H)
        bool captured;


    public:
        // These are Dependent on Tickrate, hence they aren't macros
        const float MONSTER_CAPTURE_RATE = 1.f / TICK_TIME; 
        const float HUNTER_CAPTURE_RATE = 1.f / TICK_TIME / 3; 
        const float DECAY_CAPTURE_RATE = 1.f / TICK_TIME / 6;

        // time assigned to freq (30 * 1000 / TICK_TIME == amount of ticks for 30 seconds)
        Beacon(GridPosition position, Restriction r = R_NEUTRAL, int freq = 30 * 1000 / TICK_TIME);

        int getFrequency();
        bool isCaptured();
        bool canPing();

        void updateCaptureAmount(float captureDelta);
        void decayCaptureAmount();

        void incrementTickCounter();
        void resetTickCounter();

};

#endif