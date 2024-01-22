#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/planeteInfoTool.h"
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip> 
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 
#include "engine/RenderTools/soundTool.h"
#include "path_util.h"

PlaneteInfoTool::PlaneteInfoTool(RenderContext* renderContext) : RenderComponent(renderContext){

    typeDict = m_renderContext->colorByTypeDict;
    initLabels();
    initButtons();
    generate_mode3();
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    float fontsize = 20.0f;
    float fontSizeScaled = fontsize * winWidth / 1920;
    storyFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("../assets/fonts/Roboto.ttf").c_str(), fontSizeScaled);
    radius *= winWidth;
    changeParametersTool = new ChangeParametersTool(m_renderContext);
}

PlaneteInfoTool::~PlaneteInfoTool() {
    for (Labbel* label : labbels) {
        delete label;
    }
    for (Labbel* label : labbelsMode3) {
        delete label;
    }
    for (Button* btn : buttons) {
        delete btn;
    }
    for (Button* btn : buttonsMode3) {
        delete btn;
    }
    for (ImageButton* btn : imageButtons) {
        delete btn;
    }
    for (ImageButton* btn : imageButtonsMode3) {
        delete btn;
    }
    for (ToggleButton* btn : togglebuttonsMode3) {
        delete btn;
    }
    delete changeParametersTool;
}

void PlaneteInfoTool::initLabels(){
    float diff = 0.03f;
    float beg = 0.45f;
    float titleX = 0.875f-0.1*0.9;
    float middleX = 0.875+0.025;
    float fontStaticSize = 22.0f;
    float fontDynamicSize = 21.0f;
    Labbel *nameLabbel = new Labbel(0.875,0.26f,ImVec4(255,255,255,255),
                            "Name",30.0f,0.9f);    

    Labbel *typeLabbel = new Labbel(0.875,0.29f,ImVec4(255,255,255,255),
                            "Type",20.0f,0.9f);  
    Labbel *typeLabbelBO = new Labbel(0.875,0.30f,ImVec4(255,255,255,255),
                        "Type",23.0f,1.0f);  

    Labbel *masseStaticLabbel = new Labbel(titleX ,beg,ImVec4(255,255,255,255),
                        "Mass",fontStaticSize,0.9f);  
    Labbel *masseLabbel = new Labbel(middleX,beg,ImVec4(255,255,255,255),
                    "Mass",fontDynamicSize,0.7f);  

    Labbel *radiusStaticLabbel = new Labbel(titleX ,beg+diff,ImVec4(255,255,255,255),
                    "Radius",fontStaticSize,0.9f); 
    Labbel *radiusLabbel = new Labbel(middleX,beg+diff,ImVec4(255,255,255,255),
                "Radius",fontDynamicSize,0.7f); 
    
    Labbel *orbitalStaticLabbel = new Labbel(titleX ,beg+2*diff,ImVec4(255,255,255,255),
                    "Orbital period",fontStaticSize,0.9f); 
    Labbel *orbitalLabbel = new Labbel(middleX,beg+2*diff,ImVec4(255,255,255,255),
                "Orbital",fontDynamicSize,0.7f); 

     Labbel *sideralStaticLabbel = new Labbel(titleX ,beg+3*diff,ImVec4(255,255,255,255),
                    "Sideral period",fontStaticSize,0.9f); 
    Labbel *sideralLabbel = new Labbel(middleX,beg+3*diff,ImVec4(255,255,255,255),
                "Sideral",fontDynamicSize,0.7f);    
    
    Labbel *distanceStaticLabbel = new Labbel(titleX ,beg+4*diff,ImVec4(255,255,255,255),
                    "Distance from star",fontStaticSize,0.9f); 
    Labbel *distanceLabbel = new Labbel(middleX,beg+4*diff,ImVec4(255,255,255,255),
                "distance",fontDynamicSize,0.7f); 
    Labbel *creationLabbel = new Labbel(0.875,0.32,ImVec4(150,255,175,255),
                "Created",18.0f,0.7f); 
    Labbel* temperatureStaticLabbel = new Labbel(titleX ,beg+5*diff,ImVec4(255,255,255,255),
                    "Temperature",fontStaticSize,0.9f);
    Labbel* temperatureLabbel = new Labbel(middleX,beg+5*diff,ImVec4(255,255,255,255),
                "temperature",fontDynamicSize,0.7f);
    masseStaticLabbel->alignLeft = true;
    radiusStaticLabbel->alignLeft = true;
    orbitalStaticLabbel->alignLeft = true;
    sideralStaticLabbel->alignLeft = true;
    distanceStaticLabbel->alignLeft = true;
    temperatureStaticLabbel->alignLeft = true;
    masseLabbel->alignLeft = true;
    radiusLabbel->alignLeft = true;
    orbitalLabbel->alignLeft = true;
    sideralLabbel->alignLeft = true;
    distanceLabbel->alignLeft = true;
    temperatureLabbel->alignLeft = true;
    
    labbels.push_back(nameLabbel);
    labbels.push_back(typeLabbelBO);
    labbels.push_back(typeLabbel);
    labbels.push_back(masseStaticLabbel);
    labbels.push_back(masseLabbel);
    labbels.push_back(radiusStaticLabbel);
    labbels.push_back(radiusLabbel);
    labbels.push_back(orbitalStaticLabbel);
    labbels.push_back(orbitalLabbel);
    labbels.push_back(sideralStaticLabbel);
    labbels.push_back(sideralLabbel);
    labbels.push_back(distanceStaticLabbel);
    labbels.push_back(distanceLabbel);
    labbels.push_back(temperatureStaticLabbel);
    labbels.push_back(temperatureLabbel);
    labbels.push_back(creationLabbel);

}

