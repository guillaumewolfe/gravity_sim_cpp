#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <UI/Buttons.h>
#include <UI/Labbel.h>
#include "opencv2/opencv.hpp"

#ifndef BASESTATE_H
#define BASESTATE_H

class Game; 

class BaseState {
protected:

    Game* gameObj;
    GLFWwindow* window;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;

public:
    BaseState(Game* gameObjs);
    virtual ~BaseState() {}
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void UpdatePhysics(double dt) = 0;
    virtual void Draw() = 0;
    virtual void Exit() = 0;
    virtual std::string getDescription() = 0;
    virtual void RestartState() = 0;
};
#endif