#include "states/BaseState.h"
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include "game/game.h"
#include <SDL_mixer.h>
#include "UI/MessageTool.h"
#include "engine/RenderTools/settingsTool.h"

#ifndef MENUSTATE_H
#define MENUSTATE_H
class MenuState : public BaseState {

private:
    // For video:
    cv::VideoCapture cap;
    GLuint videoTexture; 
    bool videoInitialized = false;
    bool newFrameReady = false;
    double elapsedTime;
    bool showSettings = false;

    MessageTool* messageBox;
    SettingsTool* settingsTool;

    //Music
    Mix_Music* bgMusic;

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
    

    //UI
    void generateDialogBox(std::function<void()> func, const std::string& message);

    //Video:
    void generateVideo();
    void generateMusic();
    void UpdateVideo();
    void drawVideo();

    double lastTime;
    double accumulator = 0.0;
    float fadeOpacity = 1.0f;
    bool musicStarted = false;
    SoundTool* soundTool;
    int winWidth, winHeight;





    // Autres méthodes et attributs spécifiques au menu
};
#endif