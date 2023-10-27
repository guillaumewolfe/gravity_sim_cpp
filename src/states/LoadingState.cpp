#include "states/LoadingState.h"


LoadingState::LoadingState(GLFWwindow* window) : BaseState(window) {}

void LoadingState::Enter() {
    std::cout << "Entering Loading State" << std::endl;
}

void LoadingState::Update() {
}


void LoadingState::Draw() {
}


void LoadingState::Exit() {
    std::cout << "Exiting Loading State" << std::endl;
}

std::string LoadingState::getDescription() {
    return "Loading State";
}
