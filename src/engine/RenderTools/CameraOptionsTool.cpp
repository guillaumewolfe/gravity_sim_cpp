#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/CameraOptionsTool.h"
#include <vector>
#include <string>

CameraOptionsTool::CameraOptionsTool(RenderContext* renderContext) : RenderComponent(renderContext){
    generate_labels();
    generate_buttons();
    initcameraModeList();
}

void CameraOptionsTool::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    updateButtonMode();
    drawBackground();
    draw_UI();
    update_label();
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        ImVec2 mousePos = ImGui::GetMousePos();
        //std::cout << "Clic de souris détecté en position: (" << mousePos.x/winWidth << ", " << mousePos.y/winHeight << ")" << std::endl;
    }
    ImGui::End(); 

}


void CameraOptionsTool::initcameraModeList(){
    cameraMode[0]="Free Camera";
    cameraMode[1]="Follow Object";
    cameraMode[2] = "Axis focus";
}

void CameraOptionsTool::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;


    float longueur = winWidth* 0.175; // Exemple de taille
    float hauteur = winHeight * 0.45; // Exemple de taille

    centerPos = ImVec2(winWidth * 0.095f, winHeight * 0.765f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);   

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 100), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(255,255,255,40), // Couleur
                        cornerRadius,0,0.2f);  
    float taille_y = 0.06f*0.8;
    float yPosition = 0.684; // Juste en dessous des boutons, ajustez 0.02f si nécessaire
    float lineWidth = winWidth *3*0.035f*0.8;  // La longueur de la ligne, ajustez selon vos besoins
    float lineThickness = winHeight*0.002; // L'épaisseur de la ligne, ajustez selon vos besoins
    ImVec2 lineStart = ImVec2(centerPos.x - lineWidth * 0.5f, winHeight * yPosition);
    ImVec2 lineEnd = ImVec2(centerPos.x + lineWidth * 0.5f, winHeight * yPosition);
    drawList->AddLine(lineStart, lineEnd, IM_COL32(255,255,255,100), lineThickness);
}

