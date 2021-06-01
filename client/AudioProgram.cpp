#include "AudioProgram.h"

// Audio program constructor
AudioProgram::AudioProgram() {
    soLoudEngine.init();
    initializeAllAudioFiles();

    musicVolume = 0.55f;
    effectsVolume = 1.0f;
    isMuted = false;
}

// Initializes all the audio files.
// audio/game_mp3/README.md contains all the licensing information for audio files 
// that were not created by us.
void AudioProgram::initializeAllAudioFiles() {

    // ====================== Music ====================== //
    // SoLoud::WavStream is for long-playing music... (streams from disk)
    SoLoud::WavStream* titleMusic = new SoLoud::WavStream();
    titleMusic->load("audio/game_mp3/Grieve_Title_Screen.mp3");
    musicMap[TITLE_MUSIC] = titleMusic;

    SoLoud::WavStream* combatMusic = new SoLoud::WavStream();
    combatMusic->load("audio/game_mp3/Grieve_Combat.mp3");
    musicMap[COMBAT_MUSIC] = combatMusic;

    // ====================== Sound Effects ====================== //
    // SoLoud::Wav is for quick sound effects...

    // Fighter Sounds
    SoLoud::Wav* fighterAttackEffect =  new SoLoud::Wav();
    fighterAttackEffect->load("audio/game_mp3/fighter_attack.mp3");
    effectMap[FIGHTER_ATTACK_SOUND] = fighterAttackEffect;

    // Mage Sounds
    SoLoud::Wav* mageFireballEffect =  new SoLoud::Wav();
    mageFireballEffect->load("audio/game_mp3/mage_fireball.wav");
    effectMap[MAGE_FIREBALL_SOUND] = mageFireballEffect;
    SoLoud::Wav* mageAttackEffect =  new SoLoud::Wav();
    mageAttackEffect->load("audio/game_mp3/mage_attack.wav");
    effectMap[MAGE_ATTACK_SOUND] = mageAttackEffect;

    // Cleric Sounds
    SoLoud::Wav* clericAttackEffect =  new SoLoud::Wav();
    clericAttackEffect->load("audio/game_mp3/cleric_attack.wav");
    effectMap[CLERIC_ATTACK_SOUND] = clericAttackEffect;
    SoLoud::Wav* clericHealEffect =  new SoLoud::Wav();
    clericHealEffect->load("audio/game_mp3/cleric_heal.wav");
    effectMap[CLERIC_HEAL_SOUND] = clericHealEffect;

    // Rogue Sounds
    SoLoud::Wav* rogueAttackEffect =  new SoLoud::Wav();
    rogueAttackEffect->load("audio/game_mp3/rogue_attack.wav");
    effectMap[ROGUE_ATTACK_SOUND] = rogueAttackEffect;

    // Monster Sounds
    SoLoud::Wav* monsterThrowEffect =  new SoLoud::Wav();
    monsterThrowEffect->load("audio/game_mp3/monster_throw.wav");
    effectMap[MONSTER_THROW_SOUND] = monsterThrowEffect;
    SoLoud::Wav* monsterClawEffect =  new SoLoud::Wav();
    monsterClawEffect->load("audio/game_mp3/monster_claw.wav");
    effectMap[MONSTER_CLAW_SOUND] = monsterClawEffect;

    // Other Sounds
    SoLoud::Wav* selectEffect =  new SoLoud::Wav();
    selectEffect->load("audio/game_mp3/pop.wav");
    effectMap[SELECT_SOUND] = selectEffect;
    SoLoud::Wav* healthPickupEffect =  new SoLoud::Wav();
    healthPickupEffect->load("audio/game_mp3/heal_objective_taken.wav");
    effectMap[HEALTH_PICKUP_SOUND] = healthPickupEffect;
    SoLoud::Wav* damageTakenEffect =  new SoLoud::Wav();
    damageTakenEffect->load("audio/game_mp3/damage_effect.mp3");
    effectMap[DAMAGE_TAKEN_SOUND] = damageTakenEffect;
}


