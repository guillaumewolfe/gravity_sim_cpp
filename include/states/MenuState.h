#include "states/BaseState.h"


#ifndef MENUSTATE_H
#define MENUSTATE_H
class MenuState : public BaseState {
public:

    void Enter() override;
    void Update() override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;

    // Autres méthodes et attributs spécifiques au menu
};
#endif