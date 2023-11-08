#include "states/BaseState.h"
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include "game/game.h"

#ifndef MENUSTATE_H
#define MENUSTATE_H
class MenuState : public BaseState {

private:
    // For video:
    cv::VideoCapture cap;
    GLuint videoTexture; 
    bool videoInitialized = false;
    bool newFrameReady = false;
    std::vector<Button*> generateButtons();
    std::vector<Labbel*> generateLabbels();    
    
public:
    MenuState(Game* gameObj);
    void Enter() override;
    void Update() override;
    void UpdatePhysics(double dt) override;
    void Draw() override;
    void drawUiElements();
    void Exit() override;
    std::string getDescription() override;

    //Video:
    void generateVideo();
    void UpdateVideo();
    void drawVideo();


    // Autres méthodes et attributs spécifiques au menu
};
#endif