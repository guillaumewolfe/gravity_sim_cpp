#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/SuccessTool.h"
#include <algorithm>
#include "save/saveTool.h"

SuccessTool::SuccessTool(RenderContext* renderContext) : RenderComponent(renderContext){
    saveTool = new SaveTool();
    initSuccess();
    initUI();

}

SuccessTool::~SuccessTool(){
    delete saveTool;
}


void SuccessTool::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    checkSuccess();
    drawUI();
 

    ImGui::End();
}

void SuccessTool::Open(){
    if(!m_renderContext->showSuccessTool){
        m_renderContext->showSuccessTool = true;
    }
    isTransiting = true;
    transitionStep = 0;
    badgeOffset = 0;
    selectedSuccess = nullptr;
    setShowMode(3);
    if(successToShow.empty()){
        setShowMode(1);
    }
}

void SuccessTool::getSuccessListToShow(){
    successToShow.clear();

    for(auto& success : successList){
        if(showMode == ShowMode::showCompleted){
            if(success->isCompleted()){
                successToShow.push_back(success);
                continue;   
                }
    }else if(showMode == ShowMode::showInProgress){
        if(!success->isCompleted()){
            successToShow.push_back(success);
            continue;
        }
    }else{
        successToShow.push_back(success);
    }

}}


void SuccessTool::setShowMode(int mode){
    if(mode == 1){//Show all
        showMode = ShowMode::showAll;

        viewAllButton->isOn = true;
        viewCompletedButton->isOn = false;
        viewInProgressButton->isOn = false;

    }else if(mode == 2){//Show completed
        showMode = ShowMode::showCompleted;

        viewAllButton->isOn = false;
        viewCompletedButton->isOn = true;
        viewInProgressButton->isOn = false;

    }else if(mode == 3){//Show in progress
        showMode = ShowMode::showInProgress;

        viewAllButton->isOn = false;
        viewCompletedButton->isOn = false;
        viewInProgressButton->isOn = true;

    }
    getSuccessListToShow();
    questOffset = 0;
}

void SuccessTool::Close(){
    m_renderContext->showSuccessTool = false;
}

