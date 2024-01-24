
#include "glad/glad.h"
#include "engine/RenderTools/UITool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <iomanip>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>

UITool::UITool(RenderContext* renderContext) : RenderComponent(renderContext){
    start_time = std::chrono::system_clock::now();
}

UITool::~UITool() {
}

void UITool::Draw() {
glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
auto now = std::chrono::steady_clock::now();
frameCount++;
std::chrono::duration<float> elapsed = now - lastFrameTime;
if (elapsed.count() >= 1.0f) { // Update every second
    fps = frameCount / elapsed.count();
    frameCount = 0;
    lastFrameTime = now;
}

draw_rect();
draw_labbels();
draw_buttons();
std::ostringstream fpsStream;
fpsStream << std::fixed << std::setprecision(2) << fps << " FPS";
ImGui::SetCursorPos(ImVec2(winWidth*0.93, winHeight*0.01));
ImGui::Text("%s", fpsStream.str().c_str());
checkScroll();
ImGui::End(); 


}

void UITool::draw_rect(){
    float longueur = winWidth * 0.60; // Exemple de taille
    float hauteur = winHeight * 0.06; // Exemple de taille

    // Position Y du bord supérieur du rectangle pour que le bas du rectangle soit au bas de la fenêtre
    float topY = winHeight - hauteur;

    // Position X du bord gauche pour centrer le rectangle
    float leftX = (winWidth - longueur) / 2;

    ImVec2 topLeft = ImVec2(leftX, topY);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 5.0f;

    // Dessinez le rectangle
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                      ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                      IM_COL32(10,10,10,0), // Couleur
                      cornerRadius, 0, 2.0f);


    ImVec2 centerPos = ImVec2(leftX + longueur / 2, topY + hauteur / 2);



    bool connexionSuccess = m_renderContext->systemeSolaire->apiTool->connectionSuccess;
    if(connexionSuccess){
        drawLive(drawList, centerPos);
    } else {
        drawConnexionFailed(drawList, centerPos);
    }


}

void UITool::drawLive(ImDrawList* drawList, ImVec2 centerPos){
if(!*(m_renderContext->isLive)){return;}

float distanceLive = centerPos.x-winWidth*0.12;
std::string text = "Live";


    drawList->AddText(ImVec2(distanceLive+winWidth*0.035, centerPos.y-winHeight*0.01), 
                        IM_COL32(150,255,150,200), // Couleur
                        text.c_str());

    drawList->AddCircleFilled(ImVec2(distanceLive+winWidth*0.025, centerPos.y), 
                                winWidth*0.004, 
                                IM_COL32(150,250,150,200), // Couleur
                                40);
}



void UITool::drawConnexionFailed(ImDrawList* drawList, ImVec2 centerPos){
if(!*(m_renderContext->isLive)){return;}

float distanceLive = centerPos.x-winWidth*0.12;
std::string text = "Offline";
ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    drawList->AddText(ImVec2(distanceLive+winWidth*0.033-textSize.x/2, centerPos.y-winHeight*0.01), 
                        IM_COL32(255,150,150,150), // Couleur
                        text.c_str());

}



void UITool::draw_buttons(){
        // Dessinez les boutons
        for (Button* button : m_renderContext->buttons) {
            button->Draw();
        }
        for(ImageButton* button : m_renderContext->imageButtons){
            button->Draw();
        }
}
void UITool::update_time() {
    start_time = std::chrono::system_clock::now();
}

void UITool::draw_labbels() {
    // Utilisez 'start_time' au lieu de 'current_time'
    auto simulated_time = start_time;

    // Obtenez le temps de simulation en secondes et convertissez-le en entier
    auto simulation_time_seconds = static_cast<long long>(*(m_renderContext->simulationTime));

    // Ajoutez cette durée à 'start_time'
    simulated_time += std::chrono::seconds(simulation_time_seconds);
    std::time_t simulated_time_t = std::chrono::system_clock::to_time_t(simulated_time);

    // Formatez la date et l'heure
    std::ostringstream stream;
    stream << std::put_time(std::localtime(&simulated_time_t), "%Y-%m-%d %H:%M:%S");
    std::string newDateTime = stream.str();

    // Mettez à jour le texte du label avec la nouvelle date et heure
    if (m_renderContext->labels.size() > 0) {
        m_renderContext->labels[0]->UpdateText(newDateTime); 
    }

    // Mettez à jour le second label si nécessaire
    std::string newLabelText2 = m_renderContext->speedSettings[*(m_renderContext->currentSpeedIndex)].second;
    if (m_renderContext->labels.size() > 1) {
        m_renderContext->labels[1]->UpdateText(newLabelText2);
    }

    // Dessinez les labels
    for (Labbel* label : m_renderContext->labels) {
        label->Draw();
    }
}


void UITool::checkScroll(){
    if(m_renderContext->mouseIsOnMinimap){return;}
    if(ImGui::IsWindowHovered()){
        if(ImGui::GetIO().MouseWheel != 0){
            if(ImGui::GetIO().MouseWheel > 0){
                bool in = false;
                    m_renderContext->currentCamera->zoomByDistance(in,0.075);
                    }
            else{
                bool in = true;
                m_renderContext->currentCamera->zoomByDistance(in,0.075);
                    }
        }
    }
}