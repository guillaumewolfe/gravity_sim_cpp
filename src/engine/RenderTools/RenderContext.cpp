#include "engine/RenderTools/RenderContext.h"

RenderContext::RenderContext(float* time,float* tmul ,std::vector<Labbel*>& lbls, std::vector<Button*>& btns)
    : simulationTime(time),timeMultiplier(tmul) ,labels(lbls), buttons(btns) {
}