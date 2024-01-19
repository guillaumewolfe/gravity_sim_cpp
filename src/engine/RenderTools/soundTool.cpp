#include "engine/RenderTools/soundTool.h"
#include <SDL_mixer.h>
#include "game/game.h"
#include "path_util.h"


SoundTool::SoundTool(GameSettings * settings):settings(settings){
    initAllSounds();
}

SoundTool::~SoundTool(){
    for (const auto& pair : soundMap) {
        Mix_FreeChunk(pair.second);
    }
    Mix_CloseAudio();
}



void SoundTool::initAllSounds(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2 , 2048) == -1) {
        std::cerr << "SDL_mixer could not initialize audio: " << Mix_GetError() << std::endl;
        return ;
    }   
    Mix_AllocateChannels(16);
    soundMap["hover"] = Mix_LoadWAV(getFullPath("/assets/sounds/select.wav").c_str());
    soundMap["normal"] = Mix_LoadWAV(getFullPath("/assets/sounds/click.wav").c_str());
    soundMap["start"] = Mix_LoadWAV(getFullPath("/assets/sounds/startSim.wav").c_str());
    soundMap["creation"] = Mix_LoadWAV(getFullPath("/assets/sounds/confirm_creation.wav").c_str());
    soundMap["restart"] = Mix_LoadWAV(getFullPath("/assets/sounds/restart.mp3").c_str()); // Note: Mix_LoadWAV is typically used for WAV files
    
    //Planets
    soundMap["Mars"] = Mix_LoadWAV(getFullPath("/assets/sounds/planets/test.wav").c_str());
    // Check for loading errors
    for (const auto& pair : soundMap) {
        if (pair.second == nullptr) {
            std::cerr << "Failed to load sound " << pair.first << "! SDL_mixer Error: " << Mix_GetError() << std::endl;
            exit(1);
        }
    }

}

void SoundTool::changeButtonVolume(float volume){
    for (const auto& pair : soundMap) {
        if(pair.first == "hover"){
            Mix_VolumeChunk(pair.second, volume*MIX_MAX_VOLUME*0.25);
            continue;
        }
        Mix_VolumeChunk(pair.second, volume*MIX_MAX_VOLUME*0.75);
    }
}

void SoundTool::playSound(const std::string& soundName) {
    auto it = soundMap.find(soundName);
    if (it != soundMap.end()) {
        Mix_PlayChannel(-1, it->second, 0);
    } else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}

void SoundTool::updateVolume(){
    float volume = settings->mainVolume*settings->sfxVolume;
    changeButtonVolume(volume);
}

void SoundTool::test(){
    std::cout << "test" << std::endl;
}   