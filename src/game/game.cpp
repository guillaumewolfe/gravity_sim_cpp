#include <iostream>
#include <game/game.h>




Game::Game():currentState(nullptr){};
Game::~Game() {
    if (currentState) {
        currentState->Exit();
        delete currentState;
    }
}

void Game::Init(){};

void Game::Update(){
    if(currentState){
        currentState -> Update();
    }

};
void Game::Draw(){    if(currentState){
        currentState -> Draw();
    }};


void Game::ChangeState(BaseState* newState) {
    if (currentState) {
        currentState->Exit();
        delete currentState;
    }
    currentState = newState;
    if (currentState) {
        currentState->Enter();
    }
}
void Game::getState(){
    if(currentState){
        std::cout << currentState->getDescription() << std::endl;
    }
}