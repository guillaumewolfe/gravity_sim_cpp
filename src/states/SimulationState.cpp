#include "states/SimulationState.h"


SimulationState::SimulationState(Game* gameObj) : BaseState(gameObj) {}

void SimulationState::Enter() {
    std::cout << "Entering Simulation State" << std::endl;
}

void SimulationState::Update() {
}
void SimulationState::UpdatePhysics(int dt){};

void SimulationState::Draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void SimulationState::Exit() {
    std::cout << "Exiting Simulation State" << std::endl;
}

std::string SimulationState::getDescription() {
    return "Simulation State";
}
