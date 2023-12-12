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


PlaneteInfoTool::PlaneteInfoTool(RenderContext* renderContext) : RenderComponent(renderContext){

    typeDict = m_renderContext->colorByTypeDict;
    initLabels();
    initButtons();
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    storyFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("../assets/fonts/Roboto.ttf", 16.0f);
}

void PlaneteInfoTool::initLabels(){
    float diff = 0.04f;
    float beg = 0.45f;
    float titleX = 0.82f;
    float middleX = 0.92;
    Labbel *nameLabbel = new Labbel(0.875,0.26f,ImVec4(255,255,255,255),
                            "Name",30.0f,0.9f);    

    Labbel *typeLabbel = new Labbel(0.875,0.30f,ImVec4(255,255,255,255),
                            "Type",20.0f,0.9f);  
    Labbel *typeLabbelBO = new Labbel(0.875,0.30f,ImVec4(255,255,255,255),
                        "Type",23.0f,1.0f);  

    Labbel *masseStaticLabbel = new Labbel(titleX ,beg,ImVec4(255,255,255,255),
                        "Mass",20.0f,0.9f);  
    Labbel *masseLabbel = new Labbel(middleX,beg,ImVec4(255,255,255,255),
                    "Mass",18.0f,0.7f);  

    Labbel *radiusStaticLabbel = new Labbel(titleX ,beg+diff,ImVec4(255,255,255,255),
                    "Radius",20.0f,0.9f); 
    Labbel *radiusLabbel = new Labbel(middleX,beg+diff,ImVec4(255,255,255,255),
                "Radius",18.0f,0.7f); 
    
    Labbel *orbitalStaticLabbel = new Labbel(titleX ,beg+2*diff,ImVec4(255,255,255,255),
                    "Orbital period",20.0f,0.9f); 
    Labbel *orbitalLabbel = new Labbel(middleX,beg+2*diff,ImVec4(255,255,255,255),
                "Orbital",18.0f,0.7f); 

     Labbel *sideralStaticLabbel = new Labbel(titleX ,beg+3*diff,ImVec4(255,255,255,255),
                    "Sideral period",20.0f,0.9f); 
    Labbel *sideralLabbel = new Labbel(middleX,beg+3*diff,ImVec4(255,255,255,255),
                "Sideral",18.0f,0.7f);    
    
    Labbel *distanceStaticLabbel = new Labbel(titleX ,beg+4*diff,ImVec4(255,255,255,255),
                    "Distance from star",15.0f,0.9f); 
    Labbel *distanceLabbel = new Labbel(middleX,beg+4*diff,ImVec4(255,255,255,255),
                "distance",18.0f,0.7f); 
    Labbel *creationLabbel = new Labbel(0.875,0.34,ImVec4(150,255,175,255),
                "Created",18.0f,0.7f); 


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
    labbels.push_back(creationLabbel);
}

