#define GL_SILENCE_DEPRECATION
#include "engine/RenderTools/steamTool.h"
#include "steam_api.h"
#include "engine/RenderTools/SuccessTool.h"


SteamTool::SteamTool(RenderContext* renderContext) : RenderComponent(renderContext){
    if (SteamAPI_Init()) {
        // Steam API has been successfully initialized
        isConnected = true;
    } else {
        // Initialization failed, perhaps Steam is not running
        isConnected = false;
    }
    lastUpdateTime = std::chrono::steady_clock::now();
    //Sync success with steam
}

// Desctructeur
SteamTool::~SteamTool() {
    SteamAPI_Shutdown();
}

void SteamTool::Draw() {
    updatePlayTime();
}

void SteamTool::updatePlayTime(){
        if (!isConnected) return;
    

        auto now = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - lastUpdateTime).count();
        if (elapsedSeconds >= updateInterval) {
            // Mise à jour du temps de jeu ici
            int totalPlayTime = 0;
            SteamUserStats()->GetStat("TotalPlayTime", &totalPlayTime);
            totalPlayTime += elapsedSeconds; // Ou une autre valeur représentant le temps passé depuis la dernière mise à jour
            SteamUserStats()->SetStat("TotalPlayTime", totalPlayTime);
            SteamUserStats()->StoreStats();

            // Réinitialisez lastUpdateTime pour le prochain intervalle
            lastUpdateTime = now;
        }
}

void SteamTool::getTotalPlayTime(){
    //Print the stat of the total play time of the game
    if(!isConnected) return;
    int totalPlayTime = 0;
    SteamUserStats()->GetStat("TotalPlayTime", &totalPlayTime);
    std::cout << "Total play time: " << totalPlayTime << " minutes" << std::endl;

}



std::string SteamTool::getUserName(){
    if(!isConnected) return std::string("");
    const char* userNameCStr = SteamFriends()->GetPersonaName();
    std::string userName(userNameCStr);
    return userName;
}


void SteamTool::newSuccess(int id){
    if(!isConnected) return;

    std::string achievementName = "SQ_QUEST_" + std::to_string(id);
    bool bAchieved = false;

    // Vérifier si le succès est déjà débloqué
    if (SteamUserStats()->GetAchievement(achievementName.c_str(), &bAchieved)) {
        // Si l'appel réussit et bAchieved est false, le succès n'est pas encore débloqué
        if (!bAchieved) {
            SteamUserStats()->SetAchievement(achievementName.c_str());
            SteamUserStats()->StoreStats();
            std::cout << "Succès débloqué : " << achievementName << std::endl;
        }
    } else {
        // L'appel à GetAchievement a échoué
        std::cout << "Impossible de vérifier l'état du succès : " << achievementName << std::endl;
    }
}


void SteamTool::removeSuccess(int id){
    if(!isConnected) return;
    std::string achievementName = "SQ_QUEST_"+std::to_string(id);
    SteamUserStats()->ClearAchievement(achievementName.c_str());
    SteamUserStats()->StoreStats();
}

void SteamTool::syncSuccess(std::vector<Success *> successList){
    if(!isConnected) return;
    for(auto& success : successList){
        std::string achievementName = "SQ_QUEST_"+std::to_string(success->id);
        bool bAchieved = false;
        if (SteamUserStats()->GetAchievement(achievementName.c_str(), &bAchieved)) {
            if (bAchieved && success->isDone == false) {
                success->isDone = true;
                
            }
        }
    }
    
}
