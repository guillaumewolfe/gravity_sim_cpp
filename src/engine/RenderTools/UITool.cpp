
#include "engine/RenderTools/UITool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>

UITool::UITool(RenderContext* renderContext) : RenderComponent(renderContext){
    start_time = std::chrono::system_clock::now();
}

void UITool::Draw() {
glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

draw_rect();
draw_labbels();
draw_buttons();


ImGui::End(); 




}

void UITool::draw_rect(){
    float longueur = winWidth * 0.5; // Exemple de taille
    float hauteur = winHeight * 0.06; // Exemple de taille

    // Assurez-vous que le bas du rectangle ne dépasse pas le bas de la fenêtre
    float bottomEdge = winHeight - hauteur; // Nouveau calcul pour le bord inférieur
    float centerPosY = std::min(bottomEdge, winHeight * 0.97f);

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, centerPosY);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 5.0f;

    // Dessinez le rectangle
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(10, 10, 15, 175), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                      ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                      IM_COL32(10,10,10,0), // Couleur
                      cornerRadius, 0, 2.0f);

    if(!*(m_renderContext->isLive)){return;}
    float distanceLive = centerPos.x+winWidth*0.45;
    drawList->AddCircleFilled(ImVec2(distanceLive, centerPos.y), 
                                winWidth*0.005, 
                                IM_COL32(255,20,20,200), // Couleur
                                40);
    drawList->AddText(ImVec2(distanceLive+winWidth*0.01, centerPos.y-winHeight*0.01), 
                        IM_COL32(255,50,50,200), // Couleur
                        "Live");

}


void UITool::draw_buttons(){
        // Dessinez les boutons
        for (Button* button : m_renderContext->buttons) {
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
