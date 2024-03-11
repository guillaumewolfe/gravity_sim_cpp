#include "engine/RenderTools/SuccessTool.h"
#include "save/saveTool.h"
#include <chrono>
#include <ctime>
#include "engine/RenderTools/steamTool.h"

std::vector<Success*> SuccessTool::getSuccessList() {return successList;}

int SuccessTool::getSuccessCount() {return successList.size();}

void SuccessTool::setNotificationTool(NotificationTool* notificationTool) {this->notificationTool = notificationTool;}



void SuccessTool::checkSuccess() {
    for (auto success : successList) {
        if (!success->isDone && success->checkSuccess()) {
            success->stepsDone++;
            std::cout<<success->title<<" : "<<success->stepsDone<<"/"<<success->totalSteps<<std::endl;
            saveTool->SaveSuccessList(successList);
            if(success->id== 1){
                saveTool->saveElementsToVerify(&elementsToVerify, 0);
            }
        }

        if(!success->isDone && success->stepsDone >= success->totalSteps) {
            success->isDone = true;
            //notificationTool->Open("Success done!", "You have completed the Quest: " + success->title, "See", "Close", [this](){Open();});
            m_renderContext->steamTool->newSuccess(success->id);
        }
    }
}



void SuccessTool::initSuccess(){
    elementsToVerify = {"Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"}; //List 0
        auto exploreSolarSystenFunc = [&]() -> bool {
            if(m_renderContext->currentCamera->followedObject == nullptr) return false;
            if (m_renderContext->currentCamera->followedObject->isCreated == true) return false;
            if(m_renderContext->currentCamera->isGlobalFollowing == true) return false;
            //if(m_renderContext->currentCamera->followedObject->type == 1 or m_renderContext->currentCamera->followedObject->type == 0 or m_renderContext->currentCamera->followedObject->type == 5)return false;

            std::string currentObjectName = m_renderContext->currentCamera->followedObject->getDefaultName();
            auto it = std::find(elementsToVerify.begin(), elementsToVerify.end(), currentObjectName);

            if (it != elementsToVerify.end()) {
                elementsToVerify.erase(it);
                return true;
            }
            return false;
        };
        auto exploreMoon = [&]() -> bool {
            if(m_renderContext->currentCamera->followedObject == nullptr) return false;
            if (m_renderContext->currentCamera->followedObject->isCreated == true) return false;
            if(m_renderContext->currentCamera->isGlobalFollowing == true) return false;

            if(m_renderContext->currentCamera->followedObject->getDefaultName() == "Moon") {
                return true;
            }
            return false;
        };

        auto destroyEarth = [&]() -> bool {
            if (earthDestroyed) {
                return true;
            }
            return false;
        };

        auto addObject = [&]() -> bool {
            for(auto& object : m_renderContext->systemeSolaire->getObjects()){
                if(object->isCreated && *(m_renderContext->isCreating) == false){
                    return true;
                }
            }
            return false;
        };

        auto collision = [&]() -> bool {
            if (oneCollision) {
                return true;
            }
            return false;
        };

        auto starCollision = [&]() -> bool {
            if (twoStarsCollided) {
                twoStarsCollided = false;
                return true;
            }
            return false;
        };        

        auto hungryForMoreFunc = [&]() -> bool {
            if(m_renderContext->systemeSolaire->getObjects().size() == 1 && m_renderContext->systemeSolaire->getObjects()[0]->type == 0){
                return true;
            }
            return false;
        };

        auto moonWatching = [&]() -> bool {
            
            bool isFirstPerson = m_renderContext->currentCamera->firstPersonModeEnabled;
            CelestialObject* followedObject = m_renderContext->currentCamera->followedObject;
            CelestialObject* lookAtObject = m_renderContext->currentCamera->firstPersonTargetObject;

            if(!isFirstPerson){
                return false;
            }
            if(followedObject == nullptr or followedObject->getDefaultName() != "Earth"){
                return false;
            }
            if(lookAtObject == nullptr or lookAtObject->getDefaultName() != "Moon"){
                return false;
            }
            return true;

        };

        auto davidVsGoliath = [&]() -> bool {
            if (DavidVsGoliath) {
                return true;
            }
            return false;
        };


        auto quiz1DoneFunc = [&]() -> bool {
            if(quiz1Done){
                quiz1Done = false;
                return true;
            }
            return false;
        };

        auto quiz2DoneFunc = [&]() -> bool {
            if(quiz2Done){
                quiz2Done = false;
                return true;
            }
            return false;
        };

        auto quiz3DoneFunc = [&]() -> bool {
            if(quiz3Done){
                quiz3Done = false;
                return true;
            }
            return false;
        };

        auto quizDoneFunc = [&]() -> bool {
            if(quizDone){
                quizDone = false;
                return true;
            }
            return false;
        };

        auto simulationSavedFunc = [&]() -> bool {
            if(simulationSaved){
                simulationSaved = false;
                return true;
            }
            return false;
        };

        auto simulationLoadedFunc = [&]() -> bool {
            if(simulationLoaded){
                simulationLoaded = false;
                return true;
            }
            return false;
        };

        auto simulationSavedFuncMultiple = [&]() -> bool {
            if(simulationSavedMultiple){
                simulationSavedMultiple = false;
                return true;
            }
            return false;
        };

        auto simulationLoadedFuncMultiple = [&]() -> bool {
            if(simulationLoadedMultiple){
                simulationLoadedMultiple = false;
                return true;
            }
            return false;
        };

        auto timeTraveler = [&]() -> bool {
            // Convertir fromTime et toTime en time_t
            auto now = std::chrono::system_clock::now();
            auto oneYearLater = m_renderContext->getCurrentTime(); 
            std::time_t fromTimeT = std::chrono::system_clock::to_time_t(now);
            std::time_t toTimeT = std::chrono::system_clock::to_time_t(oneYearLater);
            
            // Convertir en std::tm pour une manipulation plus facile
            std::tm fromTm = *std::localtime(&fromTimeT);
            std::tm toTm = *std::localtime(&toTimeT);
            
            // Ajuster l'année de fromTm d'un an
            fromTm.tm_year += 1;
            
            // Normaliser les deux structures tm (important pour comparer les dates correctement)
            std::mktime(&fromTm);
            std::mktime(&toTm);
            
            // Comparer les composants de date (année, mois, jour)
            return (fromTm.tm_year == toTm.tm_year) && (fromTm.tm_mon == toTm.tm_mon) && (fromTm.tm_mday == toTm.tm_mday);
        };
        auto timeTraveler10 = [&]() -> bool {
            // Convertir fromTime et toTime en time_t
            auto now = std::chrono::system_clock::now();
            auto oneYearLater = m_renderContext->getCurrentTime(); 
            std::time_t fromTimeT = std::chrono::system_clock::to_time_t(now);
            std::time_t toTimeT = std::chrono::system_clock::to_time_t(oneYearLater);
            
            // Convertir en std::tm pour une manipulation plus facile
            std::tm fromTm = *std::localtime(&fromTimeT);
            std::tm toTm = *std::localtime(&toTimeT);
            
            // Ajuster l'année de fromTm d'un an
            fromTm.tm_year += 10;
            
            // Normaliser les deux structures tm (important pour comparer les dates correctement)
            std::mktime(&fromTm);
            std::mktime(&toTm);
            
            // Comparer les composants de date (année, mois, jour)
            return (fromTm.tm_year == toTm.tm_year) && (fromTm.tm_mon == toTm.tm_mon) && (fromTm.tm_mday == toTm.tm_mday);
        };

    auto firstSteps = [&]() -> bool {
        if(minimapTravel){
            minimapTravel = false;
            return true;
        }
        return false;
    };

    auto collisionCounter = [&]() -> bool {
        if(oneCollision){
            oneCollision = false;
            return true;
        }
        return false;
    };


    //Succès 1, explore tout le système solaire
    std::cout<<successList.size()+1<<std::endl;
    successList.push_back(new Success(successList.size()+1,m_renderContext, 8, "Explore Solar System","Explore every planet in our System and focus on it.", exploreSolarSystenFunc));//1
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Lunar Lookout","Find the moon", exploreMoon));//2
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Stellar Sculptor","Create a new object and custumize it", addObject));//3
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Small World","Use the minimap to travel to an object", firstSteps));//4
    successList.push_back(new Success(successList.size()+1,m_renderContext, 25, "Cosmic Clash","Cause 25 celestial collisions", collisionCounter));//5
    successList.push_back(new Success(successList.size()+1,m_renderContext, 10, "Star Wars","Cause a celestial collision between 2 stars 10 times", starCollision));//6
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "The Destroyer of Worlds","Destroy Earth", destroyEarth));//7
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Diner is served!","Create a Black Hole, and make it devour all objects around", hungryForMoreFunc));//8
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Moonwatch","Observe the Moon from Earth in First-Person View", moonWatching));//9
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "David vs Goliath","Throw a moon at Jupiter", davidVsGoliath));//10
    successList.push_back(new Success(successList.size()+1,m_renderContext, 5,"Good try", "Finish 5 quiz without a perfect result", quizDoneFunc));//11
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Beginner's Luck","Get a perfect result on the easiest difficulty", quiz1DoneFunc));//12
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Intermediate knowledge","Get a perfect result on the intermediate difficulty", quiz2DoneFunc));//13
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Master of the Universe","Get a perfect result on the hardest difficulty", quiz3DoneFunc));//14
    successList.push_back(new Success(successList.size()+1,m_renderContext, 5, "Making Memories","Save 5 current states", simulationSavedFunc));//15
    successList.push_back(new Success(successList.size()+1,m_renderContext, 15, "Advanced Scrapbooker","Save 15 current state", simulationSavedFuncMultiple));//16
    successList.push_back(new Success(successList.size()+1,m_renderContext, 5, "Time traveller","Load 5 past simulations", simulationLoadedFunc));//17
    successList.push_back(new Success(successList.size()+1,m_renderContext, 15, "Advanced nostalgic","Load 15 past simulations", simulationLoadedFuncMultiple));//18
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "Speed x100","Travel 1 year from now", timeTraveler));//19
    successList.push_back(new Success(successList.size()+1,m_renderContext, 1, "A new decade","Travel 10 year from now", timeTraveler10));//20

    

    float alpha = 0.40;

    successList[0]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/1.png", alpha);
    successList[1]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/2.png", alpha);
    successList[2]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/3.png", alpha);
    successList[3]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/0.png", alpha);
    successList[4]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/4.png", alpha);
    successList[5]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/5.png", alpha);
    successList[6]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/6.png", alpha);
    successList[7]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/7.png", alpha);
    successList[8]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/8.png", alpha);
    successList[9]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/9.png", alpha);
    successList[10]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/10.png", alpha);
    successList[11]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/11.png", alpha);
    successList[12]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/12.png", alpha);
    successList[13]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/13.png", alpha);
    successList[14]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/14.png", alpha);
    successList[15]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/15.png", alpha);
    successList[16]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/16.png", alpha);
    successList[17]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/17.png", alpha);
    successList[18]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/18.png", alpha);
    successList[19]->badgeNOTCOMPLTETED = loadTextureCOMPLETED("assets/badges/19.png", alpha);

    alpha = 1.0;
    successList[0]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/1c.png", alpha);
    successList[1]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/2c.png", alpha);
    successList[2]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/3c.png", alpha);
    successList[3]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/0c.png", alpha);
    successList[4]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/4c.png", alpha);
    successList[5]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/5c.png", alpha);
    successList[6]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/6c.png", alpha);
    successList[7]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/7c.png", alpha);
    successList[8]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/8c.png", alpha);
    successList[9]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/9c.png", alpha);
    successList[10]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/10c.png", alpha);
    successList[11]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/11c.png", alpha);
    successList[12]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/12c.png", alpha);
    successList[13]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/13c.png", alpha);
    successList[14]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/14c.png", alpha);
    successList[15]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/15c.png", alpha);
    successList[16]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/16c.png", alpha);
    successList[17]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/17c.png", alpha);
    successList[18]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/18c.png", alpha);
    successList[19]->badgeCOMPLETED = loadTextureCOMPLETED("assets/badges/19c.png", alpha);

    saveTool->checkSuccessList(successList);
    saveTool->SyncSuccessList(successList);
    
    //Load elements to verify List
    saveTool->loadElementsToVerify(elementsToVerify, 0);

    m_renderContext->steamTool->syncSuccess(successList);
    /*
    for(int i = 0; i < successList.size(); i++){
        //successList[i]->isDone = true;
        m_renderContext->steamTool->removeSuccess(successList[i]->id);
    }
    */
}

