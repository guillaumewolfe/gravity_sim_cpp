
#include "glad/glad.h"
#include "engine/RenderTools/OptionsTool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include "engine/RenderTools/soundTool.h"


OptionsTool::OptionsTool(RenderContext* renderContext) : RenderComponent(renderContext){
    generate_buttons();
    generate_labels();
}

OptionsTool::~OptionsTool() {
    for (Button* btn : buttons) {
        delete btn;
    }
    for (Labbel* label : labbels) {
        delete label;
    }
    delete imageButton;
    delete icon;
}

void OptionsTool::Draw(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.3f, 0.4f, 0.3f)); // Exemple de couleur (bleu foncé)

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    // Obtenir la taille de la fenêtre

    draw_rect();
    draw_labbels();
    draw_buttons();

    ImGui::End();
    ImGui::PopStyleColor();
}

void OptionsTool::generate_buttons(){
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    icon = new Icon(0.43,0.33,ImVec2(0.04f,0.04f),0.35f,"../assets/button/menuIcon.png",0.7);
    ImVec4 buttonColor = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    float size = 0.175*0.85/2;
    imageButton = new ImageButton(playSoundFunc,0.50+size,0.33,ImVec2(0.025f,0.025f),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/close.png", 0,
                            std::bind(&OptionsTool::CloseButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    float taille_y = 0.045;
    float taille_x = 0.175*0.4;
    float diff = taille_y+0.25*taille_y;
   Button *RestartButton = new Button(playSoundFunc,0.50f, 0.5-diff, ImVec2(taille_x, taille_y),
                               ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Resume", 0.2f,21.0f,
                            std::bind(&OptionsTool::CloseButton, this),5);       
                            
   Button *OptionsButton = new Button(playSoundFunc,0.50f, 0.5, ImVec2(taille_x, taille_y),
                               ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Settings", 0.2f,21.0f,
                            std::bind(&OptionsTool::OptionsButton, this),5);   
   Button *AboutButton = new Button(playSoundFunc,0.50f, 0.5+diff, ImVec2(taille_x, taille_y),
                               ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "About", 0.2f,21.0f,
                            std::bind(&OptionsTool::AboutButton, this),5);  
   Button *MainMenuButton = new Button(playSoundFunc,0.50f, 0.5+2*diff, ImVec2(taille_x, taille_y),
                               ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Main Menu", 0.2f,21.0f,
                            std::bind(&OptionsTool::MenuButton, this),5);   


    buttons.push_back(RestartButton);
    buttons.push_back(OptionsButton);
    buttons.push_back(MainMenuButton);
    buttons.push_back(AboutButton);

}

void OptionsTool::generate_labels(){
    Labbel* menuLabbel = new Labbel(0.5f,0.375f,ImVec4(255,255,255,255),
                                "Options",28.0f,0.8f);

    labbels.push_back(menuLabbel);
}


void OptionsTool::draw_buttons(){
    for (Button* btn : buttons) {
        btn->Draw();
    }
    icon->Draw();
    imageButton->Draw();
}

void OptionsTool::draw_labbels(){
    for (Labbel* label : labbels) {
        label->Draw();
    }
}

void OptionsTool::draw_rect(){

    float longueur = winWidth * 0.175; // Exemple de taille
    float hauteur = winHeight * 0.4; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);

    // Obtenir le DrawList pour dessiner
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Rayon des coins arrondis
    float cornerRadius = winWidth*0.01;

    //Carré pour tout le fond d'écran
    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winHeight*2,winHeight*2), 
                            IM_COL32(0, 0, 0, 200), // Couleur
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0,175), // Couleur
                            cornerRadius);    // Dessiner le premier rectangle avec coins arrondis

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);



    
}



void OptionsTool::setMenuButtonFunction(const std::function<void()>& func) {
    menuButtonFunction = func;
}

void OptionsTool::setAboutButtonFunction(const std::function<void()>& func) {
    aboutButtonFunction = func;
}

void OptionsTool::setSettingsButtonFunction(const std::function<void()>& func) {
    settingsButtonFunction = func;
}

void OptionsTool::setResumeButtonFunction(const std::function<void()>& func) {
    resumeButtonFunction = func;
}

void OptionsTool::CloseButton(){
    resumeButtonFunction();
}

void OptionsTool::AboutButton(){
    aboutButtonFunction();
    *(m_renderContext->showOptions) = false;
}
void OptionsTool::MenuButton(){
    menuButtonFunction();
}

void OptionsTool::OptionsButton(){
    settingsButtonFunction();
    *(m_renderContext->showOptions) = false;
}