#include <iostream>
#include <game/game.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>




Game::Game() : currentState(nullptr), window(nullptr) {}
Game::~Game() {
    if (currentState) {
        currentState->Exit();
        delete currentState;
    }
    CleanupOpenGL(); // Ajoutez cette ligne pour le nettoyage OpenGL.
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

bool Game::InitOpenGL() {
    if (!glfwInit()) {
        return false;
    }

    window = glfwCreateWindow(800, 600, "Your Game Title", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    
    // Initialisez GLAD ou toute autre extension loader si vous en utilisez un
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }

    return true;
}
GLFWwindow* Game::getWindow() { return window; }

void Game::CleanupOpenGL() {
    glfwDestroyWindow(window);
    glfwTerminate();
}