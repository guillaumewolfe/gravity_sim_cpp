#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/creatorTool.h"


CreatorTool::CreatorTool(RenderContext* renderContext) : RenderComponent(renderContext){
    generate_buttons();
    generate_labels();
}

void CreatorTool::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.3f, 0.4f, 0.3f)); // Exemple de couleur (bleu foncé)

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    drawBackground();
    draw_buttons();
    draw_labels();


    ImGui::End(); 
    ImGui::PopStyleColor();
}

void CreatorTool::drawBackground(){
    float longueur = winWidth* 0.30; // Exemple de taille
    float hauteur = winHeight * 0.75; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 30.0f;



    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0, 0, 0, 250), // Couleur
                            cornerRadius);

    float cornerRadiusAdjustment = 10.0f;
    drawList->AddRect(topLeft,
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur),
                        IM_COL32(100, 100, 100, 150), // Couleur de remplissage
                        cornerRadius,0,3.0f); // Ajustez le rayon ici

}

void CreatorTool::End(){}



void CreatorTool::generate_labels(){
    Labbel *MessageLabel = new Labbel(0.5f,0.18f,ImVec4(255,255,255,255),
                            "Choose type of object",45.0f,0.7f);
    labbels.push_back(MessageLabel);
}
void CreatorTool::draw_labels(){
        for (Labbel* label : labbels) {
        label->Draw();
    }
}

void CreatorTool::generate_buttons(){
   Button *OKButton = new Button(0.45f, 0.8125, ImVec2(0.05, 0.045),
                                ImVec4(1.0f, 0.5f, 0.5f, 1.0f),
                                ImVec4(1.0f, 0.6f, 0.6f, 1.0f),
                               
                               "Précédant", ImGui::GetFont(), 0.50f,
                               std::bind(&CreatorTool::CloseButtonPressed, this));  

   Button *CloseButton = new Button(0.55f, 0.8125, ImVec2(0.05, 0.04),
                               ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                               ImVec4(0.6f, 1.0f, 0.6f, 1.0f),
                               "Suivant", ImGui::GetFont(), 0.55f,
                               std::bind(&CreatorTool::CloseButtonPressed, this));  
    buttons.push_back(OKButton);
    buttons.push_back(CloseButton);
}
void CreatorTool::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
}

void CreatorTool::CloseButtonPressed(){

}

