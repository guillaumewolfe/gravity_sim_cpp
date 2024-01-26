#include <states/BaseState.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "UI/Buttons.h"
#include <vector>
#include <thread>
#include <future>
#include "engine/RenderTools/soundTool.h"

#ifndef GAME_H
#define GAME_H

class SoundTool;

struct GameSettings {
    // Paramètres graphiques
    int resolutionX = 1920;
    int resolutionY = 1080;
    bool fullscreen = false;
    bool vsync = true;
    bool antiAliasing = true;
    int textureQuality = 2;
    bool highQuality = true;

    float movementSpeed = 0.1f;
    float rotationSpeed = 0.1f;
    // Paramètres audio
    float mainVolume = 0.5f;
    float musicVolume = 0.5f;
    float sfxVolume = 0.5f;
    bool volumeChanged = false;

    SoundTool* soundTool = nullptr;



    // Autres paramètres...
};



class Game {
private:
    BaseState* currentState = nullptr; 
    GLFWwindow* window = nullptr;
    std::vector<Button*> buttons = std::vector<Button*>();
    bool shouldClose = false;
    GameSettings settings;;

    BaseState* requestedState = nullptr;
    bool changeStateRequested = false;
public:
    Game();
    ~Game();

    std::unordered_map<std::string, BaseState*> states = std::unordered_map<std::string, BaseState*>();


    bool ShouldClose() const; 
    void ApplyStateChange();
    void setShouldClose(bool value);
    void getState();
    GameSettings* getSettings();
    void InitSettings();

    void Init();
    void Load();
    ImFont* Create_font();
    void Update();
    void UpdatePhysics(double dt);
    void Draw();
    void Close();
    void ChangeState(const std::string& stateKey);
    bool InitOpenGL();
    GLFWwindow* getWindow();
    void CleanupOpenGL();
    void InitGameStates();
    bool loadingComplete = false;
    void ApplyFadeOut(GLFWwindow* loadingWindow);
    GLFWwindow* loadingWindow = nullptr;
    void initLoadingWindow();
    //getSoundTool
    SoundTool* getSoundTool();

    ImGuiContext* mainContext = ImGui::CreateContext();
    ImGuiContext* loadingContext = ImGui::CreateContext();


};

#endif
