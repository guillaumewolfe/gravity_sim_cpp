#include "states/BaseState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/EngineIncludes.h"
#include "engine/Physics/Physics.h"
#include "engine/RenderTools/Render.h"
#include "states/MenuState.h"
#include "UI/MessageTool.h"
#include "UI/ImageButton.h"


#ifndef SIMULATION_H
#define SIMULATION_H
class SimulationState : public BaseState {
private:
    float simulation_time = 0.0f;
    float time_multiplier = 1.0f;

    float previousMouseWheel = 0.0f;
    
    //UI ELEMENTS

    std::vector<Button*> generateButtons();
    std::vector<ImageButton*> generateImageButtons();
    std::vector<Labbel*> generateLabbels();
    std::vector<ImageButton*>  imageButtons;
    MessageTool* messageBox;
    SystemeSolaire* systemeSolaire;
    Physics* physics;
    void generateDialogBox(std::function<void()> func, const std::string& message);

    //RenderTool
    Render* render;

    //Simulation
    bool isPaused = false;
    bool forcePause = false;
    bool showAxes = false;
    bool isCreating = false;
    bool isLive = false;

    RenderContext* renderContext;

    Camera* currentCamera;

public:
    int currentSpeedIndex = 6;
    int followedObjectIndex = 0;
    std::vector<std::pair<int, std::string>> speedSettings = {

            {-604800, " -1 week/second"},
            {-86400*2, "-2 day/second"},
            {-86400, "-1 day/second"},
            {-86400/2, "-0.5 day/second"},
            {-3600, "-1 hour/second"},
            {-3600/2, "-0.5 hour/second"},
            {1, "Normal speed"},
            {3600/2, "0.5 hour/second"},
            {3600, "1 hour/second"},
            {86400/2, "0.5 day/second"},
            {86400, "1 day/second"},
            {86400*2, "2 day/second"},
            {604800 , "1 week/second"},
            // Ajoutez d'autres vitesses ici
        };





   SimulationState(Game* gameObj);
    void Enter() override;
    void Update() override;
    void UpdatePhysics(double dt) override;
    void Draw() override;
    void drawUiElements();
    void Exit() override;
    void RestartState() override;
    std::string getDescription() override;

    // Bouttons:
    void Pause();
    void Restart();
    void MenuButton();
    void ShowAxesButton();
    void ShowCameraOptionsButton();
    void CreateObjectButton();
    void SettingsButton();
    void MinimapButton();
    void OptionsButton();
    void TelescopeButton();
    void changeSimulationSpeed(bool increase);
    void changeFollowedObject();
    void resetView();
    void showInfos();
    bool showInfo=true;
    bool showMinimap;
    bool buttonsActivated = true;
    bool isOrbiting = true;
    bool showCameraOptions = false;
    bool showOptions = false;
    bool showSettings = false;
    bool isShowZoomClose = true;

    bool firstMouse = true;
    bool isRotatingWithMouse = false;
    bool isTranslating=false;
    ImVec2 lastMousePos = ImVec2(0, 0);
    void rotateCamWithMouse();

    void deactivateButtons();
    void activateButtons();
    void resetButtons();
    void checkButtonState();
};
#endif