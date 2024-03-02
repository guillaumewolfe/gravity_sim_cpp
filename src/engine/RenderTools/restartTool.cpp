
#include "engine/RenderTools/RestartTool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include "engine/RenderTools/soundTool.h"
#include "engine/RenderTools/soundTool.h"

RestartTool::RestartTool(RenderContext* renderContext) : RenderComponent(renderContext){
    initUI();
}

RestartTool::~RestartTool() {
}

void RestartTool::Draw(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.3f, 0.4f, 0.3f)); // Exemple de couleur (bleu foncé)

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    // Obtenir la taille de la fenêtre

    drawUI();

    ImGui::End();
    ImGui::PopStyleColor();
}


void RestartTool::Open(){
    //Check if its a loadedState
    loadStateActive = m_renderContext->systemeSolaire->isLoadedState;
    if(loadStateActive){
        OpenLoad();}
    else{
        OpenDefault();}

}
void RestartTool::Close(){
    m_renderContext->showRestartTool = false;
}

void RestartTool::OpenLoad(){
    messageLabbel->UpdateText("Do you wish to restart the loaded or default state?");
    closeButton->hidden = true;
    restartLoadedStateButton->hidden = false;
    restartDefaultButton->UpdateText("Default");
}

void RestartTool::OpenDefault(){
    messageLabbel->UpdateText("Do you wish to restart the simulation?");
    closeButton->hidden = false;
    restartLoadedStateButton->hidden = true;
    restartDefaultButton->UpdateText("Restart");
}

void RestartTool::restartDefault(){
    restartFunction(true);
    Close();
}

void RestartTool::restartLoadedState(){
    restartFunction(false);
    Close();
}

void RestartTool::drawUI(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueur = winWidth * 0.275; // Exemple de taille
    float hauteur = winHeight * 0.175; // Exemple de taille
    float cornerRadius = winWidth*0.01;
    ImVec2 topLeft = ImVec2(winWidth * 0.5 - longueur * 0.5, winHeight * 0.5 - hauteur * 0.5);
    ImVec2 bottomRight = ImVec2(winWidth * 0.5 + longueur * 0.5, winHeight * 0.5 + hauteur * 0.5);

    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winHeight*2,winHeight*2), 
                            IM_COL32(0, 0, 0, 200), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0, 0, 0, 250), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30, 200), // Color
                            cornerRadius);

    closeImageButton->Draw();
    messageLabbel->Draw();
    restartDefaultButton->Draw();
    restartLoadedStateButton->Draw();
    closeButton->Draw();
    notificationIcon->Draw();

    
}

void RestartTool::initUI(){
    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    float taille_x = 0.35;
    float taille_y = 0.15;

    messageLabbel = new Labbel(0.5f,0.475,ImVec4(255,255,255,255),
                            "Do you wish to restart",20.0f,0.80f);

    closeImageButton = new ImageButton(playSoundFunc,0.62, 0.439, ImVec2(taille_x*0.15, taille_y*0.15),0.65,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&RestartTool::Close, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);    
    float hauteur = 0.54f;
    restartDefaultButton = new Button(playSoundFunc,0.535f, hauteur, ImVec2(0.045, 0.035),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                            "Default", 0.1f,16.0f,
                            std::bind(&RestartTool::restartDefault, this),5); 
    restartLoadedStateButton = new Button(playSoundFunc,0.465f, hauteur, ImVec2(0.045, 0.035),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                            "Loaded", 0.1f,16.0f,
                            std::bind(&RestartTool::restartLoadedState, this),5); 
    closeButton = new Button(playSoundFunc,0.465f, hauteur, ImVec2(0.045, 0.035),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(1.0f, 0.5f, 0.5f, 1.0f),
                            "Close", 0.05f,16.0f,
                            std::bind(&RestartTool::Close, this),5); 
    
    notificationIcon = new Icon(0.38, 0.439,ImVec2(0.02f,0.02f),0.35f,"../assets/button/warning.png",0.7);

}

void RestartTool::setRestartFunction(const std::function<void(bool defaultState)>& func){
    restartFunction = func;
}