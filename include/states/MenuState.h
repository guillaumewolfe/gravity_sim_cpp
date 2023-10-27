#include "states/BaseState.h"


#ifndef MENUSTATE_H
#define MENUSTATE_H
class MenuState : public BaseState {
public:
    MenuState(GLFWwindow* window);
    void Enter() override;
    void Update() override;
    void UpdatePhysics(int dt) override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;

    // Autres méthodes et attributs spécifiques au menu
};
#endif