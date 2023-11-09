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
    bool shouldClose = false;

    //Fonts
    ImFont* myFont;
    std::map<std::string, ImFont*> fonts;
    void LoadFonts();

    BaseState* requestedState;
    bool changeStateRequested;
public:
    Game();
    ~Game();

    bool ShouldClose() const; 
    void ApplyStateChange();
    void setShouldClose(bool value);
    void getState();
    ImFont* getFont();
    ImFont* getFont(const std::string& fontKey);

    void Init();
    ImFont* Create_font();
    void Update();
    void UpdatePhysics(double dt);
    void Draw();
    void Close();
    void ChangeState(BaseState* newState);
    bool InitOpenGL();
    GLFWwindow* getWindow();
    void CleanupOpenGL();
};

#endif
