#include "engine/RenderTools/RenderContext.h"

RenderContext::RenderContext(float* time,float* tmul ,Camera* currentCam,std::vector<Labbel*>& lbls, std::vector<Button*>& btns)
    : simulationTime(time),timeMultiplier(tmul) ,currentCamera(currentCam),labels(lbls), buttons(btns) {
}