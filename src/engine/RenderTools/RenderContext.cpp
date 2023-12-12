#include "engine/RenderTools/RenderContext.h"

RenderContext::RenderContext(float* time,float* tmul ,Camera* currentCam,std::vector<Labbel*>& lbls, 
std::vector<Button*>& btns,std::vector<ImageButton*>& imgbtns, float* maxsize, bool* showaxe, SystemeSolaire* sysSol, int* speedIndice, std::vector<std::pair<int, std::string>> speedList,bool* iscreating, bool* showInfo, bool* showCameraOptions,bool* isLive, bool* showOptions, bool* showSettings )
    : simulationTime(time),timeMultiplier(tmul) ,currentCamera(currentCam),
    labels(lbls), buttons(btns),imageButtons(imgbtns), maxSize(maxsize), showAxes(showaxe), systemeSolaire(sysSol), speedSettings(speedList), currentSpeedIndex(speedIndice),isCreating(iscreating),showInfo(showInfo), showCameraOptions(showCameraOptions), isLive(isLive), showOptions(showOptions), showSettings(showSettings) {
    
    
    
    
    colorByTypeDict[0] = std::make_pair("Black Hole", ImVec4(60, 60, 60, 1));
    colorByTypeDict[1] = std::make_pair("Star", ImVec4(250,237,97, 1)); // Jaune pour une étoile par exemple
    colorByTypeDict[2] = std::make_pair("Terrestrial Planet", ImVec4(200, 255, 200, 1));
    colorByTypeDict[3] = std::make_pair("Gas Giant", ImVec4(204, 153, 204, 1)); // Bleu pour une planète
    colorByTypeDict[4] = std::make_pair("Ice Giant", ImVec4(0, 103, 204, 1)); // Bleu pour une planète
    colorByTypeDict[5] = std::make_pair("Natural satellite", ImVec4(200, 200, 200, 1)); // Gris pour un satellite naturel

}
