#include "states/SimulationState.h"


SimulationState::SimulationState(GLFWwindow* window) : BaseState(window) {}

void SimulationState::Enter() {
    std::cout << "Entering Simulation State" << std::endl;
}

void SimulationState::Update() {
}
void SimulationState::UpdatePhysics(int dt){};

void SimulationState::Draw() {
}


void SimulationState::Exit() {
    std::cout << "Exiting Simulation State" << std::endl;
}

std::string SimulationState::getDescription() {
    return "Simulation State";
}