void SuccessTool::QUEST_MINIMAP_TRAVEL(){
    minimapTravel = true;
}

void SuccessTool::QUEST_SAVE_LOAD_SIMULATION(bool saved){
    if(saved){
        simulationSaved = true;
        simulationSavedMultiple = true;
    }
    else{
        simulationLoaded = true;
        simulationLoadedMultiple = true;
    }
}

void SuccessTool::QUEST_DESTROY_EARTH(){
    earthDestroyed = true;
}

void SuccessTool::QUEST_QUIZ_DONE(int difficulty, float score){
    if(score < 100){
        quizDone = true;
    }else{//Perfect score
        if(difficulty == 1){
            quiz1Done = true;
        }
        if(difficulty == 2){
            quiz2Done = true;
        }
        if(difficulty == 3){
            quiz3Done = true;
        }
    }

}

void SuccessTool::checkCollision(CelestialObject* obj1, CelestialObject* obj2){
    oneCollision = true;

    bool isEarth = obj1->getDefaultName() == "Earth" || obj2->getDefaultName() == "Earth";
    bool isNotCreated = !obj1->isCreated || !obj2->isCreated;
    if(isEarth && isNotCreated){
        QUEST_DESTROY_EARTH();
    }

    bool StarsCollided = obj1->type==1 && obj2->type==1;
    if(StarsCollided){
        twoStarsCollided = true;
    }

    //If collision between Moon and Jupiter
    if((obj1->getDefaultName() == "Moon" && obj2->getDefaultName() == "Jupiter") || (obj2->getDefaultName() == "Moon" && obj1->getDefaultName() == "Jupiter")){
        DavidVsGoliath = true;
    }

}