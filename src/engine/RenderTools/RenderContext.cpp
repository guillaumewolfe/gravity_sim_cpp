#include "engine/RenderTools/RenderContext.h"
#include <SDL_mixer.h>
#include "engine/RenderTools/soundTool.h"
#include "engine/RenderTools/notificationTool.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

RenderContext::RenderContext(float* time,float* tmul ,Camera* currentCam,std::vector<Labbel*>& lbls, 
std::vector<Button*>& btns,std::vector<ImageButton*>& imgbtns, float* maxsize, bool* showaxe, SystemeSolaire* sysSol, int* speedIndice, std::vector<std::pair<int, std::string>> speedList,bool* iscreating, bool* showInfo, bool* showCameraOptions,bool* isLive, bool* showOptions, bool* showSettings, bool* isOrbiting, bool* isPaused,SoundTool* soundTool)
    : simulationTime(time),timeMultiplier(tmul) ,currentCamera(currentCam),
    labels(lbls), buttons(btns),imageButtons(imgbtns), maxSize(maxsize), showAxes(showaxe), systemeSolaire(sysSol), speedSettings(speedList), currentSpeedIndex(speedIndice),isCreating(iscreating),showInfo(showInfo), showCameraOptions(showCameraOptions), isLive(isLive), showOptions(showOptions), showSettings(showSettings), isOrbiting(isOrbiting), isPaused(isPaused), soundTool(soundTool) {
    
    
    colorByTypeDict[0] = std::make_pair("Black Hole", ImVec4(60, 60, 60, 1));
    colorByTypeDict[1] = std::make_pair("Star", ImVec4(250,250,120, 1)); // Jaune pour une étoile par exemple
    colorByTypeDict[2] = std::make_pair("Terrestrial Planet", ImVec4(200, 255, 200, 1));
    colorByTypeDict[3] = std::make_pair("Gas Giant", ImVec4(204, 153, 204, 1)); // Bleu pour une planète
    colorByTypeDict[4] = std::make_pair("Ice Giant", ImVec4(0, 103, 204, 1)); // Bleu pour une planète
    colorByTypeDict[5] = std::make_pair("Natural satellite", ImVec4(200, 200, 200, 1)); // Gris pour un satellite naturel


    typeDictColor["BlackHole"] = ImVec4(0, 0, 0, 1);
    typeDictColor["Sun"] = ImVec4(250, 250, 120, 1); // Jaune pour une étoile par exemple
    typeDictColor["Mercury"] = ImVec4(100, 100, 100, 1);
    typeDictColor["Venus"] = ImVec4(238, 147, 17, 1); // Orange pour Venus
    typeDictColor["Earth"] = ImVec4(50, 100, 255, 1); // Bleu pour la Terre
    typeDictColor["Mars"] = ImVec4(197, 84, 62, 1); // Rougeâtre pour Mars
    typeDictColor["Jupiter"] = ImVec4(234, 206, 180, 1);
    typeDictColor["Saturn"] = ImVec4(217, 186, 140, 1); // Jaune pour Saturne
    typeDictColor["Uranus"] = ImVec4(155, 221, 227, 1);
    typeDictColor["Neptune"] = ImVec4(39, 114, 210, 1); // Bleu pour Neptune
    typeDictColor["Ice"] = ImVec4(122, 137, 150, 1); // Bleu clair pour les objets glacés
    typeDictColor["Rock"] = ImVec4(137, 80, 77, 1); // Gris pour les objets rocheux
    typeDictColor["Volcanic"] = ImVec4(62, 95, 86, 1); // Gris foncé pour les objets volcaniques
    typeDictColor["Moon"] = ImVec4(200, 200, 200, 1); // Gris pour les lunes
}


ImVec4 RenderContext::getTypeColor(const std::string& type) {
    if (typeDictColor.find(type) != typeDictColor.end()) {
        return typeDictColor[type];
    }
    else {
        return ImVec4(255, 255, 255, 1);
    }
}

void RenderContext::setCurrentTime(std::chrono::system_clock::time_point time) {
    currentDateTime = time;
}

std::chrono::system_clock::time_point RenderContext::getCurrentTime() {
    return currentDateTime;
}

    std::string RenderContext::getDateTime() {
        auto now = std::chrono::system_clock::to_time_t(currentDateTime);
        std::tm *ptm = std::localtime(&now);
        std::stringstream ss;
        ss << std::put_time(ptm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }