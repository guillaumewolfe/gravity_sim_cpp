#include "states/BaseState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef ENDSTATE_H
#define ENDSTATE_H
class EndState: public BaseState {
public:
    EndState(Game* gameObj);
    void Enter() override;
    void Update() override;
    void UpdatePhysics(double dt) override;
    void Draw() override;
    void Exit() override;
    void RestartState() override;
    std::string getDescription() override;

    // Autres méthodes et attributs spécifiques au menu
};
#endif