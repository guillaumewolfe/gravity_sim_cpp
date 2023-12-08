#include "states/BaseState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef LOADING_H
#define LOADING_H
class LoadingState : public BaseState {
public:
    LoadingState(Game* gameObj);
    void Enter() override;
    void Update() override;
    void UpdatePhysics(double dt) override;
    void Draw() override;
    void Exit() override;
    void RestartState() override;
    std::string getDescription() override;

private:
    ImFont* customFont;
    void drawLoadingrectangle();
    int winWidth, winHeight;
    ImDrawList* drawList;
    // Autres méthodes et attributs spécifiques au menu
    float progress = 0.0f;
};
#endif