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
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
}

void PlaneteInfoTool::initLabels(){
    float diff = 0.04f;
    float beg = 0.40f;
    float titleX = 0.82f;
    float middleX = 0.92;
    Labbel *nameLabbel = new Labbel(0.875,0.26f,ImVec4(255,255,255,255),
                            "Name",35.0f,0.9f);    

    Labbel *typeLabbel = new Labbel(0.875,0.30f,ImVec4(255,255,255,255),
                            "Type",23.0f,0.9f);  
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

    ImGui::End(); 
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
    glTranslatef(winWidth * 0.875f, winHeight * 0.32f,0);
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