void PlaneteInfoTool::initButtons(){
    float taille_x = 0.035f*0.8;
    float taille_y = 0.06f*0.8;
    float x_center = 0.875f;
    float diffx = taille_x*1.3;
    float y_center = 0.36;

    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    ImageButton *infoButton = new ImageButton(playSoundFunc,x_center-diffx, y_center, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/stats.png", 0,
                            std::bind(&PlaneteInfoTool::setMode, this, 1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    infoButton->isOn = true;
    ImageButton *toolButton = new ImageButton(playSoundFunc,x_center+diffx, y_center, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/tool.png", 0,
                            std::bind(&PlaneteInfoTool::setMode, this, 3),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton *textButton = new ImageButton(playSoundFunc,x_center, y_center, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/story.png", 0,
                            std::bind(&PlaneteInfoTool::setMode, this, 2),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton *closeButton = new ImageButton(playSoundFunc,0.96, 0.25, ImVec2(taille_x*0.5, taille_y*0.5),0.60,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&PlaneteInfoTool::closeButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
        

    ImageButton *zoomButton = new ImageButton(playSoundFunc,0.803646, 1-0.29f, ImVec2(taille_x*0.7, taille_y*0.7),0.90,
                        button_color,button_color,
                        "../assets/button/moveTo.png", 0,
                            std::bind(&PlaneteInfoTool::moveToButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
        
    ImageButton *orbit = new ImageButton(playSoundFunc,0.803646, 0.75f, ImVec2(taille_x*0.7, taille_y*0.7),0.90,
                        button_color,button_color,
                        "../assets/button/orbit.png", 0,
                            std::bind(&PlaneteInfoTool::orbitButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton *playStory = new ImageButton(playSoundFunc,0.96-0.16, 0.255, ImVec2(taille_x*0.7, taille_y*0.7),0.90,
                        button_color,button_color,
                        "../assets/button/listen.png", 0,
                            std::bind(&PlaneteInfoTool::playStoryButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    imageButtons.push_back(infoButton);
    imageButtons.push_back(textButton);
    imageButtons.push_back(toolButton);
    imageButtons.push_back(closeButton);
    imageButtons.push_back(zoomButton);
    imageButtons.push_back(orbit);
    imageButtons.push_back(playStory);
}


void PlaneteInfoTool::updateLabels(){
    //Name
    labbels[0]->UpdateText(m_object->getName());

    //Object type
    labbels[2]->UpdateText(getTypeName(m_object->type));
    labbels[2]->UpdateColor(getTypeColor(m_object->type));
    labbels[1]->UpdateAlpha(0);

    if(mode!=1){
    for(int i=3;i<15;i++){
        labbels[i]->isHidden = true;
    }}else{
        for(int i=3;i<15;i++){
        labbels[i]->isHidden = false;
    }}

    //Masse
    std::string massString = formatScientific(m_object->weight)+ " kg";
    labbels[4]->UpdateText(massString);

    //Radius
    std::string radiusString = formatScientific(m_object->real_radius/1000)+ " km";
    labbels[6]->UpdateText(radiusString);

    //Orbital
    std::string orbitalString;
    if(m_object->type==1 || m_object->type==5){orbitalString = "-";}
    else if(!m_object->orbitalString.empty()){
        orbitalString = m_object->orbitalString;
    }
    labbels[8]->UpdateText(orbitalString);


    //Sideral period
    double period = m_object->rotationSidSpeed*24*60*60;
    period = abs(1/period);
    std::ostringstream sideralString;
    sideralString << std::fixed << std::setprecision(2) << period << " days";
    std::string periodString = sideralString.str();
    labbels[10]->UpdateText(periodString);

    //Distance from star
    if(m_object->type==1){
        labbels[12]->UpdateText("-");}
 //Soleil empty

    else if (m_object->type == 2 || m_object->type == 3 || m_object->type == 4){//Planets
    auto distance = (m_object->position_real-m_renderContext->systemeSolaire->getSun()->position_real).norm();
    distance = distance/149597870e3;
    std::ostringstream distanceString;
    distanceString << std::fixed << std::setprecision(2) << distance << " AU";
    std::string distance2String = distanceString.str();
    labbels[12]->UpdateText(distance2String);    
    labbels[11]->UpdateText("Distance from star");}

    else if(m_object->type==5){//Satelites
        auto distance = (m_object->position_real-m_renderContext->systemeSolaire->objects[3]->position_real).norm();
        std::string distanceString = formatScientific(distance)+ " km";
        labbels[12]->UpdateText(distanceString);
        labbels[11]->UpdateText("Distance from planet");
    }
    
    if(m_object->isCreated){
        labbels[15]->UpdateAlpha(0.7);
    }else{
        labbels[15]->UpdateAlpha(0);
    }

//Temperature
    if(!m_object->temperature.empty()){
        labbels[14]->UpdateText(m_object->temperature);
    }else{
        labbels[14]->UpdateText("-");
    }

    if(!*(m_renderContext->isOrbiting) && !m_renderContext->currentCamera->isGlobalFollowing){
        imageButtons[5]->hidden = false;
    }else{
        imageButtons[5]->hidden = true;
    }
    if(mode==3){
        imageButtons[5]->hidden = true;
    }

}


void PlaneteInfoTool::generate_mode3(){
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    float toggleSizeX = 0.03f;
    float toggleSizeY = 0.03f;
    float PositionX = 0.925;
    float PositionYImage = 0.45;
    float diffYImage = 0.040;
    ImageButton* removeButton = new ImageButton(playSoundFunc,PositionX,PositionYImage+3*diffYImage,ImVec2(toggleSizeX,toggleSizeY),0.7f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f), ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/delete.png", 0,
                            [this]() {
                            auto boundFunction = std::bind(&PlaneteInfoTool::removePlanete, this);
                            this->confirmBoxFunction(boundFunction, phrase);}
                            ,3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* changeMassButton = new ImageButton(playSoundFunc,PositionX,PositionYImage,ImVec2(toggleSizeX,toggleSizeY),0.7f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f), ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/weight.png", 0,
                            std::bind(&PlaneteInfoTool::changeMass, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* changeRadiusButton = new ImageButton(playSoundFunc,PositionX,PositionYImage+diffYImage,ImVec2(toggleSizeX,toggleSizeY),0.7f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f), ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/changeRadius.png", 0,
                            std::bind(&PlaneteInfoTool::changeRadius, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton* changePositionButton = new ImageButton(playSoundFunc,PositionX,PositionYImage+2*diffYImage,ImVec2(toggleSizeX,toggleSizeY),0.7f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f), ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/changePosition.png", 0,
                            std::bind(&PlaneteInfoTool::changePosition, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton* changeNameButton = new ImageButton(playSoundFunc,0.925,0.26f,ImVec2(toggleSizeX,toggleSizeY),0.7f,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f), ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/edit.png", 0,
                            std::bind(&PlaneteInfoTool::changeName, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    imageButtonsMode3.push_back(removeButton);
    imageButtonsMode3.push_back(changeMassButton);
    imageButtonsMode3.push_back(changeRadiusButton);
    imageButtonsMode3.push_back(changePositionButton);
    imageButtonsMode3.push_back(changeNameButton);


    ImVec4 color = ImVec4(200,200,200,150);
    ImVec4 active = ImVec4(150,255,150,200);
    toggleSizeX = 0.019f;
    toggleSizeY = 0.016f;
    PositionX = 0.955;
    float PositionY = 0.66f;
    float diffY = 0.030f;
        ToggleButton* showOrbitToggle = new ToggleButton(PositionX,PositionY,ImVec2(toggleSizeX,toggleSizeY),color,
                                        active,
                                        nullptr,50);
        ToggleButton* ShowNameToggle = new ToggleButton(PositionX,PositionY+diffY,ImVec2(toggleSizeX,toggleSizeY),color,
                                        active,
                                        nullptr,50);
        ToggleButton* showPathToggle = new ToggleButton(PositionX,PositionY+2*diffY,ImVec2(toggleSizeX,toggleSizeY),color,
                                        active,
                                        nullptr,50);
        togglebuttonsMode3.push_back(showOrbitToggle);
        togglebuttonsMode3.push_back(ShowNameToggle);
        togglebuttonsMode3.push_back(showPathToggle);

        float sizeXofText = 0.06;
        Labbel* showOrbitLabbel = new Labbel(PositionX-sizeXofText,PositionY,ImVec4(255,255,255,255),
                            "Show orbit",19.0f,0.8f);
        Labbel* ShowNameLabbel = new Labbel(PositionX-sizeXofText,PositionY+diffY,ImVec4(255,255,255,255),
                            "Show name",19.0f,0.8f); 
        Labbel* showPathLabbel = new Labbel(PositionX-sizeXofText,PositionY+2*diffY,ImVec4(255,255,255,255),
                            "Show path",19.0f,0.8f);

        PositionX = 0.805;
        Labbel* changeMasseLabbel = new Labbel(PositionX,PositionYImage,ImVec4(255,255,255,255),
                            "Change mass",21.0f,0.8f);
        Labbel* changeRadiusLabbel = new Labbel(PositionX,PositionYImage+diffYImage,ImVec4(255,255,255,255),
                            "Change radius",21.0f,0.8f);
        Labbel* changeTextureLabbel = new Labbel(PositionX,PositionYImage+2*diffYImage,ImVec4(255,255,255,255),
                            "Change Position",21.0f,0.8f);
        Labbel* removeTextureLabbel = new Labbel(PositionX,PositionYImage+3*diffYImage,ImVec4(255,150,150,255),
                            "Remove",21.0f,0.9f);
        Labbel* smallNamePlanteLabbel = new Labbel(0.875,0.29f,ImVec4(255,255,255,255),
                            "Sun",16.0f,0.9f);


        showOrbitLabbel->alignLeft = true;
        ShowNameLabbel->alignLeft = true;
        showPathLabbel->alignLeft = true;
        changeMasseLabbel->alignLeft = true;
        changeRadiusLabbel->alignLeft = true;
        changeTextureLabbel->alignLeft = true;  
        removeTextureLabbel->alignLeft = true;
        labbelsMode3.push_back(showOrbitLabbel);
        labbelsMode3.push_back(ShowNameLabbel);
        labbelsMode3.push_back(showPathLabbel);
        labbelsMode3.push_back(changeMasseLabbel);
        labbelsMode3.push_back(changeRadiusLabbel);
        labbelsMode3.push_back(changeTextureLabbel);
        labbelsMode3.push_back(removeTextureLabbel);
        labbelsMode3.push_back(smallNamePlanteLabbel);
}


void PlaneteInfoTool::Draw() {

    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    
    if(m_renderContext->currentCamera->selectedObject!=nullptr){
        m_object = m_renderContext->currentCamera->selectedObject;
        updateLabels();
    }
    else if(m_renderContext->currentCamera->followedObject!=nullptr){
        m_object = m_renderContext->currentCamera->followedObject;
        updateLabels();
    }
    if(m_object->isDeleted){
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    drawBackground();
    draw_labels();
    draw_buttons();
    draw_story();
    changeParametersTool->Update(m_object);
    changeParametersTool->Draw();

    if(mode==3){
        draw_mode3();
    }

    ImGui::End(); 

    if(!*(m_renderContext->showInfo)){mode = 1;} //Reset mode

}

void PlaneteInfoTool::draw_buttons(){
    for (ImageButton* button : imageButtons) {
        button->Draw();
    }    
    if(mode==1){
        imageButtons[0]->isOn = true;
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = false;}
    else if(mode==2){
        imageButtons[0]->isOn = false;
        imageButtons[1]->isOn = true;
        imageButtons[2]->isOn = false;}
    else if(mode==3){
        imageButtons[0]->isOn = false;
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = true;}
    else{
        imageButtons[0]->isOn = false;
        imageButtons[1]->isOn = false;
        imageButtons[2]->isOn = false;}

    if(m_renderContext->currentCamera->selectedObject != nullptr && m_renderContext->currentCamera->followedObject == nullptr ||(m_renderContext->currentCamera->followedObject != nullptr  && m_renderContext->currentCamera->selectedObject != nullptr && m_renderContext->currentCamera->followedObject != m_renderContext->currentCamera->selectedObject)||  m_renderContext->currentCamera->isGlobalFollowing ){
        if(mode == 3){imageButtons[4]->hidden = true;}
    else{
        imageButtons[4]->hidden = false;}}

    else{imageButtons[4]->hidden = true;}

//Check if object changed
    if (m_renderContext->currentCamera->selectedObject != nullptr) {
        // Vérifiez si l'objet actuel est différent de l'objet précédent
        if (m_object != previousObject) {
            mode = 1;
            previousObject = m_object;
        }
        m_object = m_renderContext->currentCamera->selectedObject;
        
    } else if (m_renderContext->currentCamera->followedObject != nullptr) {
        // Vérifiez si l'objet actuel est différent de l'objet précédent
        if (m_object != previousObject) {
            mode = 1;
            previousObject = m_object;
        }
        m_object = m_renderContext->currentCamera->followedObject;
    }
}

void PlaneteInfoTool::drawBackground(){
    float longueur = winWidth* 0.20; // Exemple de taille
    float hauteur = winHeight * 0.56; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.875f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);
    isHovering = ImGui::IsMouseHoveringRect(topLeft, bottomRight);
    float cornerRadius = 10.0f;


ImVec4 color = getTypeColor(m_object->type);

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 150), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(color.x,color.y,color.z,50), // Couleur
                        cornerRadius,0,winWidth*0.001);
//IM_COL32(90, 120, 149, 120), // Couleur
    float taille_y = 0.06f*0.8;
    float yPosition = 0.350+taille_y*1.01; // Juste en dessous des boutons, ajustez 0.02f si nécessaire
    float lineWidth = winWidth * 2*0.06f*0.9;  // La longueur de la ligne, ajustez selon vos besoins
    float lineThickness = 2.0f; // L'épaisseur de la ligne, ajustez selon vos besoins
    ImVec2 lineStart = ImVec2(centerPos.x - lineWidth * 0.5f, winHeight * yPosition);
    ImVec2 lineEnd = ImVec2(centerPos.x + lineWidth * 0.5f, winHeight * yPosition);

    // Dessiner la ligne de délimitation
    drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 100), lineThickness); // Couleur blanche, ajustez si nécessaire

}

void PlaneteInfoTool::draw_labels(){
        for (Labbel* label : labbels) {
        label->Draw();
    }    

}

void PlaneteInfoTool::draw_mode3(){
    for (ImageButton* button : imageButtonsMode3) {
        button->Draw();
    }  
    for (Labbel* label : labbelsMode3) {
        label->Draw();
    }
    for (size_t i = 0; i < togglebuttonsMode3.size(); ++i) {
        ToggleButton* button = togglebuttonsMode3[i];
        if(m_object->type == 0 || m_object->type == 1 && i == 0){
            button->isDeactivated=true;}
        else{button->isDeactivated=false;}
            button->Draw();
    }
    
    phrase = "Do you wish to remove "+m_object->getName()+" ?";
    //Effet pour les Toggle sur la planètes (Path + Orbit + name)
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float positionx= 0.8325f;
    float positiony = 1.0-0.315f;
    float orbitRadiusCircle = radius*1.4;
    angleCercleOrbit += 0.005f; // Ajustez cette valeur pour contrôler la vitesse de rotation

    // Calcul des coordonnées x et y du petit rond
    satelliteX = winWidth * positionx + cos(angleCercleOrbit) * orbitRadiusCircle;
    satelliteY = winHeight * positiony + sin(angleCercleOrbit) * orbitRadiusCircle;
    // Dessiner le petit rond en orbite
    float satelliteRadius = winWidth*0.002; // Vous pouvez ajuster la taille du petit rond ici
    ImVec4 couleur = getTypeColor(m_object->type);
    if(m_object->showOrbit && m_object->type!=1 && m_object->type!=0){
    drawList->AddCircle(ImVec2(winWidth * positionx, winHeight * positiony), orbitRadiusCircle, IM_COL32(255, 255, 255, 100), 100, 1.0f);}
    
ImVec4 colorCenterDot;
int numBlurCircles;
if(m_object->type == 2 || m_object->type == 3 ||m_object->type == 4 ){
    colorCenterDot = ImVec4(255,215,80,255);
    numBlurCircles = 40;
}
else if(m_object->type == 5){
    colorCenterDot = ImVec4(50,50,255,255);
    numBlurCircles = 15;
}else{
    colorCenterDot = ImVec4(255,255,255,0);
    numBlurCircles = 0;
}
// Draw the central sun
float radiusCenter = winWidth*0.006;
drawList->AddCircleFilled(ImVec2(positionx*winWidth, positiony*winHeight), radiusCenter, IM_COL32(250, 250, 120,150), 100);
// Draw the blur effect // Number of blur circles
float blurIncrease = radiusCenter*0.06;  // How much larger each successive blur circle is
float initialAlpha = 20; // Starting alpha value for the outermost blur circle
float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
for (int i = 0; i < numBlurCircles; ++i) {
    float blurRadius = satelliteRadius + blurIncrease * (i + 1);
    float alpha = initialAlpha - alphaDecrease * i;
    drawList->AddCircleFilled(ImVec2(positionx*winWidth, positiony*winHeight), blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);
}

    
    
float startAngle = angleCercleOrbit - IM_PI / 1.5; // Retranche 90 degrés
float endAngle = angleCercleOrbit;               // L'angle actuel du satellite

// Normalisation des angles dans l'intervalle [0, 2*PI]
startAngle = fmod(startAngle + 2 * IM_PI, 2 * IM_PI);
endAngle = fmod(endAngle + 2 * IM_PI, 2 * IM_PI);
if (endAngle < startAngle) {
    endAngle += 2 * IM_PI;
}
const int numSegments = 25;
float segmentLength = (endAngle - startAngle) / numSegments;
if(m_object->showPath && m_object->type!=1 && m_object->type!=0){
for (int i = 0; i < numSegments; ++i) {
    // Calcul des angles de début et de fin pour le segment actuel
    float segmentStartAngle = startAngle + i * segmentLength;
    float segmentEndAngle = segmentStartAngle + segmentLength;

    // Calcul de la valeur alpha pour le segment actuel
    float alpha = 255 * ((float)i / numSegments);

    // Dessiner le segment de l'arc avec la valeur alpha
    drawList->PathArcTo(ImVec2(winWidth * positionx, winHeight * positiony), orbitRadiusCircle, segmentStartAngle, segmentEndAngle, 10); // 10 segments pour un dessin lisse de chaque petit arc
    drawList->PathStroke(IM_COL32(255, 255, 255, alpha), false, 4.0f);
}}
if(m_object->showName){
labbelsMode3[7]->isHidden=false;
labbelsMode3[7]->UpdateText(m_object->getName());
float labelOffsetY = 0.025;
    if(m_object->type==1 || m_object->type==0){
        labbelsMode3[7]->UpdatePosition(0.8325f, 1-0.3f-labelOffsetY-0.03);
    }
else{
labbelsMode3[7]->UpdatePosition(satelliteX/winWidth, satelliteY/winHeight - labelOffsetY);}
labbelsMode3[7]->UpdateColor(couleur);}
else{
    labbelsMode3[7]->isHidden=true;
}
if(m_object->showPath && (m_object->type == 0 || m_object->type==1)){
if(m_object->showPath){
// Position de départ et de fin
ImVec2 startPos = ImVec2(winWidth * 0.8325f, winHeight * 0.70f);
ImVec2 endPos = ImVec2(winWidth * 0.78f, winHeight * 0.70f);

// Nombre de segments pour le dégradé
const int numSegments = 20;

// Calcul de la longueur de chaque segment de ligne
float segmentLengthX = (endPos.x - startPos.x) / numSegments;
float segmentLengthY = (endPos.y - startPos.y) / numSegments;

float initialAlpha = 200; // Valeur alpha initiale pour le premier segment
float alphaDecrease = initialAlpha / numSegments; // Diminution de l'alpha par segment

for (int i = 0; i < numSegments; ++i) {
    // Calcul du début et de la fin de chaque segment
    ImVec2 segmentStart = ImVec2(startPos.x + segmentLengthX * i, startPos.y + segmentLengthY * i);
    ImVec2 segmentEnd = ImVec2(segmentStart.x + segmentLengthX, segmentStart.y + segmentLengthY);

    // Calcul de la valeur alpha pour le segment actuel
    float alpha = initialAlpha - alphaDecrease * i;

    // Dessiner le segment de ligne avec la valeur alpha
    drawList->AddLine(segmentStart, segmentEnd, IM_COL32(255, 255, 255, alpha), winHeight*0.003);
}
    
}
}

}

std::string PlaneteInfoTool::getTypeName(int type) {
    return typeDict[type].first;
}

ImVec4 PlaneteInfoTool::getTypeColor(int type) {
    return typeDict[type].second;
}


std::string PlaneteInfoTool::formatScientific(double value) {
    if (value == 0.0) {
        return "0";
    }

    int exponent = static_cast<int>(std::floor(std::log10(std::abs(value))));
    double base = value / std::pow(10.0, exponent);

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2); // Set fixed-point notation with 2 decimal places
    stream << base << " x 10^" << exponent;
    return stream.str();
}


void PlaneteInfoTool::drawTexturedSphere() {
    if(m_object->isDeleted){
        return;
    }
    Vec3 translation;
    float scaledRadius;
    if (mode==3){
        scaledRadius = radius * 0.25;
        if(m_object->type!=1 && m_object->type != 0){
            translation = Vec3(satelliteX, winHeight-satelliteY,0);
        }else{
            translation = Vec3(winWidth * 0.8325f, winHeight * 0.30f,0);
            scaledRadius = radius * 0.75;
        }
    }else{
        scaledRadius = radius;
        translation = Vec3(winWidth * 0.875f, winHeight * 0.29f,0);
    }    
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    glOrtho(0, winWidth, 0, winHeight, -scaledRadius, scaledRadius);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    glTranslatef(translation.x, translation.y, translation.z);
    glRotatef(90,1,0,0);
    glRotatef(Rotation,0,0,-1);
    Rotation+=0.2;
    if(Rotation==360){Rotation-=360;}

    glBindTexture(GL_TEXTURE_2D, m_object->getTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glEnable(GL_TEXTURE_2D);


    const float PI = 3.14159265358979323846;
    const float PI_2 = PI * 2.0f;

    for (int i = 0; i < numSlices; ++i) {
        float phi1 = static_cast<float>(i) / static_cast<float>(numSlices) * PI;
        float phi2 = static_cast<float>(i + 1) / static_cast<float>(numSlices) * PI;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= numSegments; ++j) {
            float theta = static_cast<float>(j) / static_cast<float>(numSegments) * PI_2;

            float x1 = scaledRadius * sinf(phi1) * cosf(theta);
            float y1 = scaledRadius * sinf(phi1) * sinf(theta);
            float z1 = scaledRadius * cosf(phi1);

            float x2 = scaledRadius * sinf(phi2) * cosf(theta);
            float y2 = scaledRadius * sinf(phi2) * sinf(theta);
            float z2 = scaledRadius * cosf(phi2);

            // Calcul des coordonnées de texture en fonction de l'angle theta et phi
            float u1 = static_cast<float>(j) / static_cast<float>(numSegments);
            float v1 = static_cast<float>(i) / static_cast<float>(numSlices);

            float u2 = static_cast<float>(j) / static_cast<float>(numSegments);
            float v2 = static_cast<float>(i + 1) / static_cast<float>(numSlices);

            glTexCoord2f(u1, v1);
            glVertex3f(x1, y1, z1);

            glTexCoord2f(u2, v2);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void PlaneteInfoTool::draw_story(){
if(mode != 2){return;}
ImGui::SetNextWindowPos(ImVec2(winWidth*0.78, winHeight*0.43)); // Position de la fenêtre
ImGui::SetNextWindowSize(ImVec2(winWidth*0.19, winWidth*0.135)); // Taille de la fenêtre
ImGuiStyle& style = ImGui::GetStyle();
style.Colors[ImGuiCol_Text].w = 0.7; // alphaValue est compris entre 0.0f (complètement transparent) et 1.0f (complètement opaque)
// Créer la nouvelle fenêtre
float originalScrollbarSize = style.ScrollbarSize;
style.ScrollbarSize = 10.0f; // Modifier la taille de la barre de défilement

ImGui::Begin("Ma Nouvelle Fenêtre",nullptr, ImGuiWindowFlags_NoDecoration| ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysVerticalScrollbar);

std::string story;
if(m_object->isCreated){
     story = "\tThis object, created by you, now ventures through the solar system. Observe as it travels through space, interacting with planets and moons. Your creation alters its path and the environment around it, offering a unique perspective on the forces at play in the cosmos. Watch how it responds to the gravitational pull of celestial bodies, a direct result of your input in this cosmic journey.";}
else{
     story = m_object->story;
}
// Contenu de la fenêtre...
ImGui::PushFont(storyFont);
ImGui::TextWrapped("%s",story.c_str());
ImGui::PopFont();
ImGui::End();
style.Colors[ImGuiCol_Text].w = 1.0f; // Remettre la valeur par défaut
style.ScrollbarSize = originalScrollbarSize; // Remettre la valeur par défaut
}

void PlaneteInfoTool::setMode(int mode){
    this->mode = mode;

    if(mode==3){updateMode3();}
}

void PlaneteInfoTool::closeButton(){
    *(m_renderContext->showInfo) = false;
    changeParametersTool->setMode(0, nullptr);
}

void PlaneteInfoTool::moveToButton(){
    CelestialObject* selectedObject = m_renderContext->currentCamera->selectedObject;
    CelestialObject* followedObject = m_renderContext->currentCamera->followedObject;
    if(selectedObject!=nullptr){
        if(selectedObject != followedObject || m_renderContext->currentCamera->isGlobalFollowing)
            {m_renderContext->currentCamera->newFollowObject(selectedObject);
}}

}

void PlaneteInfoTool::updateMode3(){
    togglebuttonsMode3[0]->updatePointer(&m_object->showOrbit);
    togglebuttonsMode3[1]->updatePointer(&m_object->showName);
    togglebuttonsMode3[2]->updatePointer(&m_object->showPath);
}

void PlaneteInfoTool::removePlanete(){
    m_renderContext->systemeSolaire->removeObject(m_object);
    *(m_renderContext->showInfo) = false;
    changeParametersTool->setMode(0, nullptr);
    *m_renderContext->isPaused = false;
}
void PlaneteInfoTool::changeMass(){
    changeParametersTool->setMode(1, m_object);
}
void PlaneteInfoTool::changeRadius(){
    changeParametersTool->setMode(2, m_object);
}
void PlaneteInfoTool::changePosition(){
    changeParametersTool->setMode(3, m_object);
}

void PlaneteInfoTool::changeName(){
    changeParametersTool->setMode(4, m_object);
}


void PlaneteInfoTool::orbitButton(){
    m_renderContext->currentCamera->stopAxisTransition();
    *(m_renderContext->isOrbiting) = true;
}

void PlaneteInfoTool::setConfirmBoxFunction(const std::function<void(std::function<void()> func, const std::string& message)>& confirmBoxFunc) {
    confirmBoxFunction = confirmBoxFunc;
}

void PlaneteInfoTool::playStoryButton(){
    if(m_object==nullptr){return;}
    m_renderContext->soundTool->playSound(m_object->typeName);
}