#include "states/BaseState.h"


#ifndef LOADING_H
#define LOADING_H
class LoadingState : public BaseState {
public:

    void Enter() override;
    void Update() override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;

    // Autres méthodes et attributs spécifiques au menu
};
#endif