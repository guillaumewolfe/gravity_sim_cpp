#include "engine/RenderTools/Render.h"
#include "states/SimulationState.h"


Render::Render(SimulationState& simulation)
: simulationState(simulation) // Initialisation de la référence
{
    std::string nom = simulationState.getDescription();
    setBackground();
}


void Render::Draw(){


}

void Render::setBackground(){
    cv::Mat image = cv::imread("../assets/textures/background.jpg");
    if (image.empty()) {
        std::cerr << "Failed to load texture" << std::endl;
        return;
    }

}