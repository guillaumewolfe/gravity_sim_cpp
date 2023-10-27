#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/EngineIncludes.h"
#include "states/statesIncludes.h"
#include <game/game.h>
#include "opencv2/opencv.hpp"

using namespace std; 



int main() {
    cout << CV_VERSION << endl;
    return 0;
}




/* GAME LOOPP
    //Time accumulator for physical updates:
    double t = 0.0;
    const double dt = 1/60.0;
    double currentTime = glfwGetTime();
    double accumulator = 0.0;
    
    Game myGame;

    if (!myGame.InitOpenGL()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    myGame.ChangeState(new LoadingState(myGame.getWindow()));
    myGame.ChangeState(new SimulationState(myGame.getWindow()));


    while (!glfwWindowShouldClose(myGame.getWindow())) {

        //Time for physical update
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        //Loop for Physical udate
        while (accumulator>= dt){
            myGame.UpdatePhysics(dt);
            t += dt;
            accumulator-=dt;}


        //Update + Drawing
        myGame.Update();
        myGame.Draw();


        //OpenGL refresh window
        glfwSwapBuffers(myGame.getWindow());
        glfwPollEvents();
    }

*/