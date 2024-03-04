#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/WelcomeTool.h"
#include "engine/RenderTools/soundTool.h"
#include "path_util.h"
#include "opencv2/opencv.hpp"


WelcomeTool::WelcomeTool(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    centerPos = ImVec2(winWidth / 2, winHeight / 2);
    longueur = winWidth*0.5;
    hauteur = winHeight*0.65;

    topLeft = ImVec2(centerPos.x - longueur / 2, centerPos.y - hauteur / 2);
    bottomRight = ImVec2(centerPos.x + longueur / 2, centerPos.y + hauteur / 2);
    initUI();
    initIconsMode1();
    initPresentation();

    float fontSizeScaledMain = 18.0f; // Taille de police
    float fontSizeScaledSub = 13.0f; // Taille de police
    gameModeMainFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaledMain*winWidth / 1920);
    gameModeSubFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaledSub*winWidth / 1920);

}


void WelcomeTool::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    
    drawUI();
    drawMode();

    ImGui::End();
}

void WelcomeTool::Open() {
    setMode(1);
}

void WelcomeTool::Close() {
    *shouldDraw = false;
}


void WelcomeTool::setMode(int mode) {
    this->mode = mode;

    if(mode == 1)subTitle->UpdateText("Options");
    if(mode == 2)subTitle->UpdateText("Controls");
    if(mode == 3)subTitle->UpdateText("");
}

void WelcomeTool::drawMode() {
    switch (mode)
    {
    case 1:
        drawMode1();
        break;
    case 2:
        drawMode2();
        break;
    case 3:
        drawMode3();
        break;
    default:
        break;
    }
}


void WelcomeTool::drawUI() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    float cornerRadius = winWidth*0.020;

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
                            IM_COL32(20, 25, 30, 150), // Couleur
                            cornerRadius);

    float var = 1;
    int baseR = 50*var;
    int baseG = 85*var;
    int baseB = 92*var;

    //drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(baseR, baseG, baseB, 100), cornerRadius);

    title->Draw();
    subTitle->Draw();
    nextBtn->Draw();
    backBtn->Draw();
    closeBtn->Draw();
    drawCircles();

    //Cout click position 
    if(ImGui::IsMouseClicked(0))std::cout<< "Click position : " << ImGui::GetMousePos().x/winWidth << " " << ImGui::GetMousePos().y/winHeight << std::endl;
}

