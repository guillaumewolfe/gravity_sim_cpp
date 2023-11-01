#include "states/BaseState.h"
#include "UI/Buttons.h"
#include "game/game.h"

#ifndef MENUSTATE_H
#define MENUSTATE_H
class MenuState : public BaseState {

private:

    std::vector<Button*> menu_buttons;



public:
    MenuState(Game* gameObj);
    void Enter() override;
    std::vector<Button*> generateButtons();
    void Update() override;
    void UpdatePhysics(int dt) override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;


    // Autres méthodes et attributs spécifiques au menu
};
#endif