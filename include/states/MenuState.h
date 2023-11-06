#include "states/BaseState.h"
#include "UI/Buttons.h"
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

public:
    MenuState(Game* gameObj);
    void Enter() override;
    std::vector<Button*> generateButtons();
    void Update() override;
    void UpdatePhysics(int dt) override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;

    //Video:
    void generateVideo();
    void UpdateVideo();
    void drawVideo();


    // Autres méthodes et attributs spécifiques au menu
};
#endif