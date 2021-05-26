#ifndef _AUDIOPROGRAM_H_
#define _AUDIOPROGRAM_H_

// Audio Integration with SoLoud
// #define WITH_MINIAUDIO
#include "audio/include/soloud.h"
#include "audio/include/soloud_wav.h"
#include "audio/include/soloud_wavstream.h"

#include <map>
#include <vector>

#include "../common/game/GameConstants.h"

#define MAX_HEARING_DISTANCE 20.0f
#define DEFAULT_MUSIC_VOLUME 0.55f
#define DEFAULT_EFFECTS_VOLUME 1.0f

struct AudioPosition {
    float x;
    float y;
};

enum Sound {
    TITLE_MUSIC,
    COMBAT_MUSIC,
    FIGHTER_ATTACK_SOUND,
    MAGE_FIREBALL_SOUND,
    MAGE_ATTACK_SOUND,
    MONSTER_THROW_SOUND,
    CLERIC_ATTACK_SOUND,
    ROGUE_ATTACK_SOUND,
    SELECT_SOUND,

};

class AudioProgram {
private:
	SoLoud::Soloud soLoudEngine;
	std::map<Sound, SoLoud::WavStream*> musicMap;
    std::map<Sound, SoLoud::Wav*> effectMap;

    std::vector<int> musicHandles;

    float musicVolume, effectsVolume;
    bool isMuted;


public:
    AudioProgram();
    void initializeAllAudioFiles();
    void playAudioWithoutLooping(Sound);
    void playAudioWithLooping(Sound);
    void stopAudio(Sound);
    void setSpecificVolume(Sound, float);
    void setAllVolume(float);
    void setEffectsVolume(float);
    void setMusicVolume(float);
    float getSoundLength(Sound);
    void playDirectionalEffect(Sound, AudioPosition, AudioPosition);
    void toggleMute();
};

#endif