#include <states/BaseState.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "UI/Buttons.h"
#include <vector>
#include <thread>
#include <future>


#ifndef GAME_H
#define GAME_H
struct GameSettings {
    // Paramètres graphiques
    int resolutionX;
    int resolutionY;
    bool fullscreen;
    bool vsync;
    bool antiAliasing;
    int textureQuality;
    bool highQuality;

    float movementSpeed;
    float rotationSpeed;
    // Paramètres audio
    float mainVolume;
    float musicVolume;
    float sfxVolume;
    bool volumeChanged;

    // Autres paramètres...
};



class Game {
private:
    BaseState* currentState; 
    GLFWwindow* window;
    std::vector<Button*> buttons;
    bool shouldClose = false;
    GameSettings settings;

    BaseState* requestedState;
    bool changeStateRequested;
public:
    Game();
    ~Game();

    std::unordered_map<std::string, BaseState*> states;


    bool ShouldClose() const; 
    void ApplyStateChange();
    void setShouldClose(bool value);
    void getState();
    GameSettings* getSettings();
    void InitSettings();

    void Init();
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
    GLFWwindow* loadingWindow;
    void initLoadingWindow();

    ImGuiContext* mainContext = ImGui::CreateContext();
    ImGuiContext* loadingContext = ImGui::CreateContext();

};

#endif