void PlaneteInfoTool::initButtons(){
    float taille_x = 0.035f*0.8;
    float taille_y = 0.06f*0.8;
    float x_center = 0.875f;
    float diffx = taille_x*1.3;
    float y_center = 0.375;

    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);

    ImageButton *infoButton = new ImageButton(x_center-diffx, y_center, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/stats.png", 0,
                            std::bind(&PlaneteInfoTool::setMode, this, 1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    infoButton->isOn = true;
    ImageButton *toolButton = new ImageButton(x_center+diffx, y_center, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/tool.png", 0,
                            std::bind(&PlaneteInfoTool::setMode, this, 3),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton *textButton = new ImageButton(x_center, y_center, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/story.png", 0,
                            std::bind(&PlaneteInfoTool::setMode, this, 2),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton *closeButton = new ImageButton(0.96, 0.25, ImVec2(taille_x*0.5, taille_y*0.5),0.60,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&PlaneteInfoTool::closeButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
        

    ImageButton *zoomButton = new ImageButton(0.96-0.17, 0.25, ImVec2(taille_x*0.6, taille_y*0.6),0.90,
                        button_color,button_color,
                        "../assets/button/moveTo.png", 0,
                            std::bind(&PlaneteInfoTool::moveToButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
        


    imageButtons.push_back(infoButton);
    imageButtons.push_back(textButton);
    imageButtons.push_back(toolButton);
    imageButtons.push_back(closeButton);
    imageButtons.push_back(zoomButton);
}


void PlaneteInfoTool::updateLabels(){
    //Name
    labbels[0]->UpdateText(m_object->getName());

    //Object type
    labbels[2]->UpdateText(getTypeName(m_object->type));
    labbels[2]->UpdateColor(getTypeColor(m_object->type));
    if(m_object->type==0){
    labbels[1]->UpdateText(getTypeName(m_object->type));
    labbels[1]->UpdateAlpha(0.6);}
    else{labbels[1]->UpdateAlpha(0);}

    if(mode!=1){
    for(int i=3;i<13;i++){
        labbels[i]->isHidden = true;
    }}else{
        for(int i=3;i<13;i++){
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
    period = 1/period;
    std::ostringstream sideralString;
    sideralString << std::fixed << std::setprecision(2) << period << " days";
    std::string periodString = sideralString.str();
    labbels[10]->UpdateText(periodString);

    //Distance from star
    if(m_object->type==1){
        std::string st = "-";
        labbels[12]->UpdateText("");
        labbels[11]->UpdateText("");}
 //Soleil empty

    else if (m_object->type == 2 || m_object->type == 3 || m_object->type == 4){//Planets
    auto distance = (m_object->position_real-m_renderContext->systemeSolaire->objects[0]->position_real).norm();
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
        labbels[13]->UpdateAlpha(0.7);
    }else{
        labbels[13]->UpdateAlpha(0);
    }

}


void PlaneteInfoTool::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    if(m_renderContext->currentCamera->followedObject!=nullptr){
        m_object = m_renderContext->currentCamera->followedObject;
        updateLabels();
    }else if(m_renderContext->currentCamera->selectedObject!=nullptr){
        m_object = m_renderContext->currentCamera->selectedObject;
        updateLabels();
    }


    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    drawBackground();
    draw_labels();
    draw_buttons();
    draw_story();

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

    if(m_renderContext->currentCamera->selectedObject != nullptr && m_renderContext->currentCamera->followedObject == nullptr){
        imageButtons[4]->hidden = false;}
    else{imageButtons[4]->hidden = true;}

//Check if object changed
    if (m_renderContext->currentCamera->followedObject != nullptr) {
        // Vérifiez si l'objet actuel est différent de l'objet précédent
        if (m_object != previousObject) {
            mode = 1;
            previousObject = m_object;
        }
        m_object = m_renderContext->currentCamera->followedObject;
    } else if (m_renderContext->currentCamera->selectedObject != nullptr) {
        // Vérifiez si l'objet actuel est différent de l'objet précédent
        if (m_object != previousObject) {
            mode = 1;
            previousObject = m_object;
        }
        m_object = m_renderContext->currentCamera->selectedObject;
    }
}

void PlaneteInfoTool::drawBackground(){
    float longueur = winWidth* 0.20; // Exemple de taille
    float hauteur = winHeight * 0.55; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.875f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;


ImVec4 color = getTypeColor(m_object->type);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(50,50,50,20), // Couleur
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(color.x,color.y,color.z,5), // Couleur
                            cornerRadius);
    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(color.x,color.y,color.z,120), // Couleur
                        cornerRadius,0,2.0f);
//IM_COL32(90, 120, 149, 120), // Couleur
}

void PlaneteInfoTool::draw_labels(){
        for (Labbel* label : labbels) {
        label->Draw();
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


void PlaneteInfoTool::drawTexturedSphere(float radius, int numSegments, int numSlices) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    glOrtho(0, winWidth, 0, winHeight, -radius, radius);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    // Positionnement de la sphère en fonction de la distance calculée
    glTranslatef(winWidth * 0.875f, winHeight * 0.29f,0);
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

            float x1 = radius * sinf(phi1) * cosf(theta);
            float y1 = radius * sinf(phi1) * sinf(theta);
            float z1 = radius * cosf(phi1);

            float x2 = radius * sinf(phi2) * cosf(theta);
            float y2 = radius * sinf(phi2) * sinf(theta);
            float z2 = radius * cosf(phi2);

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
ImGui::Begin("Ma Nouvelle Fenêtre",nullptr, ImGuiWindowFlags_NoDecoration| ImGuiWindowFlags_NoBackground);

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
}

void PlaneteInfoTool::setMode(int mode){
    this->mode = mode;
}

void PlaneteInfoTool::closeButton(){
    *(m_renderContext->showInfo) = false;
}

void PlaneteInfoTool::moveToButton(){
    CelestialObject* selectedObject = m_renderContext->currentCamera->selectedObject;
    CelestialObject* followedObject = m_renderContext->currentCamera->followedObject;
    if(selectedObject!=nullptr){
        if(selectedObject != followedObject)
            {m_renderContext->currentCamera->newFollowObject(selectedObject);
}}

}