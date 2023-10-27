#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifndef BASESTATE_H
#define BASESTATE_H


class BaseState {
protected:
    GLFWwindow* window;

public:
    BaseState(GLFWwindow* window);
    virtual ~BaseState() {}
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Exit() = 0;
    virtual std::string getDescription() = 0;
};
#endif