#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <vector>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include "engine/EngineIncludes.h"
#include <map>

class RenderContext {
public: 

    RenderContext(float* time, float* tmul ,Camera* currentCam,std::vector<Labbel*>& lbls, std::vector<Button*>& btns, float* maxsize, bool* showaxe,
    SystemeSolaire* sysSol, int* speedIndice,std::vector<std::pair<int, std::string>> speedList, bool* iscreating, bool* showInfo, bool* showCameraOptions, bool* isLive);
    
    float* simulationTime;
    float* timeMultiplier;
    float* maxSize;
    bool* showAxes;
    bool* isCreating;
    bool* showInfo;
    int* currentSpeedIndex;
    std::vector<std::pair<int, std::string>> speedSettings;
    std::vector<Labbel*> labels;
    std::vector<Button*> buttons;
    Camera* currentCamera;
    SystemeSolaire* systemeSolaire;
    bool* showCameraOptions;
    bool* isLive;



    std::map<int, std::pair<std::string, ImVec4>> colorByTypeDict;

};

#endif // RENDERCONTEXT_H