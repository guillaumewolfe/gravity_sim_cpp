#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/EngineIncludes.h"
#include "states/statesIncludes.h"
#include <game/game.h>
#include "opencv2/opencv.hpp"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace std; 



int main() {
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
    myGame.Init();


    while (!myGame.ShouldClose() && !glfwWindowShouldClose(myGame.getWindow())) {

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
        myGame.Draw();
        myGame.Update();

        //OpenGL refresh window
        glfwSwapBuffers(myGame.getWindow());
        glfwPollEvents();
    }
}



