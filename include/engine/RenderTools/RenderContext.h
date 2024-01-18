#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <vector>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include "UI/ImageButton.h"
#include "engine/EngineIncludes.h"
#include <map>

class SimulationState;
class SoundTool;

class RenderContext {
public: 

    RenderContext(float* time, float* tmul ,Camera* currentCam,std::vector<Labbel*>& lbls, std::vector<Button*>& btns,std::vector<ImageButton*>& imgbtns, float* maxsize, bool* showaxe,
    SystemeSolaire* sysSol, int* speedIndice,std::vector<std::pair<int, std::string>> speedList, bool* iscreating, bool* showInfo, bool* showCameraOptions, bool* isLive, bool* showOptions, bool* showSettings, bool* isOrbiting, bool* isPaused, SoundTool* soundTool);
    
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
    std::vector<ImageButton*> imageButtons;
    Camera* currentCamera;
    SystemeSolaire* systemeSolaire;
    bool* showCameraOptions;
    bool* isLive;
    bool* showOptions;
    bool* showSettings;
    bool* isOrbiting;
    bool* isPaused;
    SimulationState* simulationState;
    bool showAllOrbits = false;
    bool showAllNames = true;
    bool showAllPaths = true;
    bool showZoom = false;
    bool showMinimap = false;
    bool showControls = true;
    bool showCollision = true;
    bool shouldClickOnNames = true;
    bool mouseIsOnMinimap = false;
    SoundTool* soundTool;






    std::map<int, std::pair<std::string, ImVec4>> colorByTypeDict;

};

#endif // RENDERCONTEXT_H