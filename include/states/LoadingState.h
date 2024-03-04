#include "states/BaseState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <random>

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
    void setWindow(GLFWwindow* newWindow);

private:
    ImFont* customFont;
    void drawLoadingrectangle();
    int winWidth, winHeight;
    ImDrawList* drawList;
    // Autres méthodes et attributs spécifiques au menu
    float progress = 0.0f;
    void drawCircle(float screenPosX, float screenPosY, float radius,float lineWidth, float red, float green, float blue, float alpha);
    void drawFullCircle(float screenPosX, float screenPosY, float radius, float red, float green, float blue, float alpha);
    void drawSolarSystem();

    std::map<std::string, ImVec4> typeDictColor;
    std::vector<std::string> systemSolaire = {"Sun","Mercury","Venus","Earth","Mars","Jupiter","Saturn","Uranus","Neptune"};
    std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();
    void generate_colors();
    void initializePlanetAngles();
    std::vector<float> initialAngles;
    void drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, ImVec4 colorCenterDot);
    void drawSunEffect(ImVec2 planetPos, float radius);
    void generateRandomStars();
    void drawFixedStars();
    int numStars = 500;
    std::vector<std::pair<float, float>> stars;
    GLuint loadTexture(const char* relativeFilename);
    void drawImage(ImVec2 pos, ImVec2 size, GLuint textureID);
    void initImages();
    GLuint sunTexture;

};
#endif