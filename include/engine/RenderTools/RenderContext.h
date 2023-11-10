#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <vector>
#include "UI/Buttons.h"
#include "UI/Labbel.h"


class RenderContext {
public:
    float* simulationTime;
    float* timeMultiplier;
    std::vector<Labbel*> labels;
    std::vector<Button*> buttons;

    RenderContext(float* time, float* tmul ,std::vector<Labbel*>& lbls, std::vector<Button*>& btns);
};

#endif // RENDERCONTEXT_H