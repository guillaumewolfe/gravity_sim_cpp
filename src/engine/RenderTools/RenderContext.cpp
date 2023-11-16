#include "engine/RenderTools/RenderContext.h"

RenderContext::RenderContext(float* time,float* tmul ,Camera* currentCam,std::vector<Labbel*>& lbls, 
std::vector<Button*>& btns, float* maxsize, bool* showaxe, SystemeSolaire* sysSol, int* speedIndice, std::vector<std::pair<int, std::string>> speedList )
    : simulationTime(time),timeMultiplier(tmul) ,currentCamera(currentCam),
    labels(lbls), buttons(btns), maxSize(maxsize), showAxes(showaxe), systemeSolaire(sysSol), speedSettings(speedList), currentSpeedIndex(speedIndice) {
}
