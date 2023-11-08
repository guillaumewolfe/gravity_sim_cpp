#include "states/EndState.h"

EndState::EndState(Game* gameObj) : BaseState(gameObj) {}
void EndState::Enter() {
    std::cout << "Entering End State" << std::endl;
}

void EndState::Update() {
}

void EndState::UpdatePhysics(double dt){};

void EndState::Draw() {
}


void EndState::Exit() {
    std::cout << "Exiting End State" << std::endl;
}

std::string EndState::getDescription() {
    return "End State";
}