void WelcomeTool::initUI(){
    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);

    float taille_x = 0.15;
    float taille_y = 0.15;
    
    //Welcome Label
    title = new Labbel(0.5f,0.25f,ImVec4(255,255,255,255),
                                "Space Query",40.0f,0.6f);
    
    subTitle = new Labbel(0.5f,0.29f,ImVec4(255,255,255,255),
                                    "Options",25.0f,0.4f);
    //imagebuttons
    nextBtn = new ImageButton(nullptr,0.575, 0.775, ImVec2(taille_x*0.225, taille_y*0.225),1.0f,
                        button_color,button_color,
                        "../assets/button/right.png", 0,
                            std::bind(&WelcomeTool::nextMode, this,true),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    backBtn = new ImageButton(nullptr,0.425, 0.775, ImVec2(taille_x*0.225, taille_y*0.225),1.0,
                        button_color,button_color,
                        "../assets/button/left.png", 0,
                            std::bind(&WelcomeTool::nextMode, this,false),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    closeBtn = new ImageButton(nullptr,0.728646, 0.207364, ImVec2(taille_x*0.175, taille_y*0.175),0.65,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&WelcomeTool::Close, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
}

void WelcomeTool::nextMode(bool next){
    if(next){
        mode++;
        if(mode > nbreModes){
            mode = 1;
        }
    }else{
        mode--;
        if(mode < 1){
            mode = nbreModes;
        }
    }
    if(mode == 1)subTitle->UpdateText("Options");
    if(mode == 2)subTitle->UpdateText("Controls");
    if(mode == 3)subTitle->UpdateText("");
}


void WelcomeTool::drawCircles(){
    // Dessine 3 cercles entre les 2 ImageButtons. Si le mode est 1, le premier cercle est rempli, si le mode est 2, le second cercle est rempli, et si le mode est 3, le troisième cercle est rempli.
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 center = ImVec2(centerPos.x, winHeight*0.775);
    float radius = winWidth*0.0030;
    float thickness = winHeight*0.0015;
    float spacing = winWidth*0.0075;

    // Calcule les positions des cercles
    ImVec2 pos1 = ImVec2(center.x - spacing * 1.5, center.y); // Ajuste la position pour 3 cercles
    ImVec2 pos2 = ImVec2(center.x, center.y); // Centre
    ImVec2 pos3 = ImVec2(center.x + spacing * 1.5, center.y); // Nouvelle position pour le 3e cercle

    // Gère le survol pour les 3 cercles
    bool isHovered1 = ImGui::IsMouseHoveringRect(ImVec2(pos1.x - radius, pos1.y - radius), ImVec2(pos1.x + radius, pos1.y + radius));
    bool isHovered2 = ImGui::IsMouseHoveringRect(ImVec2(pos2.x - radius, pos2.y - radius), ImVec2(pos2.x + radius, pos2.y + radius));
    bool isHovered3 = ImGui::IsMouseHoveringRect(ImVec2(pos3.x - radius, pos3.y - radius), ImVec2(pos3.x + radius, pos3.y + radius)); // Nouveau survol pour le 3e cercle

    // Ajuste l'opacité et le rayon en fonction du survol
    float alpha1 = isHovered1 ? 255 : 125;
    float alpha2 = isHovered2 ? 255 : 125;
    float alpha3 = isHovered3 ? 255 : 125; // Nouvelle opacité pour le 3e cercle
    radius = isHovered1 ? winWidth*0.0030 : winWidth*0.0025;
    float radius2 = isHovered2 ? winWidth*0.0030 : winWidth*0.0025;
    float radius3 = isHovered3 ? winWidth*0.0030 : winWidth*0.0025; // Nouveau rayon pour le 3e cercle

    // Dessine les cercles avec la logique de remplissage mise à jour
    if(mode == 1){
        drawList->AddCircleFilled(pos1, radius, IM_COL32(255, 255, 255, alpha1), 100);
        drawList->AddCircle(pos2, radius2, IM_COL32(255, 255, 255, alpha2), 100, thickness);
        drawList->AddCircle(pos3, radius3, IM_COL32(255, 255, 255, alpha3), 100, thickness); // Non rempli pour les modes 2 et 3
    }else if(mode == 2){
        drawList->AddCircle(pos1, radius, IM_COL32(255, 255, 255, alpha1), 100, thickness);
        drawList->AddCircleFilled(pos2, radius2, IM_COL32(255, 255, 255, alpha2), 100);
        drawList->AddCircle(pos3, radius3, IM_COL32(255, 255, 255, alpha3), 100, thickness); // Non rempli pour les modes 1 et 3
    }else if(mode == 3){
        drawList->AddCircle(pos1, radius, IM_COL32(255, 255, 255, alpha1), 100, thickness);
        drawList->AddCircle(pos2, radius2, IM_COL32(255, 255, 255, alpha2), 100, thickness);
        drawList->AddCircleFilled(pos3, radius3, IM_COL32(255, 255, 255, alpha3), 100); // Rempli seulement pour le mode 3
    }

    // Gestion des clics pour définir le mode
    if(isHovered1 && ImGui::IsMouseClicked(0)){
        setMode(1);
    }
    if(isHovered2 && ImGui::IsMouseClicked(0)){
        setMode(2);
    }
    if(isHovered3 && ImGui::IsMouseClicked(0)){ // Nouvelle condition pour le 3e cercle
        setMode(3);
    }
}

void WelcomeTool::drawMode1() {
    playIcon->Draw();
    pauseIcon->Draw();
    stepBackwardIcon->Draw();
    stepForwardIcon->Draw();
    resetCameraIcon->Draw();
    resetModelIcon->Draw();
    SaveIcon->Draw();
    miniMapIcon->Draw();
    compareSizeIcon->Draw();
    addPlanetIcon->Draw();
    cameraIcon->Draw();
    QuizIcon->Draw();

//Draw text next to icons : 
float posXRight = 0.55f;
float posXLeft = 0.325f;
float posY = 0.375f;
float diffY = 0.075f;
float diffyText = 0.02f;

float diffX = 0.025f;
float alphaSub = 125;
float alphaMain = 200;
//DrawList
ImDrawList* drawList = ImGui::GetWindowDrawList();
//Play/Pause
ImVec2 playPos = ImVec2(posXLeft+diffX, posY);
ImGui::PushFont(gameModeMainFont);
float textHeight = ImGui::CalcTextSize("Play/Pause").y;
std::string playText = "Play/Pause";
std::string subPlaytext = "Play or pause the simulation";
drawList->AddText(ImVec2(playPos.x*winWidth,(playPos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), playText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(playPos.x*winWidth, (playPos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subPlaytext.c_str());
ImGui::PopFont();

//Forward/Backward
ImVec2 stepBackwardPos = ImVec2(posXLeft+diffX, posY+diffY);
ImGui::PushFont(gameModeMainFont);
std::string stepBackwardText = "Step Forward/Backward";
std::string subStepBackwardText = "Accelerate or decelerate the simulation";
drawList->AddText(ImVec2(stepBackwardPos.x*winWidth,(stepBackwardPos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), stepBackwardText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(stepBackwardPos.x*winWidth, (stepBackwardPos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subStepBackwardText.c_str());
ImGui::PopFont();

//Reset Camera
ImVec2 resetCameraPos = ImVec2(posXLeft+diffX, posY+2*diffY);
ImGui::PushFont(gameModeMainFont);
std::string resetCameraText = "Reset Camera";
std::string subResetCameraText = "Reset the camera to its initial position";
drawList->AddText(ImVec2(resetCameraPos.x*winWidth,(resetCameraPos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), resetCameraText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(resetCameraPos.x*winWidth, (resetCameraPos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subResetCameraText.c_str());
ImGui::PopFont();

//Reset Model
ImVec2 resetModelPos = ImVec2(posXLeft+diffX, posY+3*diffY);
ImGui::PushFont(gameModeMainFont);
std::string resetModelText = "Restart Simulation";
std::string subResetModelText = "Restart the simulation to default or loaded state";
drawList->AddText(ImVec2(resetModelPos.x*winWidth,(resetModelPos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), resetModelText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(resetModelPos.x*winWidth, (resetModelPos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subResetModelText.c_str());
ImGui::PopFont();

//Save
ImVec2 savePos = ImVec2(posXLeft+diffX, posY+4*diffY);
ImGui::PushFont(gameModeMainFont);
std::string saveText = "Save or Load";
std::string subSaveText = "Save or load the current state of the simulation";
drawList->AddText(ImVec2(savePos.x*winWidth,(savePos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), saveText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(savePos.x*winWidth, (savePos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subSaveText.c_str());
ImGui::PopFont();

//Minimap
ImVec2 miniMapPos = ImVec2(posXRight+diffX, posY);
ImGui::PushFont(gameModeMainFont);
std::string miniMapText = "Minimap";
std::string subMiniMapText = "Show or hide the minimap";
drawList->AddText(ImVec2(miniMapPos.x*winWidth,(miniMapPos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), miniMapText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(miniMapPos.x*winWidth, (miniMapPos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subMiniMapText.c_str());
ImGui::PopFont();

//Compare
ImVec2 comparePos = ImVec2(posXRight+diffX, posY+diffY);
ImGui::PushFont(gameModeMainFont);
std::string compareText = "Compare Objects";
std::string subCompareText = "Compare the size and mass of two objects";
drawList->AddText(ImVec2(comparePos.x*winWidth,(comparePos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), compareText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(comparePos.x*winWidth, (comparePos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subCompareText.c_str());
ImGui::PopFont();

//Add Planet
ImVec2 addPlanetPos = ImVec2(posXRight+diffX, posY+2*diffY);
ImGui::PushFont(gameModeMainFont);
std::string addPlanetText = "Add an Object";
std::string subAddPlanetText = "Create and add a new object to the simulation";
drawList->AddText(ImVec2(addPlanetPos.x*winWidth,(addPlanetPos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), addPlanetText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(addPlanetPos.x*winWidth, (addPlanetPos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subAddPlanetText.c_str());
ImGui::PopFont();

//Camera
ImVec2 cameraPos = ImVec2(posXRight+diffX, posY+3*diffY);
ImGui::PushFont(gameModeMainFont);
std::string cameraText = "Camera options";
std::string subCameraText = "Change the camera settings";
drawList->AddText(ImVec2(cameraPos.x*winWidth,(cameraPos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), cameraText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(cameraPos.x*winWidth, (cameraPos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subCameraText.c_str());
ImGui::PopFont();

//Quiz
ImVec2 quizPos = ImVec2(posXRight+diffX, posY+4*diffY);
ImGui::PushFont(gameModeMainFont);
std::string quizText = "Quiz";
std::string subQuizText = "Test your knowledge with a quiz";
drawList->AddText(ImVec2(quizPos.x*winWidth,(quizPos.y-diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaMain), quizText.c_str());
ImGui::PopFont();
ImGui::PushFont(gameModeSubFont);
drawList->AddText(ImVec2(quizPos.x*winWidth, (quizPos.y+diffyText/2)*winHeight-textHeight/2), IM_COL32(255, 255, 255, alphaSub), subQuizText.c_str());
ImGui::PopFont();





}

void WelcomeTool::drawMode2() {
    keyboardIcon->Draw();  
}



void WelcomeTool::initIconsMode1(){

float posXRight = 0.55f;
float posXLeft = 0.325f;

float posY = 0.375f;
float diffY = 0.075f;

float taille_x = 0.035f*0.3;
float taille_y = 0.06f*0.3;
float alpha = 0.75f;

playIcon = new Icon(posXLeft-taille_x/1.5, posY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/play.png",alpha);
pauseIcon = new Icon(posXLeft+taille_x/1.5, posY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/pause.png",alpha);

stepBackwardIcon = new Icon(posXLeft-taille_x/1.5, posY+diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/backward2.png",alpha);
stepForwardIcon = new Icon(posXLeft+taille_x/1.5, posY+diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/forward2.png",alpha);

taille_x *= 1.55;
taille_y *= 1.55;

resetCameraIcon = new Icon(posXLeft, posY+2*diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/resetPosition.png",alpha);
resetModelIcon = new Icon(posXLeft, posY+3*diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/restart.png",alpha);
SaveIcon = new Icon(posXLeft, posY+4*diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/save.png",alpha);



//Right icons in order : Minimap, Compare, AddPlanet, Camera, Quiz

miniMapIcon = new Icon(posXRight, posY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/solarsystem.png",alpha);
compareSizeIcon = new Icon(posXRight, posY+diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/compare.png",alpha);
addPlanetIcon = new Icon(posXRight, posY+2*diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/planet.png",alpha);
cameraIcon = new Icon(posXRight, posY+3*diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/camera.png",alpha);
QuizIcon = new Icon(posXRight, posY+4*diffY,ImVec2(taille_x, taille_y),1.0f,"../assets/button/quiz.png",alpha);

//How to make sure the image is not stretched? It needs to stay the same size and be centered in the icon.
//919x436
float rapport = 982.0/441.0;
float rapportScreen = static_cast<float>(winWidth) / winHeight;
float pourcX = 0.430;
float pourcY = pourcX * (rapportScreen / rapport);
keyboardIcon = new Icon(0.5, 0.5, ImVec2(pourcX, pourcY), 1.0f, "../assets/button/keyboardD.png", 0.75f);
keyboardIcon->isSquare = false;
}





void WelcomeTool::initPresentation(){

    presentation = "Hello! I'm Guillaume, a recent graduate in Physical Engineering, and I'm thrilled to introduce my first-ever game. With this project,";
    presentation += " I wanted to share my passion for physics and astronomy in a way that's both engaging and informative." ;
    presentation += " My aim was to create an experience that not only sparks interest in our universe but also makes learning about these complex topics accessible and fun.";

    presentation2 = "This game allows players to explore our solar system in a unique and interactive manner. You can compare celestial bodies, see how gravity affects their movement, and control time to observe these changes at your own pace. By adding objects like asteroids, extra suns, or even a black hole, you'll get a hands-on feel for the delicate balance of gravitational forces in space. The simulation uses live-updated data for a realistic experience.";

    presentation3 = "I'm genuinely excited to share this journey through space and time with you. Thank you for supporting my work and encouraging my endeavors. I hope you find as much joy and fascination in exploring the cosmos with this game as I have in creating it.";

    WelcomeLabbel = new Labbel(0.375, 0.38,ImVec4(255,255,255,255),
                                "Welcome!",40.0f,0.30f);

    float spacingY = 0.025;
    float PosY = 0.45;
    float PosX = 0.29;
    presentatonText1 = new Labbel(PosX, PosY,ImVec4(255,255,255,255),
                                "Hello! I'm Guillaume, a recent graduate in Physical Engineering, and I'm thrilled to introduce my first-ever game. With this project,",
                                16.0f,0.6f);

    presentatonText2 = new Labbel(PosX, PosY+spacingY,ImVec4(255,255,255,255),
                                "I wanted to share my passion for physics and astronomy in a way that's both engaging and informative. My aim was to create an",
                                16.0f,0.6f); 

    presentatonText3 = new Labbel(PosX, PosY+2*spacingY,ImVec4(255,255,255,255),
                                "experience that not only sparks interest in our universe but also makes learning about these complex topics accessible and fun.",
                                16.0f,0.6f);


    presentatonText4 = new Labbel(PosX, PosY+4*spacingY,ImVec4(255,255,255,255),
                                "This game allows players to explore our solar system in a unique and interactive manner. You can compare celestial bodies, see how",
                                16.0f,0.6f);
    
    presentatonText5 = new Labbel(PosX, PosY+5*spacingY,ImVec4(255,255,255,255),
                                "gravity affects their movement, and control time to observe these changes at your own pace. By adding objects like asteroids, extra",
                                16.0f,0.6f);

    presentatonText6 = new Labbel(PosX, PosY+6*spacingY,ImVec4(255,255,255,255),
                                "suns, or even a black hole, you'll get a hands-on feel for the delicate balance of gravitational forces in space. The simulation",
                                16.0f,0.6f);

    presentatonText7 = new Labbel(PosX, PosY+7*spacingY,ImVec4(255,255,255,255),
                                "uses live-updated data for a realistic experience.",16.0f,0.6f);

    
    presentatonText8 = new Labbel(PosX, PosY+9*spacingY,ImVec4(255,255,255,255),
                                "I'm genuinely excited to share this journey through space and time with you. Thank you for supporting my work. I hope you find",
                                16.0f,0.6f);
    
    presentatonText9 = new Labbel(PosX, PosY+10*spacingY,ImVec4(255,255,255,255),
                                "as much fascination in exploring the cosmos with this game as I have in creating it.",16.0f,0.6f);

    presentatonText1->alignLeft = true;
    presentatonText2->alignLeft = true;
    presentatonText3->alignLeft = true;
    presentatonText4->alignLeft = true;
    presentatonText5->alignLeft = true;
    presentatonText6->alignLeft = true;    
    presentatonText7->alignLeft = true;      
    presentatonText8->alignLeft = true;
    presentatonText9->alignLeft = true;      

    astronautIcon = new Icon(0.671354, 0.29845, ImVec2(0.075, 0.075), 1.0f, "../assets/button/astro.png", 0.35f);              
}


void WelcomeTool::drawMode3(){
    WelcomeLabbel->Draw();

    presentatonText1->Draw();
    presentatonText2->Draw();
    presentatonText3->Draw();
    presentatonText4->Draw();
    presentatonText5->Draw();
    presentatonText6->Draw();
    presentatonText7->Draw();
    presentatonText8->Draw();
    presentatonText9->Draw();
    astronautIcon->Draw();

}
