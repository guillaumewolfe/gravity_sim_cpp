
#include "glad/glad.h"
#include "engine/RenderTools/settingsTool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include "game/game.h"

SettingsTool::SettingsTool(RenderContext* renderContext) : RenderComponent(renderContext){
}

void SettingsTool::init(){
    generate_buttons();
    generate_labels();
    generate_UImodes1();
    generate_UImodes2();
    generate_UImodes3();
}

void SettingsTool::Draw(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.3f, 0.4f, 0.3f)); // Exemple de couleur (bleu foncé)

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    // Obtenir la taille de la fenêtre

    draw_rect();
    draw_labbels();
    draw_buttons();

    if(mode==1){
        draw_mode1();
    }else if(mode==2){
        draw_mode2();
    }else if(mode==3){
        draw_mode3();
    }

    ImGui::End();
    ImGui::PopStyleColor();

    checkButtonState();
    checkVolumeChange();
}

void SettingsTool::generate_buttons(){
    longueur = winWidth * 0.3; // Exemple de taille
    hauteur = winHeight * 0.6; // Exemple de taille

    float size = 0.3*0.9/2;
    icon = new Icon(0.5-size,0.23,ImVec2(0.035f,0.035f),0.35f,"../assets/button/settings.png",0.7);
    ImVec4 buttonColor = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);

    float taille_y = 0.04;
    float taille_x = 0.175*0.35;
    float position_y = (0.5+0.3)*0.95;
    float diffposition_x = (0.3/2)*0.35;
   Button *SaveButton = new Button(0.50f+diffposition_x, position_y, ImVec2(taille_x, taille_y),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Save", 0.2f,18.0f,
                            std::bind(&SettingsTool::SaveButton, this),5);       
   Button *ExitButton = new Button(0.50f-diffposition_x, position_y, ImVec2(taille_x, taille_y),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Cancel", 0.05f,18.0f,
                            std::bind(&SettingsTool::CloseButton, this),5);       

    ImageButton* closeButton = new ImageButton(0.50+size,0.23,ImVec2(0.025f,0.025f),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/close.png", 0,
                            std::bind(&SettingsTool::CloseButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    position_y = (0.5-0.17);
    taille_x = 0.035f*0.9;
    taille_y = 0.06f*0.9;
    float diffx = taille_x*1.4;
    ImageButton* volumeButton = new ImageButton(0.5-diffx,position_y,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/volume.png", 0,
                            std::bind(&SettingsTool::setMode, this, 1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* screenButton = new ImageButton(0.5+diffx,position_y,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/screen.png", 0,
                            std::bind(&SettingsTool::setMode, this, 3),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* controlsButton = new ImageButton(0.5,position_y,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/keyboard.png", 0,
                            std::bind(&SettingsTool::setMode, this, 2),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    buttons.push_back(SaveButton);
    buttons.push_back(ExitButton);

    imageButtons.push_back(closeButton);
    imageButtons.push_back(volumeButton);
    imageButtons.push_back(controlsButton);
    imageButtons.push_back(screenButton);
}

void SettingsTool::generate_labels(){
    Labbel* menuLabbel = new Labbel(0.5f,0.25f,ImVec4(255,255,255,255),
                                "Settings",28.0f,0.8f);

    labbels.push_back(menuLabbel);
}


void SettingsTool::generate_UImodes1(){
    float position_x = 0.5-0.3/2+0.05;
    Labbel* mainVolumeLabel = new Labbel(position_x,0.43f,ImVec4(255,255,255,255),
                                "Main Volume",16.0f,0.8f);
    Labbel* musicVolume = new Labbel(position_x,0.53f,ImVec4(255,255,255,255),
                                "Music Volume",16.0f,0.8f);
    Labbel* effectVolume = new Labbel(position_x,0.63f,ImVec4(255,255,255,255),
                                "Effect Volume",16.0f,0.8f);
    labbelsMode1.push_back(mainVolumeLabel);
    labbelsMode1.push_back(musicVolume);
    labbelsMode1.push_back(effectVolume);

    Slider *MainVolumeSlider = new Slider(0.5f,0.47,ImVec2((0.5-position_x)*2, 0.40*0.0175),ImVec4(225,225,225,225),
                                ImVec4(150.0*0.6, 250.0*0.6, 150.0*0.6, 255),&gameSettings->mainVolume,ImVec2(0,1),20);
    Slider *MusicVolumeSlider = new Slider(0.5f,0.47+0.1,ImVec2((0.5-position_x)*2, 0.40*0.0175),ImVec4(225,225,225,225),
                                ImVec4(150.0*0.6, 250.0*0.6, 150.0*0.6, 255),&gameSettings->musicVolume,ImVec2(0,1),20);
    Slider *EffectVolumeSlider = new Slider(0.5f,0.47+0.2,ImVec2((0.5-position_x)*2, 0.40*0.0175),ImVec4(225,225,225,225),
                                ImVec4(150.0*0.6, 250.0*0.6, 150.0*0.6, 255),&gameSettings->sfxVolume,ImVec2(0,1),20);
    slidersMode1.push_back(MainVolumeSlider);
    slidersMode1.push_back(MusicVolumeSlider);
    slidersMode1.push_back(EffectVolumeSlider);
}

void SettingsTool::generate_UImodes2(){
    float position_x = 0.5-0.3/2+0.05;
    Labbel* movementSpeedLabel = new Labbel(position_x,0.45f,ImVec4(255,255,255,255),
                                "Movement speed",16.0f,0.8f);
    Labbel* rotationSpeed = new Labbel(position_x,0.57f,ImVec4(255,255,255,255),
                                "Rotation speed",16.0f,0.8f);
    labbelsMode2.push_back(movementSpeedLabel);
    labbelsMode2.push_back(rotationSpeed);

    Slider *MovementSlider = new Slider(0.5f,0.49,ImVec2((0.5-position_x)*2, 0.40*0.0175),ImVec4(225,225,225,225),
                                ImVec4(150.0*0.6, 250.0*0.6, 150.0*0.6, 255),&gameSettings->movementSpeed,ImVec2(0,1),20);
    Slider *rotationSlider = new Slider(0.5f,0.61,ImVec2((0.5-position_x)*2, 0.40*0.0175),ImVec4(225,225,225,225),
                                ImVec4(150.0*0.6, 250.0*0.6, 150.0*0.6, 255),&gameSettings->rotationSpeed,ImVec2(0,1),20);

    slidersMode2.push_back(MovementSlider);
    slidersMode2.push_back(rotationSlider);
}
void SettingsTool::generate_UImodes3(){
    Labbel* screen1 = new Labbel(0.465f,0.49f,ImVec4(255,255,255,255),
                                "Primary",16.0f,0.8f);
    Labbel* screen2 = new Labbel(0.535f,0.49f,ImVec4(255,255,255,255),
                                "Secondary",16.0f,0.8f);
    Labbel* highQuality = new Labbel(0.45f,0.55f,ImVec4(255,255,255,255),
                            "High-Quality",16.0f,0.8f);
    Labbel* fullScreen = new Labbel(0.45f,0.59f,ImVec4(255,255,255,255),
                                "Full Screen",16.0f,0.8f);
    Labbel* vsync = new Labbel(0.45f,0.63f,ImVec4(255,255,255,255),
                                "Vertical Synchronization",16.0f,0.8f);
    Labbel* antiAliasing = new Labbel(0.45f,0.67f,ImVec4(255,255,255,255),
                            "Anti Aliasing",16.0f,0.8f);

    labbelsMode3.push_back(screen1);
    labbelsMode3.push_back(screen2);
    labbelsMode3.push_back(highQuality);
    labbelsMode3.push_back(fullScreen);
    labbelsMode3.push_back(vsync);
    labbelsMode3.push_back(antiAliasing);

    ImageButton* screen1Button = new ImageButton(0.465f,0.45f,ImVec2(0.04f,0.05f),0.7f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f), ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/screen1.png", 0,
                            std::bind(&SettingsTool::setScreen, this, 1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* screen2Button = new ImageButton(0.535f,0.45f,ImVec2(0.04f,0.05f),0.7f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f), ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/screen2.png", 0,
                            std::bind(&SettingsTool::setScreen, this, 2),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    screen1Button->isOn = true;
    screen2Button->isOn = false;
    imageButtonsMode3.push_back(screen1Button);
    imageButtonsMode3.push_back(screen2Button);


    ImVec4 color = ImVec4(200,200,200,150);
    ImVec4 active = ImVec4(150,255,150,200);
    float xPosition = 0.55f;
    ToggleButton* qualityToggle = new ToggleButton(xPosition,0.55f,ImVec2(0.027f,0.025f),color,
                                    active,
                                    &gameSettings->highQuality,50);
    ToggleButton* fullScreenToggle = new ToggleButton(xPosition,0.59f,ImVec2(0.027f,0.025f),color,
                                    active,
                                    &gameSettings->fullscreen,50);
    ToggleButton* vsyncToggle = new ToggleButton(xPosition,0.63f,ImVec2(0.027f,0.025f),color,
                                active,
                                &gameSettings->vsync,50);
    ToggleButton* antiAliashingToggle = new ToggleButton(xPosition,0.67f,ImVec2(0.027f,0.025f),color,
                                    active,
                                    &gameSettings->antiAliasing,50);

    ToggleButtonsMode3.push_back(qualityToggle);
    ToggleButtonsMode3.push_back(fullScreenToggle);
    ToggleButtonsMode3.push_back(vsyncToggle);
    ToggleButtonsMode3.push_back(antiAliashingToggle);

}

void SettingsTool::draw_mode1() {
    for (Labbel* label : labbelsMode1) {
        label->Draw();
    }
    for (Slider* slider : slidersMode1) {
        slider->Draw();
    }
    for (Button* btn : buttonsMode1) {
        btn->Draw();
    }
    for (ImageButton* btn : imageButtonsMode1) {
        btn->Draw();
    }
}

void SettingsTool::draw_mode2() {
    for (Labbel* label : labbelsMode2) {
        label->Draw();
    }
    for (Slider* slider : slidersMode2) {
        slider->Draw();
    }
    for (Button* btn : buttonsMode2) {
        btn->Draw();
    }
    for (ImageButton* btn : imageButtonsMode2) {
        btn->Draw();
    }
}

void SettingsTool::draw_mode3() {
    for (Labbel* label : labbelsMode3) {
        label->Draw();
    }
    for (Slider* slider : slidersMode3) {
        slider->Draw();
    }
    for (Button* btn : buttonsMode3) {
        btn->Draw();
    }
    for (ImageButton* btn : imageButtonsMode3) {
        btn->Draw();
    }
    for (ToggleButton* btn : ToggleButtonsMode3) {
        btn->Draw();
    }
}







void SettingsTool::draw_buttons(){
    for (Button* btn : buttons) {
        btn->Draw();
    }
    for (ImageButton* btn : imageButtons) {
        btn->Draw();
    }
    icon->Draw();
}

void SettingsTool::draw_labbels(){
    for (Labbel* label : labbels) {
        label->Draw();
    }
}

void SettingsTool::draw_rect(){
    longueur = winWidth * 0.3; // Exemple de taille
    hauteur = winHeight * 0.6; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);

    // Obtenir le DrawList pour dessiner
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Rayon des coins arrondis
    float cornerRadius = 10.0f; // Vous pouvez ajuster ce rayon selon vos besoins

    //Carré pour tout le fond d'écran
    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winHeight*2,winHeight*2), 
                            IM_COL32(0, 0, 0, 130), // Couleur
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0,255), // Couleur
                            cornerRadius);    // Dessiner le premier rectangle avec coins arrondis

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(255,255,255,40), // Couleur
                        cornerRadius,0,0.2f);

    float yPosition = (0.5-0.15)+ 0.06f*0.9f-0.03; // Juste en dessous des boutons, ajustez 0.02f si nécessaire
    float lineWidth = winWidth * 2*0.06f*0.9;  // La longueur de la ligne, ajustez selon vos besoins
    float lineThickness = 2.0f; // L'épaisseur de la ligne, ajustez selon vos besoins
    ImVec2 lineStart = ImVec2(centerPos.x - lineWidth * 0.5f, winHeight * yPosition);
    ImVec2 lineEnd = ImVec2(centerPos.x + lineWidth * 0.5f, winHeight * yPosition);

    // Dessiner la ligne de délimitation
    drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 100), lineThickness); // Couleur blanche, ajustez si nécessaire

    
}

