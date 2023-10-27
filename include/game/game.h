#include <states/BaseState.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef GAME_H
#define GAME_H

class Game {
private:
    BaseState* currentState; 
    GLFWwindow* window;

public:
    Game();
    ~Game();
    void getState();

    void Init();
    void Update();
    void UpdatePhysics(int dt);
    void Draw();
    void ChangeState(BaseState* newState);
    bool InitOpenGL();
    GLFWwindow* getWindow();
    void CleanupOpenGL();
};

#endif
