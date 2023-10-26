#include <iostream>
#include "game/Game.h"



Game::Game(){
    currentState = LOADING;
};
Game::~Game(){};

void Game::Init(){};
void Game::Update(){};
void Game::Draw(){};
void Game::ChangeState(GameState newState){
    currentState = newState;
};
void Game::getState(){
    std::string currentStateString;
    switch (currentState)
    {
    case LOADING:
        currentStateString = "LOADING";
        break;
    case MENU:
        currentStateString = "MENU";
        break;
    case PLAYING:
        currentStateString = "PLAYING";
        break;
    case END:
        currentStateString = "END";
        break;
    }
    std::cout << currentStateString << std::endl;
}