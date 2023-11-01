#include "states/BaseState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef SIMULATION_H
#define SIMULATION_H
class SimulationState : public BaseState {
public:
   SimulationState(Game* gameObj);
    void Enter() override;
    void Update() override;
    void UpdatePhysics(int dt) override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;

    // Autres méthodes et attributs spécifiques au menu
};
#endif