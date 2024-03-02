#include "UI/MessageTool.h"
#include <iostream>
#include <sstream>
#include <iomanip>


MessageTool::MessageTool(std::string phr, std::function<void()> onConfirmFunc): phrase(phr), onConfirm(onConfirmFunc){
    generate_labels();
    generate_buttons();
}

void MessageTool::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.3f, 0.4f, 0.3f)); // Exemple de couleur (bleu foncé)

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    // Obtenir la taille de la fenêtre

    draw_rectangles();
    draw_labels();
    draw_buttons();
    if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {OkButtonPressed();}
    if(ImGui::IsKeyPressed(ImGuiKey_Escape)) {CancelButtonPressed();}
    ImGui::End();
    ImGui::PopStyleColor();
}


void MessageTool::draw_rectangles(){
    // Calculer la position centrale de l'écran
    ImVec2 textSize = ImGui::CalcTextSize(phrase.c_str());
    float longueur = winWidth * 0.275; // Exemple de taille
    float hauteur = winHeight * 0.175; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);

    // Obtenir le DrawList pour dessiner
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Rayon des coins arrondis
    float cornerRadius = winWidth*0.01; // Vous pouvez ajuster ce rayon selon vos besoins
    
    //Carré pour tout le fond d'écran
    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winWidth, winHeight), 
                            IM_COL32(0,0,0,200), // Couleur
                            cornerRadius);     

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0,255), // Couleur
                            cornerRadius);    // Dessiner le premier rectangle avec coins arrondis

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);

    // Définir la taille du deuxième rectangle (10% plus grand)
    float enlargementFactor = 1.01f; 
    float largerSquareSizeLongueur = longueur * enlargementFactor;
    float diff = largerSquareSizeLongueur - longueur;
    float largerSquareSizeHauteur = hauteur+diff;

    // Calculer le coin supérieur gauche du deuxième rectangle
    ImVec2 largerTopLeft = ImVec2(centerPos.x - largerSquareSizeLongueur * 0.5f, centerPos.y - largerSquareSizeHauteur * 0.5f);


    
}


void MessageTool::generate_labels(){
    Labbel *MessageLabel = new Labbel(0.5f,0.47f,ImVec4(255,255,255,255),
                            phrase,0,1.0f);
    labbels.push_back(MessageLabel);
}
void MessageTool::draw_labels(){
        for (Labbel* label : labbels) {
        label->Draw();
    }
}


void MessageTool::generate_buttons(){
    float hauteur = 0.54f;

   Button *OKButton = new Button(nullptr,0.465f, hauteur, ImVec2(0.05, 0.04),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Yes", 0.2f,0,
                               std::bind(&MessageTool::OkButtonPressed, this));  
   Button *CancelButton = new Button(nullptr,0.535f, hauteur, ImVec2(0.05, 0.04),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Cancel", 0.05,0,
                               std::bind(&MessageTool::CancelButtonPressed, this));  

    buttons.push_back(OKButton);
    buttons.push_back(CancelButton);

    icon = new Icon(0.3775,0.4375,ImVec2(0.025f,0.025f),0.35f,"../assets/button/warning.png",0.7);
    float size = 0.5-0.3775;
    ImVec4 buttonColor = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    imageButton = new ImageButton(nullptr,0.50+size,0.4375,ImVec2(0.025f,0.025f),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/close.png", 0,
                            std::bind(&MessageTool::CancelButtonPressed, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
        
}
void MessageTool::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
    icon->Draw();
    imageButton->Draw();
}

void MessageTool::OkButtonPressed(){
    onConfirm();
    shouldClose = true;
}

void MessageTool::CancelButtonPressed(){
    shouldClose = true;
}