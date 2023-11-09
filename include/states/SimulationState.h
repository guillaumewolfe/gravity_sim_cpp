#include "states/BaseState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/EngineIncludes.h"
#include "engine/RenderTools/Render.h"
#include "states/MenuState.h"

#ifndef SIMULATION_H
#define SIMULATION_H
class SimulationState : public BaseState {
private:
    float simulation_time = 0.0f;
    float time_multiplier = 1.0f;
    
    //UI ELEMENTS

    std::vector<Button*> generateButtons();
    std::vector<Labbel*> generateLabbels();

    //RenderTool
    Render* render;

    //Simulation
    bool isPaused = false;

    RenderContext* renderContext;

public:
   SimulationState(Game* gameObj);
    void Enter() override;
    void Update() override;
    void UpdatePhysics(double dt) override;
    void Draw() override;
    void drawUiElements();
    void Exit() override;
    std::string getDescription() override;

    // Bouttons:
    void Pause();
    void Restart();
    void MenuButton();
};
#endif