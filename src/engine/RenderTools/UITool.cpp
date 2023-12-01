
#include "engine/RenderTools/UITool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>

UITool::UITool(RenderContext* renderContext) : RenderComponent(renderContext){

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
    float longueur = winWidth* 0.32; // Exemple de taille
    float hauteur = winHeight * 0.055; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.97f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 0.0f;


    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(10,10,10,255), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(10,10,10,255), // Couleur
                        cornerRadius,0,2.0f);


    
}


void UITool::draw_buttons(){
        // Dessinez les boutons
        for (Button* button : m_renderContext->buttons) {
            button->Draw();
        }
}


void UITool::draw_labbels(){
    // Update the text of the second label}
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << *(m_renderContext->simulationTime)/86400;
    std::string rounded_simulation_time_str = stream.str();
    std::string newLabelText = rounded_simulation_time_str + " days";
    if (m_renderContext->labels.size() > 0) {
            m_renderContext->labels[0]->UpdateText(newLabelText); 


    std::string newLabelText2 = m_renderContext->speedSettings[*(m_renderContext->currentSpeedIndex)].second;
        m_renderContext->labels[1]->UpdateText(newLabelText2); }
    // Dessinez les labels



    for (Labbel* label : m_renderContext->labels) {
        label->Draw();
    }

}