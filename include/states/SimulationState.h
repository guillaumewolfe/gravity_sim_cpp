#include "states/BaseState.h"
#include <FTGL/ftgl.h>


#ifndef SIMULATIONSTATE_H
#define LSIMULATIONSTATE_H
class SimulationState: public BaseState {
public:

    SimulationState(GLFWwindow* window);
    void Enter() override;
    void Update() override;
    void Draw() override;
    void Exit() override;
    std::string getDescription() override;

private:
    FTFont* font;
};
#endif