#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/EngineIncludes.h"
#include "states/statesIncludes.h"
#include <game/game.h>

using namespace std; 



int main() {
    Game myGame;

    if (!myGame.InitOpenGL()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    myGame.ChangeState(new LoadingState(myGame.getWindow()));
    myGame.ChangeState(new SimulationState(myGame.getWindow()));
    // Boucle principale du jeu
    while (!glfwWindowShouldClose(myGame.getWindow())) {
        myGame.Update();
        myGame.Draw();

        glfwSwapBuffers(myGame.getWindow());
        glfwPollEvents();
    }

    return 0;
}