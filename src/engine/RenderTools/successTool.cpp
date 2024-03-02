#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/SuccessTool.h"
#include <algorithm>


SuccessTool::SuccessTool(RenderContext* renderContext) : RenderComponent(renderContext){
    initSuccess();
}

SuccessTool::~SuccessTool(){}


void SuccessTool::Draw() {
    checkSuccess();
}

void SuccessTool::Open(){

}

std::vector<Success*> SuccessTool::getSuccessList() {return successList;}

int SuccessTool::getSuccessCount() {return successList.size();}

void SuccessTool::setNotificationTool(NotificationTool* notificationTool) {this->notificationTool = notificationTool;}


void SuccessTool::checkSuccess() {
    for (auto success : successList) {
        if (!success->isDone && success->checkSuccess()) {
            success->stepsDone++;
            std::cout<<success->title<<" : "<<success->stepsDone<<"/"<<success->totalSteps<<std::endl;
        }

        if(!success->isDone && success->stepsDone >= success->totalSteps) {
            success->isDone = true;
            notificationTool->Open("Success done!", "You have completed the task: " + success->title, "See", "Close", [this](){Open();});
        }
    }
}

void SuccessTool::initSuccess(){
    elementsToVerify = {"Sun", "Mercury", "Venus", "Earth","Moon", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};
        auto exploreSolarSystenFunc = [&]() -> bool {
            if(m_renderContext->currentCamera->followedObject == nullptr) return false;
            if (m_renderContext->currentCamera->followedObject->isCreated == true) return false;
            if(m_renderContext->currentCamera->isGlobalFollowing == true) return false;

            std::string currentObjectName = m_renderContext->currentCamera->followedObject->getDefaultName();
            auto it = std::find(elementsToVerify.begin(), elementsToVerify.end(), currentObjectName);

            if (it != elementsToVerify.end()) {
                elementsToVerify.erase(it);
                return true;
            }
            return false;
        };

    //Succès 1, explore tout le système solaire
    successList.push_back(new Success(m_renderContext, 10, "Explore Solar System","Explore every object in our System and focus on it!", exploreSolarSystenFunc));


    auto checkCollision = [&]() -> bool {
        if(m_renderContext->isCollidingQuest == true){
            return true;
        }
        return false;
    };

    //Succès 2, check collision
    successList.push_back(new Success(m_renderContext, 1, "Collision : Beginner","Make 2 objects collide!", checkCollision));

    //Succès 3: check collision 10 times
    successList.push_back(new Success(m_renderContext, 10, "Collision : Intermediate","Make 10 objects collide!", checkCollision));

    //Succès 4: check collision 100 times
    successList.push_back(new Success(m_renderContext, 50, "Collision : Expert","Make 50 objects collide!", checkCollision));

    //Succès 5: check collision 500 times
    successList.push_back(new Success(m_renderContext, 100, "Collision : Master","Make 100 objects collide!", checkCollision));
    
}

