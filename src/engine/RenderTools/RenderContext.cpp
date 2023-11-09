#include "engine/RenderTools/RenderContext.h"

RenderContext::RenderContext(float* time, std::vector<Labbel*>& lbls, std::vector<Button*>& btns)
    : simulationTime(time), labels(lbls), buttons(btns) {
    // ...
}