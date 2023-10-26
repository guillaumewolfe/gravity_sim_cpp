#include "states/BaseState.h"


#ifndef ENDSTATE_H
#define ENDSTATE_H
class EndState: public BaseState {
public:

    void Enter() override;
    void Update() override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;

    // Autres méthodes et attributs spécifiques au menu
};
#endif