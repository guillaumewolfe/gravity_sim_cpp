#include "states/BaseState.h"


#ifndef SIMULATIONSTATE_H
#define LSIMULATIONSTATE_H
class SimulationState: public BaseState {
public:

    void Enter() override;
    void Update() override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;

    // Autres méthodes et attributs spécifiques au menu
};
#endif