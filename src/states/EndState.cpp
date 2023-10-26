#include "states/EndState.h"

void EndState::Enter() {
    std::cout << "Entering End State" << std::endl;
}

void EndState::Update() {
    std::cout << "Updating End State" << std::endl;
}


void EndState::Draw() {
    std::cout << "Drawing End State" << std::endl;
}


void EndState::Exit() {
    std::cout << "Exiting End State" << std::endl;
}

std::string EndState::getDescription() {
    return "End State";
}
