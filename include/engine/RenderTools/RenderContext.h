#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <vector>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include "UI/ImageButton.h"
#include "engine/EngineIncludes.h"
//#include "engine/RenderTools/notificationTool.h"
#include <map>
#include <chrono>

class SimulationState;
class SoundTool;
class NotificationTool;

class RenderContext {
public: 

    RenderContext(float* time, float* tmul ,Camera* currentCam,std::vector<Labbel*>& lbls, std::vector<Button*>& btns,std::vector<ImageButton*>& imgbtns, float* maxsize, bool* showaxe,
    SystemeSolaire* sysSol, int* speedIndice,std::vector<std::pair<int, std::string>> speedList, bool* iscreating, bool* showInfo, bool* showCameraOptions, bool* isLive, bool* showOptions, bool* showSettings, bool* isOrbiting, bool* isPaused, SoundTool* soundTool);
    
    float* simulationTime = nullptr;
    float* timeMultiplier = nullptr;
    float* maxSize = nullptr;
    bool* showAxes = nullptr;
    bool* isCreating = nullptr;
    bool* showInfo = nullptr;
    int* currentSpeedIndex = nullptr;
    std::vector<std::pair<int, std::string>> speedSettings;
    std::map<std::string, ImVec4> typeDictColor;
    std::vector<Labbel*> labels;
    std::vector<Button*> buttons;
    std::vector<ImageButton*> imageButtons;
    Camera* currentCamera;
    SystemeSolaire* systemeSolaire;
    bool* showCameraOptions = nullptr;
    bool* isLive = nullptr;
    bool* showOptions = nullptr;
    bool* showSettings = nullptr;
    bool* isOrbiting = nullptr;
    bool* isPaused = nullptr;
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
    bool isChangingParameters = false;
    bool showNotificationTool = false;
    bool showRestartTool = false;
    SoundTool* soundTool = nullptr;
    NotificationTool* NotificationTool = nullptr;
    bool debug = false;
    bool showQuiz = false;
    bool showSaveSimulation = false;
    bool isCollidingQuest = false;
    ImVec4 getTypeColor(const std::string& type);
    void setCurrentTime(std::chrono::system_clock::time_point time);
    std::chrono::system_clock::time_point getCurrentTime();
    std::string getDateTime();
    std::chrono::system_clock::time_point currentDateTime;



    std::map<int, std::pair<std::string, ImVec4>> colorByTypeDict;

};

#endif // RENDERCONTEXT_H