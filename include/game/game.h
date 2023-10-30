#include <states/BaseState.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "UI/Buttons.h"
#include <vector>

#ifndef GAME_H
#define GAME_H

class Game {
private:
    BaseState* currentState; 
    GLFWwindow* window;
    std::vector<Button*> buttons;
    ImFont* myFont;


public:
    Game();
    ~Game();
    void getState();

    void Init();
    ImFont* Create_font();
    void Update();
    void UpdatePhysics(int dt);
    void Draw();
    void Close();
    void ChangeState(BaseState* newState);
    bool InitOpenGL();
    GLFWwindow* getWindow();
    void CleanupOpenGL();
};

#endif
