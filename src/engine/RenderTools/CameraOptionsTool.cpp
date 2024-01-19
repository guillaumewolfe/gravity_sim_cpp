#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/CameraOptionsTool.h"
#include <vector>
#include <string>
#include "engine/RenderTools/soundTool.h"
#include "path_util.h"

CameraOptionsTool::CameraOptionsTool(RenderContext* renderContext) : RenderComponent(renderContext){
    generate_buttons();
    generate_labels();
    initcameraModeList();
    generate_colors();
    nameFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 13.0);
    nameFontBig = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 16.0);
}
// Destructeur
CameraOptionsTool::~CameraOptionsTool() {
    for (Button* btn : buttons) {
        delete btn;
    }
    for (Labbel* label : labbels) {
        delete label;
    }
    for (ImageButton* imgbtn : imageButtons) {
        delete imgbtn;
    }
    for (Slider* slider : sliders) {
        delete slider;
    }
    for (Icon* icon : icons) {
        delete icon;
    }
    for (ToggleButton* toggle : toggleButtons) {
        delete toggle;
    }

}

void CameraOptionsTool::Draw() {
    if(m_renderContext->isChangingParameters or isClosed){return;}
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    drawBackground();
    if(!isTransitioning && !isClosing){updateButtonMode();draw_UI();}
    else{
        if(transitionProgress<1.0f){
            transitionProgress+=0.07f;
        }
        else{
            if(isClosing){
                isClosed = true;
                closeButtonFunction();
                }
            isTransitioning = false;
        }
    }

    ImGui::End(); 

}


void CameraOptionsTool::initcameraModeList(){
    cameraMode[0]="Options";
    cameraMode[1]="Follow Object";
    cameraMode[2] = "Axis focus";
}

void CameraOptionsTool::drawBackground(){
    // Existing code for sizes and positions
    longueur = winWidth * 0.15; // Example size
    hauteur = winHeight * 0.3; // Example size

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 5.0f;

    // Le point central sera utilisé comme référence pour le coin inférieur droit
    centerPos = ImVec2(winWidth * posX, winHeight * 0.775f);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueur * 0.5f, centerPos.y + hauteur * 0.5f);

    float currentHeight, currentWidth, progress;
    ImVec2 currentTopLeft;

    // Determine the current state of the transition
    if (isTransitioning) {
        // Opening transition
        progress = transitionProgress;
    } else if (isClosing) {
        // Closing transition
        progress = (1 - transitionProgress);
    } else {
        // No transition
        progress = 1.0;
    }

    currentHeight = hauteur * progress;
    currentWidth = longueur * progress;
    currentTopLeft = ImVec2(bottomRight.x - currentWidth, bottomRight.y - currentHeight);

    // Draw the transitioning rectangle
    drawList->AddRectFilled(currentTopLeft, 
                            bottomRight, // Le coin inférieur droit reste fixe
                            IM_COL32(20, 25, 30, 100), // Color
                            cornerRadius);

    drawList->AddRect(currentTopLeft, 
                      bottomRight, // Le coin inférieur droit reste fixe
                      IM_COL32(255, 255, 255, 40), // Color
                      cornerRadius, 0, 0.2f);
}