void CameraOptionsTool::generate_buttons(){
    float taille_x = 0.035f*0.8;
    float taille_y = 0.06f*0.7;
    float posX = 0.095f;
    float posY = 0.65f;
    float diffx = taille_x*1.4;
    ImVec4 buttonColor = ImVec4(0,0,0,0);
    ImageButton* freeView = new ImageButton(posX-diffx,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/solarSystem.png", 0,
                            std::bind(&CameraOptionsTool::setMode, this, 1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);     

    ImageButton* followObject = new ImageButton(posX,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/follow.png", 0,
                            std::bind(&CameraOptionsTool::setMode, this, 2),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton* axisFocus = new ImageButton(posX+diffx,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/axis.png", 0,
                            std::bind(&CameraOptionsTool::setMode, this, 3),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* closeButton = new ImageButton(0.171428,0.558f,ImVec2(0.025f,0.025f),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/close.png", 0,
                            std::bind(&CameraOptionsTool::closeWindow, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    float toggleSizeX = 0.019f;
    float toggleSizeY = 0.016f;
    ImVec4 color = ImVec4(200,200,200,150);
    ImVec4 active = ImVec4(150,255,150,200);
    float posXToggle = 0.095f;
    float posYToggle = 0.95f;
    float diffXToggle = toggleSizeX*2.5;
    ToggleButton* shownames = new ToggleButton(posXToggle,0.95f,ImVec2(toggleSizeX,toggleSizeY),color,
                                    active,
                                    &m_renderContext->showAllNames,50);
    ToggleButton* showpaths = new ToggleButton(posXToggle-diffXToggle,0.95f,ImVec2(toggleSizeX,toggleSizeY),color,
                                    active,
                                    &m_renderContext->showAllPaths,50);
    ToggleButton* showOrbits = new ToggleButton(posXToggle+diffXToggle,0.95f,ImVec2(toggleSizeX,toggleSizeY),color,
                                    active,
                                    &m_renderContext->showAllOrbits,50);


    Labbel* showNamesLabbel = new Labbel(posXToggle,0.925f,ImVec4(255,255,255,255),
                        "Show names",16.0f,0.725f);
    Labbel* showPathsLabbel = new Labbel(posXToggle-diffXToggle,0.925f,ImVec4(255,255,255,255),
                        "Show paths",16.0f,0.725f);
    Labbel* showOrbitsLabbel = new Labbel(posXToggle+diffXToggle,0.925f,ImVec4(255,255,255,255),
                        "Show orbits",16.0f,0.725f);


    imageButtons.push_back(freeView);
    imageButtons.push_back(followObject);
    imageButtons.push_back(axisFocus);
    imageButtons.push_back(closeButton);

    toggleButtons.push_back(shownames);
    toggleButtons.push_back(showpaths);
    toggleButtons.push_back(showOrbits);

    labbels.push_back(showNamesLabbel);
    labbels.push_back(showPathsLabbel);
    labbels.push_back(showOrbitsLabbel);



    posX = 0.095f;
    posY = 0.75;
    taille_x *= 1.2;
    taille_y *= 1.2;
    ImageButton* ZoomIn = new ImageButton(posX+diffx,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/zoomIn.png", 0,
                            std::bind(&CameraOptionsTool::ZoomIn, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton* ZoomOut = new ImageButton(posX-diffx,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/zoomOut2.png", 0,
                            std::bind(&CameraOptionsTool::ZoomOut, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton* resetCam = new ImageButton(posX,posY,ImVec2(taille_x,taille_y),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/resetCam.png", 0,
                            std::bind(&CameraOptionsTool::resetCamPos, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    float topy = 0.83;
    float midY = topy+0.0315;
    float botY = midY+0.025;
    taille_x /= 1.7;
    taille_y /= 1.7;
    ImageButton* arrowLeft = new ImageButton(posX-diffx/2,midY,ImVec2(taille_x,taille_y),1.0f,
                        buttonColor, buttonColor,
                        "../assets/button/arrowLeft.png", 0,
                            std::bind(&CameraOptionsTool::resetCamPos, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton* arrowRight = new ImageButton(posX+diffx/2,midY,ImVec2(taille_x,taille_y),1.0f,
                        buttonColor, buttonColor,
                        "../assets/button/arrowRight.png", 0,
                            std::bind(&CameraOptionsTool::resetCamPos, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton* arrowUp = new ImageButton(posX,topy,ImVec2(taille_x,taille_y),1.0f,
                        buttonColor, buttonColor,
                        "../assets/button/arrowUp.png", 0,
                            std::bind(&CameraOptionsTool::resetCamPos, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton* arrowDown = new ImageButton(posX,midY,ImVec2(taille_x,taille_y),1.0f,
                        buttonColor, buttonColor,
                        "../assets/button/arrowDown.png", 0,
                            std::bind(&CameraOptionsTool::resetCamPos, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    


    imageButtonsMode1.push_back(ZoomIn);
    imageButtonsMode1.push_back(ZoomOut);
    imageButtonsMode1.push_back(resetCam);

    imageButtonsMode1.push_back(arrowLeft);
    imageButtonsMode1.push_back(arrowRight);
    imageButtonsMode1.push_back(arrowUp);
    imageButtonsMode1.push_back(arrowDown);


    //Mode 2
    Labbel *followingMode = new Labbel(0.04f,0.725f,ImVec4(255,255,255,255),
                        "Following ",20.0f,0.725f);
    Labbel *targetMode = new Labbel(0.04f,0.775f,ImVec4(255,255,255,255),
                        "Target ",20.0f,0.725f);
    Labbel *firstPerson = new Labbel(0.0710f,0.825f,ImVec4(255,255,255,255),
                        "Point of view",18.0f,0.725f);
    followingMode->alignLeft = true;
    targetMode->alignLeft = true;

    ToggleButton* showFirstPerson = new ToggleButton(0.125f,0.825f,ImVec2(toggleSizeX*1.25,toggleSizeY*1.25),color,
                                    active,
                                    &m_renderContext->currentCamera->firstPersonModeEnabled,50);
    ImageButton* globalview = new ImageButton(posX,midY,ImVec2(taille_x,taille_y),1.0f,
                        buttonColor, buttonColor,
                        "../assets/button/arrowDown.png", 0,
                            std::bind(&CameraOptionsTool::globalViewButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    imageButtonsMode2.push_back(globalview);

    labbelsMode2.push_back(followingMode);
    labbelsMode2.push_back(targetMode);
    labbelsMode2.push_back(firstPerson);

    toggleButtonsMode2.push_back(showFirstPerson);

    
}
void CameraOptionsTool::generate_labels(){
    Labbel *CameraLabel = new Labbel(0.095f,0.57f,ImVec4(255,255,255,255),
                            "Camera options",20.0f,0.8f);
    Labbel *ModeLabel = new Labbel(0.095f,0.6f,ImVec4(200,255,225,255),
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
    if(mode==1){draw_mode1();}
    else if(mode==2){draw_mode2();}
    else if(mode==3){draw_mode3();}
}


void CameraOptionsTool::update_label(){
}


void CameraOptionsTool::updateButtonMode(){

    if(mode==1){
        imageButtons[0]->isOn = true;
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = false;
        labbels[1]->UpdateText("Free Camera");
    }else if(mode==2){
        imageButtons[0]->isOn = false;
        imageButtons[1]->isOn = true;
        imageButtons[2]->isOn = false;
        labbels[1]->UpdateText("Follow Object");
        if(m_renderContext->currentCamera->firstPersonModeEnabled){
            labbelsMode2[1]->UpdateAlpha(0.725);
        }else{
            labbelsMode2[1]->UpdateAlpha(0.2);
        }
        if(m_renderContext->currentCamera->followedObject != nullptr){
            labbelsMode2[2]->UpdateAlpha(0.725);
            toggleButtonsMode2[0]->isDeactivated = false;}
        else{
            labbelsMode2[2]->UpdateAlpha(0.2);
            toggleButtonsMode2[0]->isDeactivated = true;
        }
    }else if(mode==3){
        imageButtons[0]->isOn = false;
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = true;
        labbels[1]->UpdateText("Axis Focus");
    }
}

void CameraOptionsTool::draw_mode1(){
    for (ImageButton* imgbtn : imageButtonsMode1) {
        imgbtn->Draw();
    }
    for (Labbel* label : labbelsMode1) {
        label->Draw();
    }
}
void CameraOptionsTool::draw_mode2(){
    for (ImageButton* imgbtn : imageButtonsMode2) {
        imgbtn->Draw();
    }
    for (Labbel* label : labbelsMode2) {
        label->Draw();
    }
    for (ToggleButton* toggle : toggleButtonsMode2) {
        toggle->Draw();
    }
    drawDropDownMenusMode2();
}
void CameraOptionsTool::draw_mode3(){
    for (ImageButton* imgbtn : imageButtonsMode3) {
        imgbtn->Draw();
    }
    for (Labbel* label : labbelsMode3) {
        label->Draw();
    }
}

void CameraOptionsTool::drawDropDownMenusMode2(){
//Drop down list for following object :
    std::vector<std::string> objectNames;
    for (const auto& obj : m_renderContext->systemeSolaire->objects) {
        objectNames.push_back(obj->getName()); // Assuming each object has a getName() method
    }

    // Determine the display name for the current item
    std::string currentDisplayName;
    if (m_renderContext->currentCamera->followedObject) {
        currentDisplayName = m_renderContext->currentCamera->followedObject->getName();
    } else {
        currentDisplayName = "Select Object"; // Placeholder text when there is no followed object
    }


    // Calculer la taille du texte actuel
    ImVec2 textSize = ImGui::CalcTextSize(currentDisplayName.c_str(), NULL, false, 0.0f);
    float offsetX = winWidth * 0.0875;
    float offsetY = winHeight * (0.725f) - textSize.y/2;
    ImVec2 cursorPos = ImVec2(offsetX, offsetY);
    ImGui::SetCursorPos(cursorPos);
    ImGui::PushItemWidth(textSize.x*1.15);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 1, 0.95, 0.0)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.85, 1, 0.85, 0.1)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 1.0f, 0.9f, 1.0f)); // Texte
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Ajustez ces valeurs selon vos besoins
    // Create the dropdown list
    if (ImGui::BeginCombo("##Follow Object", currentDisplayName.c_str(),ImGuiComboFlags_NoArrowButton)) {
        for (int i = 0; i < objectNames.size(); ++i) {
            bool isSelected = (currentDisplayName == objectNames[i]);
            if (ImGui::Selectable(objectNames[i].c_str(), isSelected)) {
                // Change the followed object
                changeFollowObject(m_renderContext->systemeSolaire->objects[i]);
            }
            if (isSelected) {
                // Set the initial focus when opening the combo
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Drop down list for first person target
    if(m_renderContext->currentCamera->firstPersonModeEnabled && m_renderContext->currentCamera->followedObject != nullptr){
    CelestialObject* followedObject = m_renderContext->currentCamera->followedObject;

    // Préparer la liste des noms d'objets pour la sélection de la cible, en excluant l'objet suivi
    std::vector<std::string> targetNames;
    std::vector<CelestialObject*> targets;
    for (const auto& obj : m_renderContext->systemeSolaire->objects) {
        if (obj != followedObject) {
            targetNames.push_back(obj->getName());
            targets.push_back(obj);
        }
    }

    // Trouver l'index de la cible actuelle dans la liste des cibles possibles
    int currentTargetIndex = -1;
    if (m_renderContext->currentCamera->firstPersonTargetObject) {
        for (size_t i = 0; i < targets.size(); ++i) {
            if (targets[i] == m_renderContext->currentCamera->firstPersonTargetObject) {
                currentTargetIndex = i;
                break;
            }
        }
    }

    // Si la cible actuelle est invalide (identique à l'objet suivi ou non définie), sélectionnez la prochaine cible valide
    if (currentTargetIndex == -1 && !targets.empty()) {
        currentTargetIndex = 0; // Sélectionner le premier élément par défaut
        changeFirstPersonTarget(targets[currentTargetIndex]);
    }

    // Préparation de l'interface utilisateur pour la liste déroulante
    std::string currentTargetName = targetNames[currentTargetIndex];

    ImVec2 targetTextSize = ImGui::CalcTextSize(currentTargetName.c_str(), NULL, false, 0.0f);
    float targetOffsetX = winWidth * 0.0875;
    float targetOffsetY = winHeight * (0.775f) - targetTextSize.y / 2;
    ImGui::SetCursorPos(ImVec2(targetOffsetX, targetOffsetY));
    ImGui::PushItemWidth(targetTextSize.x * 1.15);

    // Affichage de la liste déroulante pour la sélection de la cible
    if (ImGui::BeginCombo("##First Person Target", currentTargetName.c_str(), ImGuiComboFlags_NoArrowButton)) {
        for (int i = 0; i < targetNames.size(); ++i) {
            bool isSelected = (i == currentTargetIndex);
            if (ImGui::Selectable(targetNames[i].c_str(), isSelected)) {
                // Change the first person target
                changeFirstPersonTarget(targets[i]);
            }
            if (isSelected) {
                // Set the initial focus when opening the combo
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    }
    // Restauration des styles et couleurs de ImGui
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(7);

}


void CameraOptionsTool::resetCamPos(){
    m_renderContext->currentCamera->resetPosition();
}
void CameraOptionsTool::ZoomIn(){
    m_renderContext->currentCamera->zoom(true);
}
void CameraOptionsTool::ZoomOut(){
    m_renderContext->currentCamera->zoom(false);
}


void CameraOptionsTool::setMode(int newMode){
    mode = newMode;
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
    m_renderContext->currentCamera->newFollowObjectGlobal(m_renderContext->currentCamera->followedObject);
}

void CameraOptionsTool::closeWindow(){
    closeButtonFunction();
}

void CameraOptionsTool::Open(){
    if(m_renderContext->currentCamera->followedObject != nullptr){
        mode = 2;
    }else{
        mode = 1;
    }
}