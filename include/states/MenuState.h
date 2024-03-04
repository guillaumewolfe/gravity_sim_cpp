#include "states/BaseState.h"
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include "game/game.h"
#include <SDL_mixer.h>
#include "UI/MessageTool.h"
#include "engine/RenderTools/settingsTool.h"



#ifndef MENUSTATE_H
#define MENUSTATE_H
class WelcomeTool;
class MenuState : public BaseState {

private:
    // For video:
    cv::VideoCapture cap = cv::VideoCapture(0);
    GLuint videoTexture = 0; 
    bool videoInitialized = false;
    bool newFrameReady = false;
    double elapsedTime = 0.0;
    bool showSettings = false;

    MessageTool* messageBox = nullptr;
    SettingsTool* settingsTool = nullptr;
    WelcomeTool* welcomeTool = nullptr;

    //Music
    Mix_Music* bgMusic = nullptr;

    //UI Elements
    std::vector<Button*> generateButtons();
    std::vector<Labbel*> generateLabbels();        
public:
    MenuState(Game* gameObj);
    void Enter() override;
    void Update() override;
    void UpdatePhysics(double dt) override;
    void Draw() override;
    void drawUiElements();
    void closeButton();
    void startButton();
    void settingsButton();
    void RestartState() override;
    void Exit() override;
    std::string getDescription() override;
    void showWelcome();
    

    //UI
    void generateDialogBox(std::function<void()> func, const std::string& message);

    //Video:
    void generateVideo();
    void generateMusic();
    void UpdateVideo();
    void drawVideo();

    double lastTime = 0.0;
    double accumulator = 0.0;
    float fadeOpacity = 1.0f;
    bool musicStarted = false;
    SoundTool* soundTool = nullptr;
    int winWidth, winHeight = 0;

    bool drawWelcomeMessage = true;
    void drawWelcome();





    // Autres méthodes et attributs spécifiques au menu
};
#endif