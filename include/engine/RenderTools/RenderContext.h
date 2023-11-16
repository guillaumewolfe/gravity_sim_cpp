#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <vector>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include "engine/EngineIncludes.h"


class RenderContext {
public: 
    float* simulationTime;
    float* timeMultiplier;
    float* maxSize;
    bool* showAxes;
    int* currentSpeedIndex;
    std::vector<std::pair<int, std::string>> speedSettings;
    std::vector<Labbel*> labels;
    std::vector<Button*> buttons;
    Camera* currentCamera;
    SystemeSolaire* systemeSolaire;


    RenderContext(float* time, float* tmul ,Camera* currentCam,std::vector<Labbel*>& lbls, std::vector<Button*>& btns, float* maxsize, bool* showaxe, 
    SystemeSolaire* sysSol, int* speedIndice,std::vector<std::pair<int, std::string>> speedList );
};

#endif // RENDERCONTEXT_H