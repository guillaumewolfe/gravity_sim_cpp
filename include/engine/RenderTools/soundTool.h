// SoundTool.h
#ifndef SoundTool_H
#define SoundTool_H

#include <iostream>
#include <map>

class Mix_Chunk;
class GameSettings;
class SoundTool{
public:
    SoundTool(GameSettings* settings);
    ~SoundTool();

    void playSound(const std::string& soundName);
    void changeButtonVolume(float volume);
    void updateVolume();
    void test();
    float currentMusicVolume = 1.0f;

private:

    void initAllSounds();
    std::map<std::string, Mix_Chunk*> soundMap = std::map<std::string, Mix_Chunk*>();
    GameSettings* settings = nullptr; 
    bool isDestroyed = false;


};

#endif // SoundTool_H