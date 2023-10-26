#include "states/MenuState.h"

void MenuState::Enter() {
    std::cout << "Entering Menu State" << std::endl;
}

void MenuState::Update() {
    std::cout << "Updating Menu State" << std::endl;
}


void MenuState::Draw() {
    std::cout << "Drawing Menu State" << std::endl;
}


void MenuState::Exit() {
    std::cout << "Exiting Menu State" << std::endl;
}

std::string MenuState::getDescription() {
    return "Menu State";
}
