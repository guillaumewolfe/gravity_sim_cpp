#include "states/MenuState.h"


MenuState::MenuState(GLFWwindow* window) : BaseState(window) {}
void MenuState::Enter() {
    std::cout << "Entering Menu State" << std::endl;
}

void MenuState::Update() {
}


void MenuState::Draw() {
}


void MenuState::Exit() {
    std::cout << "Exiting Menu State" << std::endl;
}

std::string MenuState::getDescription() {
    return "Menu State";
}