void CameraOptionsTool::generate_buttons(){
    float taille_x = 0.035f;
    float position_x_left = (1.0+0.6)/2-taille_x/2;
    posX = position_x_left-3*taille_x;
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    taille_x = 0.035f*0.7;
    float taille_y = 0.06f*0.6;
    float posY = 0.675+taille_y*1.2;
    float diffx = taille_x*1.4;
    ImVec4 buttonColor = ImVec4(0,0,0,0);
    ImageButton* freeView = new ImageButton(playSoundFunc,posX-diffx,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/parameters.png", 0,
                            std::bind(&CameraOptionsTool::setMode, this, 1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);     

    ImageButton* followObject = new ImageButton(playSoundFunc,posX,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/follow.png", 0,
                            std::bind(&CameraOptionsTool::setMode, this, 2),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton* axisFocus = new ImageButton(playSoundFunc,posX+diffx,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/axis.png", 0,
                            std::bind(&CameraOptionsTool::setMode, this, 3),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* closeButton = new ImageButton(playSoundFunc,0.74,0.641,ImVec2(0.025f,0.025f),0.4f,
                        buttonColor, buttonColor,
                        "../assets/button/close.png", 0,
                            std::bind(&CameraOptionsTool::closeWindow, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    imageButtons.push_back(closeButton);
    imageButtons.push_back(freeView);
    imageButtons.push_back(followObject);
    imageButtons.push_back(axisFocus);

    float toggleSizeX = 0.019f;
    float toggleSizeY = 0.016f;
    ImVec4 color = ImVec4(200,200,200,150);
    ImVec4 active = ImVec4(150,255,150,200);
    float posXToggle = posX;
    float posYToggle = 0.875f;
    float posYTitle = posYToggle+0.025f;
    float diffXToggle = toggleSizeX*2.5;
    ToggleButton* shownames = new ToggleButton(posXToggle,posYToggle,ImVec2(toggleSizeX,toggleSizeY),color,
                                    active,
                                    &m_renderContext->showAllNames,50);
    ToggleButton* showpaths = new ToggleButton(posXToggle-diffXToggle,posYToggle,ImVec2(toggleSizeX,toggleSizeY),color,
                                    active,
                                    &m_renderContext->showAllPaths,50);
    ToggleButton* showOrbits = new ToggleButton(posXToggle+diffXToggle,posYToggle,ImVec2(toggleSizeX,toggleSizeY),color,
                                    active,
                                    &m_renderContext->showAllOrbits,50);


    Labbel* showNamesLabbel = new Labbel(posXToggle,posYTitle,ImVec4(255,255,255,255),
                        "Show names",16.0f,0.725f);
    Labbel* showPathsLabbel = new Labbel(posXToggle-diffXToggle,posYTitle,ImVec4(255,255,255,255),
                        "Show paths",16.0f,0.725f);
    Labbel* showOrbitsLabbel = new Labbel(posXToggle+diffXToggle,posYTitle,ImVec4(255,255,255,255),
                        "Show orbits",16.0f,0.725f);

    float posYMode1 = 0.825f;
    float diffY = 0.035f;
    Labbel* showNamesLabbelBig = new Labbel(posX - 0.03,posYMode1-diffY,ImVec4(255,255,255,255),
                        "Show names",19.0f,0.725f);
    Labbel* showPathsLabbelBig = new Labbel(posX - 0.03,posYMode1,ImVec4(255,255,255,255),
                        "Show paths",19.0f,0.725f);
    Labbel* showOrbitsLabbelBig = new Labbel(posX - 0.03,posYMode1+diffY,ImVec4(255,255,255,255),
                        "Show orbits",19.0f,0.725f);


    toggleButtons.push_back(shownames);
    toggleButtons.push_back(showpaths);
    toggleButtons.push_back(showOrbits);

    labbels.push_back(showNamesLabbel);
    labbels.push_back(showPathsLabbel);
    labbels.push_back(showOrbitsLabbel);
    labbels.push_back(showNamesLabbelBig);
    labbels.push_back(showPathsLabbelBig);
    labbels.push_back(showOrbitsLabbelBig);


    float yMid = 0.8f;
    Icon *iconAxe = new Icon(posX,yMid+0.025,ImVec2(0.09f,0.09f),0.6f,
                        "../assets/button/axisPLAIN.png",0.7f);
    ImageButton* xAxis = new ImageButton(playSoundFunc,0.647619, 0.862894,ImVec2(0.03f,0.03f),0.7f,
                        buttonColor, buttonColor,
                        "../assets/button/xaxis.png", 0,
                            std::bind(&CameraOptionsTool::focusOnAxisButton, this, "x"),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* yAxis = new ImageButton(playSoundFunc,posX,0.778108,ImVec2(0.03f,0.03f),0.7f,
                        buttonColor, buttonColor,
                        "../assets/button/yaxis.png", 0,
                            std::bind(&CameraOptionsTool::focusOnAxisButton, this, "y"),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* zAxis = new ImageButton(playSoundFunc,0.707483, 0.862894,ImVec2(0.03f,0.03f),0.7f,
                        buttonColor, buttonColor,
                        "../assets/button/zaxis.png", 0,
                            std::bind(&CameraOptionsTool::focusOnAxisButton, this, "z"),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* resetCam = new ImageButton(playSoundFunc,0.648299,0.808108,ImVec2(0.03f,0.03f),0.7f,
                        buttonColor, buttonColor,
                        "../assets/button/reset2.png", 0,
                            std::bind(&CameraOptionsTool::resetViewAxis, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    iconsMode3.push_back(iconAxe);
    imageButtonsMode3.push_back(xAxis);
    imageButtonsMode3.push_back(yAxis);
    imageButtonsMode3.push_back(zAxis);
    imageButtonsMode3.push_back(resetCam);


    Icon *iconCamera = new Icon(posX-0.05,yMid,ImVec2(0.03f,0.03f),0.6f,
                        "../assets/button/camera2.png",0.7f);
    iconsMode2.push_back(iconCamera);

    diffx = 0.04;
   Button *followObjectButton = new Button(playSoundFunc,posXToggle, posYTitle, ImVec2(0.03, 0.03),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0, 1.0f),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0,  1.0f),
                               "Focus", 0.5f,16.0,
                               std::bind(&CameraOptionsTool::followObjectButton, this),1.0); 
   Button *globalViewButtonS = new Button(playSoundFunc,posXToggle-diffx, posYTitle, ImVec2(0.03, 0.03),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0, 1.0f),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0,  1.0f),
                               "Global", 0.5f,16.0,
                               std::bind(&CameraOptionsTool::globalViewButton, this),1.0); 
    Button *firstPerson = new Button(playSoundFunc,posXToggle+diffx, posYTitle, ImVec2(0.03, 0.03),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0, 1.0f),
                               ImVec4(20.0/255.0, 25.0/255.0, 30.0/255.0,  1.0f),
                            "POV", 0.5f,16.0,
                            std::bind(&CameraOptionsTool::POVButton, this),1.0); 
    buttonsMode2.push_back(followObjectButton);
    buttonsMode2.push_back(globalViewButtonS);
    buttonsMode2.push_back(firstPerson);
}

void CameraOptionsTool::generate_labels(){

    Labbel *CameraLabel = new Labbel(posX,0.65f,ImVec4(255,255,255,255),
                            "Camera options",20.0f,0.8f);
    Labbel *ModeLabel = new Labbel(posX,0.675f,ImVec4(200,255,225,255),
                            "Mode",24.0f,0.725f);

    labbels.push_back(CameraLabel);
    labbels.push_back(ModeLabel);

}


void CameraOptionsTool::draw_UI(){
    for (Button* btn : buttons) {
        btn->Draw();
    }

    for (Labbel* label : labbels) {
        label->Draw();
    }

    for (ImageButton* imgbtn : imageButtons) {
        imgbtn->Draw();
    }

    for (Slider* slider : sliders) {
        slider->Draw();
    }
    for (ToggleButton* toggle : toggleButtons) {
        toggle->Draw();
    }

}


void CameraOptionsTool::update_label(){
}


void CameraOptionsTool::updateButtonMode(){
    if(mode==1){
        imageButtons[1]->isOn = true;
        imageButtons[2]->isOn = false;
        imageButtons[3]->isOn = false;
        labbels[7]->UpdateText(cameraMode[0]);
        draw_mode1();
    }
    else if(mode==2){
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = true;
        imageButtons[3]->isOn = false;
        labbels[7]->UpdateText(cameraMode[1]);
        draw_mode2();
    }
    else if(mode==3){
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = false;
        imageButtons[3]->isOn = true;
        labbels[7]->UpdateText(cameraMode[2]);
        draw_mode3();
    }
    
    isGlobalView = m_renderContext->currentCamera->isGlobalFollowing;
    POVtarget = m_renderContext->currentCamera->firstPersonTargetObject;
    isFollowingFirstPerson = (m_renderContext->currentCamera->firstPersonModeEnabled && POVtarget != nullptr);
    if(isGlobalView){
        isFollowingFirstPerson = false;
        isFollowingObject = false;
        buttonsMode2[0]->isOn = false;
        buttonsMode2[1]->isOn = true;
        buttonsMode2[2]->isOn = false;
    }
    else if(isFollowingFirstPerson){
        isFollowingObject = false;
        isGlobalView = false;
        buttonsMode2[0]->isOn = false;
        buttonsMode2[1]->isOn = false;
        buttonsMode2[2]->isOn = true;
    }
    else{
        isFollowingObject = true;
        buttonsMode2[0]->isOn = true;
        buttonsMode2[1]->isOn = false;
        buttonsMode2[2]->isOn = false;
    }
}

void CameraOptionsTool::updateSliderPosition(){
    float posXToggle = posX;
    float toggleSizeX = 0.019f;
    float toggleSizeY = 0.016f;
    float posYToggle = 0.875f;
    float diffXToggle = toggleSizeX*2.5;
    float posXMode1 = posX+0.03;
    float sizeMode1X = toggleSizeX*1.15;
    float sizeMode1Y = toggleSizeY*1;

    float posYMode1 = 0.825f;
    float diffY = 0.035f;
    if(mode==1){
        toggleButtons[0]->isHidden = false;
        toggleButtons[1]->isHidden = false;
        toggleButtons[2]->isHidden = false;
        toggleButtons[0]->setPosition(posXMode1,posYMode1-diffY);
        toggleButtons[1]->setPosition(posXMode1,posYMode1);
        toggleButtons[2]->setPosition(posXMode1,posYMode1+diffY);

        toggleButtons[0]->setSize(sizeMode1X,sizeMode1Y);
        toggleButtons[1]->setSize(sizeMode1X,sizeMode1Y);
        toggleButtons[2]->setSize(sizeMode1X,sizeMode1Y);

        labbels[0]->isHidden = true;
        labbels[1]->isHidden = true;
        labbels[2]->isHidden = true;
        labbels[3]->isHidden = false;
        labbels[4]->isHidden = false;
        labbels[5]->isHidden = false;
    }
    else{
        toggleButtons[0]->isHidden = false;
        toggleButtons[1]->isHidden = false;
        toggleButtons[2]->isHidden = false;
        toggleButtons[0]->setPosition(posXToggle,posYToggle);
        toggleButtons[1]->setPosition(posXToggle-diffXToggle,posYToggle);
        toggleButtons[2]->setPosition(posXToggle+diffXToggle,posYToggle);

        toggleButtons[0]->setSize(toggleSizeX,toggleSizeY);
        toggleButtons[1]->setSize(toggleSizeX,toggleSizeY);
        toggleButtons[2]->setSize(toggleSizeX,toggleSizeY);

        labbels[0]->isHidden = false;
        labbels[1]->isHidden = false;
        labbels[2]->isHidden = false;
        labbels[3]->isHidden = true;
        labbels[4]->isHidden = true;
        labbels[5]->isHidden = true;
    }
    if(mode == 2 || mode == 3){
        //all hidden
        toggleButtons[0]->isHidden = true;
        toggleButtons[1]->isHidden = true;
        toggleButtons[2]->isHidden = true;
        labbels[0]->isHidden = true;
        labbels[1]->isHidden = true;
        labbels[2]->isHidden = true;
        labbels[3]->isHidden = true;
        labbels[4]->isHidden = true;
        labbels[5]->isHidden = true;
    }
}

void CameraOptionsTool::draw_mode1(){
    for(auto& icon: iconsMode1){
        icon->Draw();
    }
    for(auto& button: imageButtonsMode1){
        button->Draw();
    }
}

void CameraOptionsTool::draw_mode2(){
    if(isGlobalView){
        draw_global_following();
    }else if(isFollowingObject){
        draw_focus_following();
    }else if(isFollowingFirstPerson){
        draw_POV_following();
    }
    for (auto& icon: iconsMode2){
        icon->Draw();
    }
    for(auto& button: buttonsMode2){
        button->Draw();
    }
}
void CameraOptionsTool::draw_focus_following(){
    ImGui::PushFont(nameFontBig);
    float positionMidX = posX+0.02;
    float positionMidY = 0.818f;
    float hauteurAngle = m_renderContext->currentCamera->orbitalVerticalAngle;
    float deltaXMax = 0.005f;
    float deltaHauteurMax = 0.025f;

    float distanceCam = 0.05;

    hauteurAngle *= 0.75f;
    float newPosXCam = positionMidX-distanceCam*cos(hauteurAngle);
    float newPosYCam = positionMidY-distanceCam*sin(hauteurAngle);

    // Utilisation d'une fonction sinusoidale pour calculer deltaHauteur
    float deltaHauteur = glm::sin(hauteurAngle) * deltaHauteurMax;
    // Utilisation d'une fonction sinusoidale pour calculer deltaX
    float deltaX = glm::abs(glm::sin(hauteurAngle) * deltaXMax);

    iconsMode2[0]->UpdatePosition(newPosXCam,newPosYCam);

    CelestialObject* obj = m_renderContext->currentCamera->followedObject;

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    float radius = winWidth*0.010;

    ImVec2 position = ImVec2(positionMidX*winWidth,positionMidY*winHeight);

    ImVec4 color = typeDictColor[obj->typeName];
    //Calculate text size
    ImVec2 textSize = ImGui::CalcTextSize(obj->name.c_str());
    drawList->AddCircleFilled(position, radius, IM_COL32(color.x, color.y, color.z, 200), 100);
    drawList->AddText(ImVec2(position.x-textSize.x/2,position.y-radius*4),IM_COL32(255,255,255,255),obj->name.c_str());
    if(obj->type ==1){
        drawSunEffect(position,radius);}
    else if(obj->type !=0){
        drawPlanetLight(position,radius,obj);}

    ImGui::PopFont();
}

void CameraOptionsTool::draw_POV_following(){
    POVtarget = m_renderContext->currentCamera->firstPersonTargetObject;
    if(POVtarget == nullptr){POVtarget = getNewPOVtarget();}
    ImGui::PushFont(nameFontBig);
    float positionMidX = posX-0.03;
    float positionMidY = 0.818f;
    float positionOtherX = positionMidX + 0.07;
    CelestialObject* obj = m_renderContext->currentCamera->followedObject;
    float radiusMain = winWidth*0.012;
    float radiusOther = radiusMain/1.5;
    ImVec2 position = ImVec2(positionMidX*winWidth,positionMidY*winHeight);
    ImVec2 positionOther = ImVec2(positionOtherX*winWidth,positionMidY*winHeight);
    //La camera est entre les 2
    ImVec2 positionCamera = ImVec2((positionMidX+positionOtherX)/2,positionMidY);

    iconsMode2[0]->UpdatePosition(positionCamera.x,positionCamera.y);

    ImVec4 colorMain = typeDictColor[obj->typeName];
    ImVec4 colorOther = typeDictColor[POVtarget->typeName];

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    //Text size for 2 objects
    ImVec2 textSizeMain = ImGui::CalcTextSize(obj->name.c_str());
    ImVec2 textSizeOther = ImGui::CalcTextSize(POVtarget->name.c_str());

    drawList->AddCircleFilled(position, radiusMain, IM_COL32(colorMain.x, colorMain.y, colorMain.z, 200), 100);
    drawList->AddCircleFilled(positionOther, radiusOther, IM_COL32(colorOther.x, colorOther.y, colorOther.z, 200), 100);

    
    ImVec2 positionCombo1 = ImVec2(positionOther.x-textSizeOther.x/2,positionOther.y-radiusOther*4);
    ImVec2 positionCombo2 = ImVec2(position.x-textSizeMain.x/2,position.y-radiusOther*4);
    draw_combo(positionCombo1.x,positionCombo1.y,positionCombo2.x,positionCombo2.y);

    if(obj->type ==1){
        drawSunEffect(position,radiusMain*0.75);}
    else if(obj->type !=0){
        drawPlanetLight(position,radiusMain,obj);}
    if(POVtarget->type ==1){
        drawSunEffect(positionOther,radiusOther);}
    else if(POVtarget->type !=0){
        drawPlanetLight(positionOther,radiusOther,POVtarget);}

    ImGui::PopFont();

}

void CameraOptionsTool::draw_combo(float x1, float y1, float x2, float y2){
    ImGui::PushFont(nameFontBig);
    ImVec2 center = ImVec2(x1,y1);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 textSize = ImGui::CalcTextSize(POVtarget->getName().c_str());

    CelestialObject* newPOVtarget = nullptr; // Initialisation à nullptr
    CelestialObject* followedObject = m_renderContext->currentCamera->followedObject;


    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.0)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texte

    ImGui::SetCursorPos(center);
    ImGui::SetNextItemWidth(textSize.x*1.2);
    if (ImGui::BeginCombo("##POV TARGET", POVtarget->name.c_str(), ImGuiComboFlags_NoArrowButton)) {
        for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
            if(otherObj == followedObject){continue;}
            std::string name = otherObj->getName();
            std::string item = name;
            bool isSelected = (POVtarget->getName() == item);
            if (ImGui::Selectable(item.c_str(), isSelected)) {
                newPOVtarget = otherObj;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (newPOVtarget != nullptr) { // Vérifier si newPOVtarget n'est pas null
        changeFirstPersonTarget(newPOVtarget);
    }

//Même chose, mais cette fois pour changer le followObject
    CelestialObject* newFollowedObject = nullptr; // Initialisation à nullptr
    center = ImVec2(x2-0.003*winWidth,y2);
    textSize = ImGui::CalcTextSize(followedObject->getName().c_str());
    ImGui::SetCursorPos(center);
    ImGui::SetNextItemWidth(textSize.x*1.2);
    if (ImGui::BeginCombo("##FOLLOW OBJECT", followedObject->name.c_str(), ImGuiComboFlags_NoArrowButton)) {
        for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
            if(otherObj == POVtarget){continue;}
            std::string name = otherObj->getName();
            std::string item = name;
            bool isSelected = (followedObject->getName() == item);
            if (ImGui::Selectable(item.c_str(), isSelected)) {
                newFollowedObject = otherObj;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (newFollowedObject != nullptr) { // Vérifier si newFollowedObject n'est pas null
        changeFollowObject(newFollowedObject);
    }



    ImGui::PopStyleColor(7);
    ImGui::PopFont();
}


CelestialObject* CameraOptionsTool::getNewPOVtarget(){
    CelestialObject* followedObject = m_renderContext->currentCamera->followedObject;
    CelestialObject* earth;
    CelestialObject* moon;
    CelestialObject* sun;
    for(auto& obj: m_renderContext->systemeSolaire->objects){
        if(obj->typeName == "Earth"){earth = obj;}
        if(obj->typeName == "Moon"){moon = obj;}
        if(obj->typeName == "Sun"){sun = obj;}
    }
    if(earth != nullptr && moon != nullptr && followedObject == earth){return moon;}
    else if (earth != nullptr && moon != nullptr && followedObject == moon){return earth;}
    else if (sun!= nullptr && sun == m_renderContext->systemeSolaire->getSun() && earth != nullptr){return earth;}
    else{return m_renderContext->systemeSolaire->getSun();}
}

void CameraOptionsTool::draw_global_following(){
    CelestialObject* obj = m_renderContext->currentCamera->followedObject;
    std::vector<decltype(m_renderContext->systemeSolaire->objects)::value_type> sortedObjects(m_renderContext->systemeSolaire->objects.begin(), m_renderContext->systemeSolaire->objects.end());

    // Triez le nouveau vecteur par distance par rapport à obj
    std::sort(sortedObjects.begin(), sortedObjects.end(),
        [obj](CelestialObject* a, CelestialObject* b) {
            auto distanceA = (obj->getRealPosition() - a->getRealPosition()).norm();
            auto distanceB = (obj->getRealPosition() - b->getRealPosition()).norm();
            return distanceA < distanceB;
        }
    );
    //Remove object from sortedObjects if object->type == 5
    sortedObjects.erase(std::remove_if(sortedObjects.begin(), sortedObjects.end(), [](CelestialObject* obj) { return obj->type == 5; }), sortedObjects.end());


    float positionMidX = posX+0.02;
    float positionMidY = 0.818f;
    float maxSize = winWidth*0.04;
    float radiusCenter = winWidth*0.004;
    float otherRadius = radiusCenter/3;
    float increment = maxSize / sortedObjects.size();  // Incrément pour la distance


    iconsMode2[0]->UpdatePosition(posX-0.05,positionMidY);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Vec3 camPosition = m_renderContext->currentCamera->getPosition();
    Vec3 camDirection = (camPosition - obj->getPositionSimulation()).normalize();

        float angleCam = atan2(camDirection.z, camDirection.x);
    int index = 0;
    drawList->AddCircleFilled(ImVec2(positionMidX*winWidth,positionMidY*winHeight),maxSize, IM_COL32(0,0,0, 255), 100);
    drawList->AddCircleFilled(ImVec2(positionMidX*winWidth,positionMidY*winHeight),maxSize, IM_COL32(20, 25, 30, 100), 100);
    

    for (auto& otherObject : sortedObjects) {
        bool isObj = otherObject==obj;
        // Calculer la direction de l'objet et normaliser
        Vec3 objectDirection = (obj->getRealPosition() - otherObject->getRealPosition()).normalize();

        // Calculer l'angle de la direction de l'objet
        float objectAngle = atan2(objectDirection.z, objectDirection.x);

        // Calculer l'angle de la direction de la caméra
        float camAngle = atan2(camDirection.z, camDirection.x);

        // Ajuster l'angle de l'objet avec l'angle de la caméra
        float adjustedAngle = objectAngle - camAngle-M_PI_2;
        // Calculer la distance du centre
        float radius = index * increment;

        // Convertir en coordonnées cartésiennes
        float x = positionMidX*winWidth + radius * cos(adjustedAngle);
        float y = positionMidY*winHeight + radius * sin(adjustedAngle);

        float rayon = isObj ? radiusCenter : otherRadius;
        // Dessiner l'objet
        ImVec2 position(x, y);
        ImVec4 color = typeDictColor[otherObject->typeName];
        drawList->AddCircleFilled(position, rayon, IM_COL32(color.x, color.y, color.z, 200), 100);
        drawList->AddCircle(ImVec2(positionMidX*winWidth,positionMidY*winHeight), radius, IM_COL32(255,255,255, 10), 100,winWidth*0.0005);
        if(otherObject->type ==1){
            float rayonSoleil = isObj? rayon*0.8 : rayon*1.2;
            drawSunEffect(position,rayonSoleil);}
        else if(otherObject->type !=0){
            drawPlanetLight(position,rayon*1.2,otherObject);}
        if(isObj){drawList->AddCircle(position, rayon+winWidth*0.0005, IM_COL32(255,255,255, 200), 100,winWidth*0.001);}
        index++;

    }
}

void CameraOptionsTool::draw_mode3(){
    for(auto& icon: iconsMode3){
        icon->Draw();
    }
    for(auto& button: imageButtonsMode3){
        button->Draw();
    }
    draw_planetsMode3();
}

void CameraOptionsTool::draw_planetsMode3(){
    ImGui::PushFont(nameFont);
    ImVec2 position = iconsMode3[0]->getPosition();
    position.x *= winWidth;
    position.y *= winHeight;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 mousePos = ImGui::GetMousePos();
    float radius = winWidth*0.006;
//Check if mouse is on the planet or the name
    ImVec2 topLeft = ImVec2(position.x-radius,position.y-radius);
    ImVec2 bottomRight = ImVec2(position.x+radius,position.y+radius);
    bool mouseHover = (mousePos.x>topLeft.x && mousePos.x<bottomRight.x && mousePos.y>topLeft.y && mousePos.y<bottomRight.y);
    
    float alpha = mouseHover ? 255 : 150;
    float alphaCircle = mouseHover ? 125 : 100;
    ImVec4 color = m_renderContext->currentCamera->followedObject != nullptr ? typeDictColor[m_renderContext->currentCamera->followedObject->typeName] : ImVec4(255,255,255,255);
    drawList->AddCircleFilled(position, radius, IM_COL32(0,0,0, 255), 100);
    drawList->AddCircleFilled(position, radius, IM_COL32(color.x, color.y, color.z, 150), 100);
    drawList->AddText(ImVec2(position.x+radius*2,position.y-radius*2), IM_COL32(255,255,255, alpha), m_renderContext->currentCamera->followedObject->name.c_str());
    //drawList->AddCircle(position, radius, IM_COL32(255,255,255, 200), 100,winWidth*0.0005);
    if(m_renderContext->currentCamera->followedObject->type ==1){
        drawSunEffect(position,radius);
    }else if(m_renderContext->currentCamera->followedObject->type !=0){
        drawPlanetLight(position,radius,m_renderContext->currentCamera->followedObject);
    }
    float alphaHightlight = mouseHover ? 100 : 0;
    drawList->AddCircle(position, radius, IM_COL32(255,255,255, alphaHightlight), 100,winWidth*0.001);
    
    ImGui::PopFont();
    

    //Inputs
    CelestialObject* planet = m_renderContext->currentCamera->followedObject;
    if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && mouseHover){
        if(m_renderContext->currentCamera->isGlobalFollowing){//Si on est en global
                m_renderContext->currentCamera->newFollowObject(planet);
            }
            else{//Si on est pas en train de suivre la planète
                m_renderContext->currentCamera->newFollowObjectGlobal(planet);
            }
        }

}

void CameraOptionsTool::drawSunEffect(ImVec2 planetPos, float radius){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec4 colorCenterDot = ImVec4(255,215,80,255);
    float numBlurCircles = 40;
    float blurIncrease = radius*0.04; // How much larger each successive blur circle is
    float initialAlpha = 20; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);}
}





void CameraOptionsTool::resetCamPos(){
    m_renderContext->currentCamera->resetPosition();
    *m_renderContext->showInfo = true;
}

void CameraOptionsTool::resetViewAxis(){
    if(m_renderContext->currentCamera->isTransitingAxis){m_renderContext->currentCamera->isTransitingAxis = false;}
    if(m_renderContext->currentCamera->isGlobalFollowing){
        CelestialObject* obj = m_renderContext->currentCamera->followedObject;
        bool show = *m_renderContext->showInfo;
        resetCamPos();
        *m_renderContext->showInfo = show;
        m_renderContext->currentCamera->newFollowObjectGlobal(obj);
    }else{
        m_renderContext->currentCamera->newFollowObject(m_renderContext->currentCamera->followedObject);
    }
}

void CameraOptionsTool::ZoomIn(){
    m_renderContext->currentCamera->zoom(true);
}
void CameraOptionsTool::ZoomOut(){
    m_renderContext->currentCamera->zoom(false);
}


void CameraOptionsTool::setMode(int newMode){
    mode = newMode;

    updateSliderPosition();
}

void CameraOptionsTool::changeFollowObject(CelestialObject* obj){
    m_renderContext->currentCamera->newFollowObject(obj);
}

void CameraOptionsTool::changeFirstPersonTarget(CelestialObject* targetObject){
    m_renderContext->currentCamera->newFirstPersonTarget(targetObject);
}

void CameraOptionsTool::setCloseButtonFunction(const std::function<void()>& func) {
    closeButtonFunction = func;
}

void CameraOptionsTool::globalViewButton(){
    if(m_renderContext->currentCamera->followedObject==nullptr ){return;}
    if(isFollowingFirstPerson){m_renderContext->currentCamera->firstPersonModeEnabled = false;resetCamPos();}
    m_renderContext->currentCamera->newFollowObjectGlobal(m_renderContext->currentCamera->followedObject);
}

void CameraOptionsTool::followObjectButton(){
    if(m_renderContext->currentCamera->followedObject==nullptr ){return;}
    if(isFollowingFirstPerson){m_renderContext->currentCamera->firstPersonModeEnabled = false;resetCamPos();}
    m_renderContext->currentCamera->newFollowObject(m_renderContext->currentCamera->followedObject);
}

void CameraOptionsTool::POVButton(){
if(POVtarget == nullptr){POVtarget = getNewPOVtarget();}
if(m_renderContext->currentCamera->followedObject==nullptr ){return;}
m_renderContext->currentCamera->newFirstPersonTarget(POVtarget);
}


void CameraOptionsTool::closeWindow(){
    Close();
}


void CameraOptionsTool::Open(){
    isTransitioning = true;
    isClosing = false;
    isClosed = false;
    transitionProgress = 0.0f;
    setMode(2);
}

void CameraOptionsTool::Close(){
    isTransitioning = false;
    isClosing = true;
    isClosed = false;
    transitionProgress = 0.0f;
}

void CameraOptionsTool::focusOnAxisButton(std::string axis){
    if(isFollowingFirstPerson){m_renderContext->currentCamera->firstPersonModeEnabled = false;}
    if(axis=="x"){
        m_renderContext->currentCamera->newFocusOnAxis("x");
        }
    else if(axis=="y"){
            m_renderContext->currentCamera->newFocusOnAxis("y");
        }
        else{
            m_renderContext->currentCamera->newFocusOnAxis("z");
        }
}

bool CameraOptionsTool::mouseOnCameraOptions(){
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 topLeft = ImVec2(centerPos.x-longueur*0.5,centerPos.y-hauteur*0.5);
    ImVec2 bottomRight = ImVec2(centerPos.x+longueur*0.5,centerPos.y+hauteur*0.5);
    if(mousePos.x>topLeft.x && mousePos.x<bottomRight.x && mousePos.y>topLeft.y && mousePos.y<bottomRight.y){
        return true;
    }
    return false;

}


void CameraOptionsTool::generate_colors() {
    // Modifier le dictionnaire pour utiliser des chaînes de caractères comme clés
    typeDictColor["Black Hole"] = ImVec4(0, 0, 0, 1);
    typeDictColor["Sun"] = ImVec4(250, 250, 120, 1); // Jaune pour une étoile par exemple
    typeDictColor["Mercury"] = ImVec4(100, 100, 100, 1);
    typeDictColor["Venus"] = ImVec4(238, 147, 17, 1); // Orange pour Venus
    typeDictColor["Earth"] = ImVec4(50, 100, 255, 1); // Bleu pour la Terre
    typeDictColor["Mars"] = ImVec4(197, 84, 62, 1); // Rougeâtre pour Mars
    typeDictColor["Jupiter"] = ImVec4(234, 206, 180, 1);
    typeDictColor["Saturn"] = ImVec4(217, 186, 140, 1); // Jaune pour Saturne
    typeDictColor["Uranus"] = ImVec4(155, 221, 227, 1);
    typeDictColor["Neptune"] = ImVec4(39, 114, 210, 1); // Bleu pour Neptune
    typeDictColor["Ice"] = ImVec4(122, 137, 150, 1); // Bleu clair pour les objets glacés
    typeDictColor["Rock"] = ImVec4(137, 80, 77, 1); // Gris pour les objets rocheux
    typeDictColor["Volcanic"] = ImVec4(62, 95, 86, 1); // Gris foncé pour les objets volcaniques
    typeDictColor["Moon"] = ImVec4(200, 200, 200, 1); // Gris pour les lunes
}
void CameraOptionsTool::drawPlanetLight(ImVec2 planetPos, float radius, CelestialObject* obj){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec4 colorCenterDot = typeDictColor[obj->typeName];
    float numBlurCircles = 20;
    float blurIncrease = radius*0.03; // How much larger each successive blur circle is
    float initialAlpha = 30; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);}
}
