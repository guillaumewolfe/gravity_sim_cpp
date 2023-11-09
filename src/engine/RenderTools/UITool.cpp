#include "engine/RenderTools/UITool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "UI/Buttons.h"
#include "UI/Labbel.h"

UITool::UITool(RenderContext* renderContext) : RenderComponent(renderContext){}

void UITool::Draw() {

ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

draw_labbels();
draw_buttons();

ImGui::End(); 

    


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
    stream << std::fixed << std::setprecision(2) << *(m_renderContext->simulationTime);
    std::string rounded_simulation_time_str = stream.str();
    std::string newLabelText = "Simulation time : " + rounded_simulation_time_str;
    if (m_renderContext->labels.size() > 0) {
            m_renderContext->labels[0]->UpdateText(newLabelText);  
    // Dessinez les labels
    for (Labbel* label : m_renderContext->labels) {
        label->Draw();
    }}
}