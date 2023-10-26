#include "states/LoadingState.h"

void LoadingState::Enter() {
    std::cout << "Entering Loading State" << std::endl;
}

void LoadingState::Update() {
    std::cout << "Updating Loading State" << std::endl;
}


void LoadingState::Draw() {
    std::cout << "Drawing Loading State" << std::endl;
}


void LoadingState::Exit() {
    std::cout << "Exiting Loading State" << std::endl;
}

std::string LoadingState::getDescription() {
    return "Loading State";
}
