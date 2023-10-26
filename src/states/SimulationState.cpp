#include "states/SimulationState.h"

void SimulationState::Enter() {
    std::cout << "Entering Simulation State" << std::endl;
}

void SimulationState::Update() {
    std::cout << "Updating Simulation State" << std::endl;
}


void SimulationState::Draw() {
    std::cout << "Drawing Simulation State" << std::endl;
}


void SimulationState::Exit() {
    std::cout << "Exiting Simulation State" << std::endl;
}

std::string SimulationState::getDescription() {
    return "Simulation State";
}