// Play audio without looping (good for effects)
void AudioProgram::playAudioWithoutLooping(Sound sound) {
    if(musicMap.find(sound) != musicMap.end()) {
        SoLoud::WavStream* music = musicMap[sound];
        music->setLooping(false);
        musicHandles.push_back(soLoudEngine.play(*music, musicVolume));
    } else if (effectMap.find(sound) != effectMap.end()) {
        SoLoud::Wav* effect = effectMap[sound];
        effect->setLooping(false);
        soLoudEngine.play(*effect, effectsVolume);
    }
}

// Play audio with looping (good for background music)
void AudioProgram::playAudioWithLooping(Sound sound) {
    if(musicMap.find(sound) != musicMap.end()) {
        SoLoud::WavStream* music = musicMap[sound];
        music->setLooping(true);
        musicHandles.push_back(soLoudEngine.play(*music, musicVolume));
    } else if (effectMap.find(sound) != effectMap.end()) {
        SoLoud::Wav* effect = effectMap[sound];
        effect->setLooping(true);
        soLoudEngine.play(*effect, effectsVolume);
    }
}

// Stop the audio.
void AudioProgram::stopAudio(Sound sound) {
    if(musicMap.find(sound) != musicMap.end()) {
        musicMap[sound]->stop();
    } else if (effectMap.find(sound) != effectMap.end()) {
        effectMap[sound]->stop();
    }
}

void AudioProgram::stopAllAudio() {
    soLoudEngine.stopAll();
}

// sets volume on the specific effect/music
void AudioProgram::setSpecificVolume(Sound sound, float volume) {
    if(musicMap.find(sound) != musicMap.end()) {
        musicMap[sound]->setVolume(volume);
    } else if (effectMap.find(sound) != effectMap.end()) {
        effectMap[sound]->setVolume(volume);
    }
}

// set volume on all the sound in the game.
void AudioProgram::setAllVolume(float volume) {
    setEffectsVolume(volume);
    setMusicVolume(volume);
}

// set effects volume specifically
void AudioProgram::setEffectsVolume(float volume) {
    for(auto i = effectMap.begin(); i != effectMap.end(); i++) {
        i->second->setVolume(volume);
    }

    effectsVolume = volume;

}

// set music volume specifically
void AudioProgram::setMusicVolume(float volume) {
    for(auto i = musicMap.begin(); i != musicMap.end(); i++) {
        i->second->setVolume(volume);
    }

    for(auto i = musicHandles.begin(); i != musicHandles.end(); i++) {
        soLoudEngine.setVolume(*i, volume);
    }
    musicVolume = volume;
}

// gets the length of the sound in seconds
float AudioProgram::getSoundLength(Sound sound) {
    if(musicMap.find(sound) != musicMap.end()) {
        return musicMap[sound]->getLength();
    } else if (effectMap.find(sound) != effectMap.end()) {
        return effectMap[sound]->getLength();
    }
}

void AudioProgram::playDirectionalEffect(Sound sound, AudioPosition center, AudioPosition audio) {

    // negative = pan to the left, positive pan to the right
    float pan = (audio.x - center.x) / MAX_HEARING_DISTANCE;

    // Volume = effectsVolume * (MAX_HEARING_DISTANCE - absolute distance from player) / MAX_HEARING_DISTANCE
    float volume =  (MAX_HEARING_DISTANCE - min(sqrt(abs(audio.x - center.x) + abs(audio.y - center.y)), MAX_HEARING_DISTANCE)) / MAX_HEARING_DISTANCE * effectsVolume;
    if (effectMap.find(sound) != effectMap.end()) {
        SoLoud::Wav* effect = effectMap[sound];
        effect->setLooping(false);
        soLoudEngine.play(*effect, volume, pan);
    }
}

void AudioProgram::toggleMute() {
    printf("Muted: %d\n", isMuted);
    if(isMuted) {
        setMusicVolume(DEFAULT_MUSIC_VOLUME);
        setEffectsVolume(DEFAULT_EFFECTS_VOLUME);
        isMuted = false;
    } else {
        setMusicVolume(0.f);
        setEffectsVolume(0.f);
        isMuted = true;
    }
}