void SettingsTool::setGameSettings(GameSettings* settings){
    gameSettings = settings;
}

void SettingsTool::setCloseButtonFunction(std::function<void()> function) {
    closeButtonFunction = function;
}

void SettingsTool::setSaveButtonFunction(std::function<void()> function) {
    saveButtonFunction = function;
}

void SettingsTool::checkVolumeChange(){
    if (gameSettings) {
        if (gameSettings->mainVolume != originalMainVolume ||
            gameSettings->musicVolume != originalMusicVolume ||
            gameSettings->sfxVolume != originalSfxVolume) {
            gameSettings->volumeChanged = true;

            // Mettre à jour les valeurs originales pour éviter de marquer plusieurs fois
            originalMainVolume = gameSettings->mainVolume;
            originalMusicVolume = gameSettings->musicVolume;
            originalSfxVolume = gameSettings->sfxVolume;
        }
    }
}



void SettingsTool::checkButtonState(){
    if(mode==1){
        imageButtons[1]->isOn = true;
        imageButtons[2]->isOn = false;
        imageButtons[3]->isOn = false;
    }else if(mode==2){
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = true;
        imageButtons[3]->isOn = false;
}else if(mode==3){
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = false;
        imageButtons[3]->isOn = true;
    }
}
void SettingsTool::setMode(int newMode){
    mode = newMode;
}

void SettingsTool::Open(){
    if (gameSettings) {
        originalSettings = *gameSettings; // Faites une copie des paramètres actuels
        originalMainVolume = gameSettings->mainVolume;
        originalMusicVolume = gameSettings->musicVolume;
        originalSfxVolume = gameSettings->sfxVolume;
    }
    mode = 1;
    checkButtonState();
}

void SettingsTool::CloseButton() {
    closeButtonFunction();
    if (gameSettings) {
        *gameSettings = originalSettings; // Restaurez les paramètres originaux
    }
}

void SettingsTool::SaveButton(){
    if(gameSettings){
        gameSettings->volumeChanged = true;
    }
    saveButtonFunction();
}

void SettingsTool::setScreen(int screen){
    if(screen==1){
        imageButtonsMode3[0]->isOn = true;
        imageButtonsMode3[1]->isOn = false;
    }else if(screen==2){
        imageButtonsMode3[0]->isOn = false;
        imageButtonsMode3[1]->isOn = true;
    }
}