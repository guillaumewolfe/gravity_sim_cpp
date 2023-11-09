
#ifndef RENDER_H
#define RENDER_H

class SimulationState;

class Render {
private:

    SimulationState& simulationState;
    void setBackground();


//Background:

public:

Render(SimulationState& simulation);
void Draw();
};


#endif