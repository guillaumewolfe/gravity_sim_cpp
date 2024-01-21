#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/CreationTools/VelocityCreator.h"
#include "engine/CreationTools/CreatorManager.h"
#include "engine/Vec3.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 
#include "engine/RenderTools/soundTool.h"
#include "path_util.h"



VelocityCreator::VelocityCreator(RenderContext* renderContext, CreatorManager* manager) : StateCreator(renderContext, manager) {
    generate_buttons();
    generate_labels();
    generate_sliders();
    generate_colors();
    nameFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 10.0);
    nameFontBig = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 14.0);
}

void VelocityCreator::Enter(){
    velocityValue =0.5;
    velocityAngle = 0;
    velocityAngleOffset = -2*M_PI;
    computeEscapeVelocitySpeed();
    m_renderContext->currentCamera->creationMode();
    m_manager->updateCamera();
    selectedObject = nullptr;
    isExited = false;
} 

void VelocityCreator::computeEscapeVelocitySpeed(){
/*1- Trouver M
  2- Définir G
  3- Trouver distance entre object et Soleil
*/
double mass = m_renderContext->systemeSolaire->objects[0]->getWeight();
const double G = 6.67430e-11;
double deltax = m_manager->newCreatedObject->getRealPosition().x - m_renderContext->systemeSolaire->objects[0]->getRealPosition().x;
double deltay = m_manager->newCreatedObject->getRealPosition().y - m_renderContext->systemeSolaire->objects[0]->getRealPosition().y;
double deltaz = m_manager->newCreatedObject->getRealPosition().z - m_renderContext->systemeSolaire->objects[0]->getRealPosition().z;
double puissance = deltax*deltax + deltay*deltay + deltaz*deltaz;
double distance = pow(puissance, 0.5);

escapeVelocity = pow(2*G*mass/distance, 0.5);
circularOrbitalVelocity = pow(G*mass/distance, 0.5);
}
void VelocityCreator::Draw(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    if (ImGui::IsKeyReleased(ImGuiKey_Escape)) {previous_state();}

    drawUI();
    draw_sliders();
    draw_labels();
    /*
    DrawOrbits();
    drawBackground();
    draw_buttons();
    draw_labels();
    draw_sliders();*/

    ImGui::End(); 
} 
void VelocityCreator::DrawOpenGL(){} 


void VelocityCreator::drawUI(){
    if(isExited){return;}
    float longueur = winWidth* 0.55; // Exemple de taille
    float hauteur = winHeight * 0.6; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0, 255), // Couleur
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(255,255,255,40), // Couleur
                        cornerRadius,0,0.2f);

    //Print mousePos % if clicked

    draw_buttons();
    drawPlanets();

}







void VelocityCreator::generate_buttons(){
           std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);

    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    float taille_x = 0.035f*1.0;
    float taille_y = 0.06f*1.0;


   Button *SaveButton = new Button(playSoundFunc,0.55, 0.765, ImVec2(taille_x*1.6, taille_y*0.65),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Select", 0.2f,20.0f,
                            std::bind(&VelocityCreator::next_state, this),5);   
    Button *Cancel = new Button(playSoundFunc,0.45, 0.765, ImVec2(taille_x*1.6, taille_y*0.65),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Previous", 0.05f,19.0f,
                        std::bind(&VelocityCreator::previous_state, this),5); 
    buttons.push_back(SaveButton);
    buttons.push_back(Cancel);

icon = new Icon(0.239456, 0.223069,ImVec2(0.025f,0.025f),0.35f,"../assets/button/speed.png",0.85);


    ImageButton* closeButton = new ImageButton(playSoundFunc,0.756782, 0.223069,ImVec2(0.025f,0.025f),0.6f,
                        button_color, button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&VelocityCreator::forceClose, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
        

    imageButtons.push_back(closeButton);
}

void VelocityCreator::generate_sliders(){
    float midx = 0.329932;
    Slider *VelocitySlider = new Slider(midx,0.43,ImVec2(0.14*0.8, 0.40*0.0175),ImVec4(100,100,100,125),
                                ImVec4(200,200,200,125),&velocityValue,ImVec2(0,1),20);

    sliders.push_back(VelocitySlider);


}


void VelocityCreator::generate_labels(){
    float midx = 0.329932;
    Labbel *MessageLabel = new Labbel(midx, 0.334059,ImVec4(255,255,255,255),
                            "Velocity:",26.0f,0.9f);
    
    Labbel *VelocityLabel = new Labbel(midx,0.398259,ImVec4(255,255,255,200),
                        "",26.0f,0.8f);
    
    Labbel *infoOrbitLabel = new Labbel(midx,0.45049,ImVec4(150,255,220,200),
                    "Stationary",18.0f,0.8f);

    Labbel *direction = new Labbel(midx,0.536453,ImVec4(255,255,255,200),
                    "Direction:",26.0f,0.8f);
    Labbel *minimap = new Labbel(0.6, 0.334059,ImVec4(255,255,255,255),
                            "Select an object:",26.0f,0.9f);
    labbels.push_back(MessageLabel);
    labbels.push_back(VelocityLabel);
    labbels.push_back(infoOrbitLabel);
    labbels.push_back(direction);
    labbels.push_back(minimap);
    Labbel* changeWeight = new Labbel(0.5,0.2525,ImVec4(255,255,255,255),
                "Select velocity",37,0.85f);

    labbels.push_back(changeWeight);




    valuesAngle.push_back(-2*M_PI);
    valuesAngle.push_back(-M_PI_2);
    valuesAngle.push_back(-M_PI);
    valuesAngle.push_back(-3*M_PI_2);
    valuesAngle.push_back(0);

}

float VelocityCreator::normalizeAngle(float angle) {
    while (angle > 2 * M_PI) angle -= 2 * M_PI;
    while (angle < -2 * M_PI) angle += 2 * M_PI;
    return angle;
}
void VelocityCreator::drawAngleSelector(CelestialObject* m_object, CelestialObject* sun, CelestialObject* selectedObject){
if(isExited){return;}
ImVec2 center = ImVec2(0.32517*winWidth, 0.655324*winHeight);
float angleObjectToSun = atan2(m_object->getRealPosition().z - sun->getRealPosition().z, m_object->getRealPosition().x - sun->getRealPosition().x);
float sizeBigCircle = winWidth*0.0475;
float posXSun = center.x + sizeBigCircle * cos(angleObjectToSun+M_PI);
float posYSun = center.y + sizeBigCircle * sin(angleObjectToSun+M_PI);
ImVec2 posSun(posXSun, posYSun);
ImVec2 posObject = center;

float angleObjectToSelected, posXSelected, posYSelected;
ImVec2 posSelectedObject;
if(selectedObject != nullptr){
    angleObjectToSelected = atan2(m_object->getRealPosition().z - selectedObject->getRealPosition().z, m_object->getRealPosition().x - selectedObject->getRealPosition().x);
    valuesAngle.push_back(angleObjectToSelected-velocityAngle-M_PI);
    posXSelected = center.x + sizeBigCircle * cos(angleObjectToSelected+M_PI);
    posYSelected = center.y + sizeBigCircle * sin(angleObjectToSelected+M_PI);
    posSelectedObject = ImVec2(posXSelected, posYSelected);
}

//Draw Sun And Object and big circle
ImVec4 colorObject = typeDictColor[m_object->typeName];
ImVec4 colorSun = typeDictColor[sun->typeName];
float radiusSun = winWidth * 0.006;
float radiusObject = winWidth * 0.006;

//Check mouse angle from m_object
ImVec2 mousePos = ImGui::GetMousePos();
float mouseAngle = atan2(mousePos.y - center.y, mousePos.x - center.x);

//Check if mouse in on circle (Center is center of big circle and size is sizeBigCircle)
float distanceMouseCenter = sqrt(pow(mousePos.x - center.x, 2) + pow(mousePos.y - center.y, 2));
bool mouseInCircle = distanceMouseCenter < sizeBigCircle;
//Check if mouse is in circle and is clicked
if (mouseInCircle && ImGui::IsMouseDown(0)) {
// Votre code existant avec des modifications pour la normalisation
velocityAngleOffset = normalizeAngle(mouseAngle - velocityAngle);
float threshold = 2 * M_PI / 50;

for (int i = 0; i < valuesAngle.size(); i++) {
    float normalizedValueAngle = normalizeAngle(valuesAngle[i]);
    if (abs(velocityAngleOffset - normalizedValueAngle) < threshold) {
        velocityAngleOffset = normalizedValueAngle;
    }
}
}


//Draw arrow
    float angle = velocityAngle + velocityAngleOffset; // The direction of the arrow
    float length = sizeBigCircle-radiusSun;// Length of the arrow

    // Calculate the end position of the arrow
    ImVec2 endPos = ImVec2(posObject.x + length * cos(angle), posObject.y + length * sin(angle));

    // Calculate the arrowhead points
    float arrowSize = radiusSun; // Size of the arrowhead

    ImVec2 arrowHeadPoints[3];
    arrowHeadPoints[0] = ImVec2(endPos.x + arrowSize * cos(angle + 5 * M_PI / 6), endPos.y + arrowSize * sin(angle + 5 * M_PI / 6));
    arrowHeadPoints[1] = endPos;
    arrowHeadPoints[2] = ImVec2(endPos.x + arrowSize * cos(angle - 5 * M_PI / 6), endPos.y + arrowSize * sin(angle - 5 * M_PI / 6));

    // Draw the line and the arrowhead
    drawList->AddLine(posObject, endPos, IM_COL32(255, 255, 255, 200),winWidth*0.0015); // White arrow line
    drawList->AddTriangleFilled(arrowHeadPoints[0], arrowHeadPoints[1], arrowHeadPoints[2], IM_COL32(255, 255, 255, 255)); // White arrowhead



ImGui::PushFont(nameFont);
ImVec2 textSizeSun = ImGui::CalcTextSize(sun->name.c_str());
ImVec2 textSizeObject = ImGui::CalcTextSize(m_object->name.c_str());

drawList->AddLine(posObject, posSun, IM_COL32(255, 255, 255, 30),winWidth*0.001);
drawList->AddCircleFilled(posSun, radiusSun, IM_COL32(colorSun.x, colorSun.y, colorSun.z, 200), 100);

drawList->AddCircleFilled(posObject, radiusObject, IM_COL32(colorObject.x, colorObject.y, colorObject.z, 255), 100);
drawList->AddCircle(center, sizeBigCircle, IM_COL32(255, 255, 255, 30), 100,winWidth*0.005);
drawList->AddText(ImVec2(posSun.x-textSizeSun.x/2, posSun.y - 3*radiusSun), IM_COL32(255, 255, 255, 200), sun->name.c_str());
drawList->AddText(ImVec2(posObject.x-textSizeObject.x/2, posObject.y - 3*radiusObject), IM_COL32(255, 255, 255, 200), m_object->name.c_str());

if(selectedObject != nullptr){
    ImVec4 colorObjectSelected = typeDictColor[selectedObject->typeName];
    drawList->AddCircleFilled(posSelectedObject, radiusObject, IM_COL32(colorObjectSelected.x, colorObjectSelected.y, colorObjectSelected.z, 255), 100);
    ImVec2 textSizeSelected = ImGui::CalcTextSize(selectedObject->name.c_str());
    drawList->AddText(ImVec2(posSelectedObject.x-textSizeSelected.x/2, posSelectedObject.y - 3*radiusObject), IM_COL32(255, 255, 255, 200), selectedObject->name.c_str());
    if(selectedObject->type==1){
        drawSunEffect(posSelectedObject, radiusObject);}
    else if(selectedObject->type!=0){
        drawPlanetLight(posSelectedObject, posSun, radiusObject);
    }
}


if(sun->type==1){
    drawSunEffect(posSun, radiusSun);}
else if (sun->type!=0){
    drawPlanetLight(posSun, center, radiusSun);
}

if(m_object->type==1){
    drawSunEffect(posObject, radiusObject);
}else if (m_object->type!=0){
    drawPlanetLight(posObject, posSun, radiusObject);
}

if(selectedObject){
    //remote last angle of valuesAngle
    valuesAngle.pop_back();
}

    ImGui::PopFont();
}


void VelocityCreator::drawPlanets(){
    if(isExited){return;}
    ImVec2 centerPosSystem = ImVec2(winWidth * 0.6f, winHeight * 0.55f);
    float maxSize = winWidth*0.125;
    float normalRadius = winWidth * 0.003;
    float mObjectRadius = normalRadius * 2.5;
    int index = 0;
    //drawList
    CelestialObject* sun = m_renderContext->systemeSolaire->getSun();
    CelestialObject* m_object = m_manager->newCreatedObject;

    calculateVelocityAngle(m_object, sun);
    drawAngleSelector(m_object, sun, selectedObject);


    std::vector<decltype(m_renderContext->systemeSolaire->objects)::value_type> sortedObjects(m_renderContext->systemeSolaire->objects.begin(), m_renderContext->systemeSolaire->objects.end());
    std::sort(sortedObjects.begin(), sortedObjects.end(),
        [sun](CelestialObject* a, CelestialObject* b) {
            auto distanceA = (sun->getRealPosition() - a->getRealPosition()).norm();
            auto distanceB = (sun->getRealPosition() - b->getRealPosition()).norm();
            return distanceA < distanceB;
        }
    );
    float increment = maxSize / sortedObjects.size();  // Incrément pour la distance
    ImVec2 sunPos;
    ImVec2 m_objectPos;

    //Check if mouse is on planet

    bool isThereSelection = false;
    for(auto& otherObject : sortedObjects){
        bool isObject = otherObject == m_object;
        bool isSoleil = otherObject == sun;
        float radiusPlanet;
        if(isObject or isSoleil){
            radiusPlanet = mObjectRadius;}
        else{radiusPlanet = normalRadius;}
    if(isObject){ImGui::PushFont(nameFontBig);}
    else{ImGui::PushFont(nameFont);}
        Vec3 objectDirection = (sun->getRealPosition() - otherObject->getRealPosition()).normalize();
        float objectAngle = atan2(objectDirection.z, objectDirection.x);
        float radius = index * increment;
        float x = centerPosSystem.x + radius * cos(objectAngle+M_PI);
        float y = centerPosSystem.y + radius * sin(objectAngle+M_PI);
        ImVec2 position(x, y);
        if(isSoleil){sunPos = position;}
        if(isObject){m_objectPos = position;}
        ImVec4 color = typeDictColor[otherObject->typeName];
        bool selected = checkSelection(m_object, sun, otherObject, position, radiusPlanet);

        if (selected && ImGui::IsMouseClicked(0)){
            selectedObject = otherObject;
            isThereSelection = true;
        }

        float alphaText = isObject ? 255 : 100;
        if(selected){alphaText = 200;}
        if(selectedObject == otherObject){
            alphaText = 255;
        }
        if(otherObject == sun){alphaText = 255;}


        ImVec2 textSize = ImGui::CalcTextSize(otherObject->name.c_str());
        drawList->AddCircleFilled(position, radiusPlanet, IM_COL32(color.x, color.y, color.z, 200), 100);
        drawList->AddCircle(ImVec2(centerPosSystem.x,centerPosSystem.y), radius, IM_COL32(255, 255, 255, 10), 100,winWidth*0.0005);
        float spacingRadius = isObject? 3 : 4;
        drawList->AddText(ImVec2(position.x-textSize.x/2, position.y - spacingRadius*radiusPlanet), IM_COL32(255, 255, 255, alphaText), otherObject->name.c_str());
        if(otherObject==m_object or otherObject == sun or otherObject == selectedObject){
            drawList->AddCircle(position, radiusPlanet, IM_COL32(255,255,255, 200), 100,winWidth*0.0005);
        }
        if(otherObject->type==1){
            drawSunEffect(position, radiusPlanet);
        }else{
            drawPlanetLight(position, centerPosSystem, radiusPlanet);
        }
        index++;
        ImGui::PopFont();
    }
    drawList->AddLine(sunPos, m_objectPos, IM_COL32(255, 255, 255, 20),winWidth*0.0015); // White arrow line
    ImVec2 mousePos = ImGui::GetMousePos();
    float distanceMouseCenter = sqrt(pow(mousePos.x - centerPosSystem.x, 2) + pow(mousePos.y - centerPosSystem.y, 2));
    bool mouseInCircle = distanceMouseCenter < maxSize;
    if(!isThereSelection && ImGui::IsMouseClicked(0) && mouseInCircle){
        selectedObject = nullptr;
    }


}


bool VelocityCreator::checkSelection(CelestialObject* m_object, CelestialObject* sun, CelestialObject* otherObject, ImVec2 position, float radius){
    //Check if mouse is on planet and planet is not m_object or sun
    ImVec2 mousePos = ImGui::GetMousePos();
    float distanceMouseCenter = sqrt(pow(mousePos.x - position.x, 2) + pow(mousePos.y - position.y, 2));
    bool mouseInCircle = distanceMouseCenter < radius*2;
    if(mouseInCircle && otherObject!=m_object && otherObject!=sun){
        return true;
    }
    return false;
    
}


void VelocityCreator::drawSunEffect(ImVec2 planetPos, float radius){
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

void VelocityCreator::drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius){
        ImVec2 directionToSun = ImVec2(planetPos.x - sunPos.x, planetPos.y - sunPos.y);
        float shadowAngle = atan2(directionToSun.y, directionToSun.x) + IM_PI; // Ajouter PI pour que l'ombre soit opposée au Soleil
        // Dessiner l'ombre comme un demi-cercle
        draw_half_circle_shadow(planetPos, radius, IM_COL32(0, 0, 0, 100), shadowAngle, 100);
}



void VelocityCreator::draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments) {
    // Calcule les angles pour le demi-cercle d'ombre
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    radius*=1.1;
    float start_angle = angle - IM_PI / 2-M_PI; // Commence à 90 degrés par rapport à l'angle d'ombre
    float end_angle = angle + IM_PI / 2-M_PI;   // Termine à 270 degrés par rapport à l'angle d'ombre

    // Début du chemin par le centre
    drawList->PathLineTo(center);

    // Ajout de l'arc pour le demi-cercle
    for (int i = 0; i <= num_segments; ++i) {
        float segment_angle = start_angle + (float(i) / float(num_segments)) * (end_angle - start_angle);
        ImVec2 vertex = ImVec2(center.x + cos(segment_angle) * radius, center.y + sin(segment_angle) * radius);
        drawList->PathLineTo(vertex);
    }

    // Fermeture du chemin en revenant au centre
    drawList->PathLineTo(center);

    // Remplissage du demi-cercle
    drawList->PathFillConvex(color);
}





void VelocityCreator::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;


    float longueur = winWidth* 0.14; // Exemple de taille
    float hauteur = winHeight * 0.40; // Exemple de taille

    centerPos = ImVec2(winWidth * 0.075f, winHeight * 0.4f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0,255), // Couleur
                            cornerRadius);

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 200), // Couleur
                            cornerRadius);
    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(255,255,255,40), // Couleur
                        cornerRadius,0,0.2f);

}

void VelocityCreator::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
    for(ImageButton* btn : imageButtons){
        btn->Draw();
    }
    icon->Draw();
}

void VelocityCreator::draw_sliders(){
        for (Slider* slider : sliders) {
        slider->Draw();
    }

}


void VelocityCreator::draw_labels(){
    updateVelocityLabel();
        for (Labbel* label : labbels) {
        label->Draw();
    }
}

void VelocityCreator::updateVelocityLabel(){
    finalVelocityValue = velocityValue * 1.3*escapeVelocity;
    double velocity = finalVelocityValue/1000;
    std::ostringstream velocityLabelString;
    velocityLabelString << std::fixed << std::setprecision(2) << velocity << " km/s";
    std::string VeloString = velocityLabelString.str();
    labbels[1]->UpdateText(VeloString); 

    std::string velocityCategory;
    ImVec4 labbelColor;
    
    double circularVelocityLower = 0.9 * circularOrbitalVelocity;
    double circularVelocityUpper = 1.1 * circularOrbitalVelocity;
    double escapeVelocityLower = 0.9 * escapeVelocity;
    double escapeVelocityUpper = 1.1 * escapeVelocity;

    // Series of conditions
    if (finalVelocityValue <= 0) {
        velocityCategory = "Stationary";
            labbelColor =ImVec4(255,100,100,255);
    } else if (finalVelocityValue < circularVelocityLower) {
        velocityCategory = "Low velocity";
            labbelColor =ImVec4(200,75,75,200);
    } else if (finalVelocityValue >= circularVelocityLower && finalVelocityValue < circularVelocityUpper) {
        velocityCategory = "Circular orbit";
        labbelColor = ImVec4(100,255,150,200);
    } else if (finalVelocityValue >= circularVelocityUpper && finalVelocityValue < escapeVelocityLower) {
        velocityCategory = "Elliptical orbit";
        labbelColor = ImVec4(100,150,255,200);
    } else if (finalVelocityValue >= escapeVelocityLower && finalVelocityValue < escapeVelocityUpper) {
        velocityCategory = "Near escape velocity";
        labbelColor = ImVec4(200,200,150,180);
    } else if (finalVelocityValue >= escapeVelocityUpper) {
        velocityCategory = "Escape trajectory";
        labbelColor = ImVec4(255,255,175,255);
    } else {
        // This else case is technically not needed as all scenarios are covered above
        velocityCategory = "Special case";
    }
    labbels[2]->UpdateText(velocityCategory);
    labbels[2]->UpdateColor(labbelColor);
}

void VelocityCreator::next_state(){
    setNewObjVelocity();
    if(m_manager->isEditing){
        m_manager->isEditingConfirmed = true;
        m_manager->Exit();
        return;
    }
    m_manager->ChangeState("ParametersCreator");
}
void VelocityCreator::previous_state(){
    m_manager->ChangeState("PositionCreator");
}
void VelocityCreator::setNewObjVelocity(){
    Vec3 newVel;
    newVel.x = finalVelocityValue * cos(velocityAngle+velocityAngleOffset);
    newVel.z = finalVelocityValue * sin(velocityAngle+velocityAngleOffset);
    newVel.y=0;
    m_manager->newCreatedObject->updateVelocity(newVel);
}


void VelocityCreator::Exit(){

} 

void VelocityCreator::calculateVelocityVector(){
    vectorLength = winWidth*0.001*velocityValue*100;

}
void VelocityCreator::DrawOrbits() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    auto& objects = m_renderContext->systemeSolaire->objects;

    // Obtenir la position du soleil (premier objet)
    const glm::vec3& sunPos3D = objects[0]->getPositionSimulation().toGlm();

    // Calculer les coordonnées d'écran du soleil
    glm::mat4 viewMatrix = m_renderContext->currentCamera->getViewMatrix();
    glm::mat4 projectionMatrix = m_renderContext->currentCamera->getProjectionMatrix();
    glm::vec4 clipCoords = projectionMatrix * viewMatrix * glm::vec4(sunPos3D, 1.0f);
    glm::vec3 ndc = glm::vec3(clipCoords) / clipCoords.w;
    float xPercentSun = (ndc.x + 1.0f) / 2.0f;
    float yPercentSun = (1.0f - ndc.y) / 2.0f;

    ImVec2 center(xPercentSun * winWidth, yPercentSun * winHeight); // Utiliser les coordonnées d'écran du soleil comme centre

    // Parcourez tous les objets pour dessiner leurs orbites
    for (const auto& object : objects) {
        glm::vec3 planetPos3D = object->getPositionSimulation().toGlm();

        // Calculer la position de la planète par rapport au soleil
        glm::vec3 relativePos = planetPos3D - sunPos3D;

        // Convertir les coordonnées 3D en coordonnées de clip
        clipCoords = projectionMatrix * viewMatrix * glm::vec4(relativePos, 1.0f);
        ndc = glm::vec3(clipCoords) / clipCoords.w;

        // Convertir en coordonnées d'écran
        float xPercent = (ndc.x + 1.0f) / 2.0f;
        float yPercent = (1.0f - ndc.y) / 2.0f;

        ImVec2 planetScreenPos = ImVec2(xPercent * winWidth, yPercent * winHeight);
        float radius = std::sqrt(std::pow(center.x - planetScreenPos.x, 2) + std::pow(center.y - planetScreenPos.y, 2));

        ImU32 color = (object == m_manager->newCreatedObject) ? IM_COL32(100, 255, 150, 120) : IM_COL32(255, 255, 255, 80);
        float tickness = (object == m_manager->newCreatedObject) ? winWidth * 0.002 : winWidth * 0.00075;
        int num_segments = 300; // Qualité du cercle
        drawList->AddCircle(center, radius, color, num_segments, tickness);

        // Si l'objet est le nouvel objet créé, dessinez la flèche de vitesse
        if (object == m_manager->newCreatedObject) {
            // Calculer l'angle de la vitesse
            glm::vec3 sunPos = objects.front()->getPositionSimulation().toGlm();
            float dx = planetPos3D.x - sunPos.x;
            float dz = planetPos3D.z - sunPos.z;
            float angleToSun = std::atan2(dz, dx);
            velocityAngle = M_PI + angleToSun + M_PI / 2; // Angle tangentiel
            if(vectorLength==0){continue;}
            // Convertir l'angle en vecteur de direction
            Vec3 velocityDirection(std::cos(velocityAngle+velocityAngleOffset), 0, std::sin(velocityAngle+velocityAngleOffset));
            float arrowLength = vectorLength; // Longueur de la flèche (modifiable)
            float minArrowLength = 10.0f; // Définir une longueur minimale pour la flèche

            // Assurez-vous que la flèche a une longueur minimale
            arrowLength = std::max(arrowLength, minArrowLength);

            // Taille de la pointe de la flèche
            float arrowHeadSize = std::min(15.0f, arrowLength / 2); // Ajuster la taille de la pointe
            float lineShortening = arrowHeadSize * std::sqrt(3) / 2;
            
            ImVec2 velocityLineEndPos = ImVec2(
                planetScreenPos.x + velocityDirection.x * (arrowLength - lineShortening),
                planetScreenPos.y + velocityDirection.z * (arrowLength - lineShortening)
            );

            // Dessinez la ligne de la flèche
            drawList->AddLine(planetScreenPos, velocityLineEndPos, IM_COL32(100,255,150,190), 3.0f);

            // Inversez la direction pour les points de la pointe de la flèche
            float pointBaseAngle = velocityAngle+velocityAngleOffset + M_PI; // Ajouter π pour inverser la direction de la pointe

            // Angle pour les points de la base de la pointe de la flèche
            float angle1 = pointBaseAngle + M_PI / 6; // 30 degrés de chaque côté de la pointe inversée
            float angle2 = pointBaseAngle - M_PI / 6;

            // Calculez les positions des points de la base de la pointe de la flèche
            ImVec2 point1 = ImVec2(velocityLineEndPos.x + std::cos(angle1) * arrowHeadSize, velocityLineEndPos.y + std::sin(angle1) * arrowHeadSize);
            ImVec2 point2 = ImVec2(velocityLineEndPos.x + std::cos(angle2) * arrowHeadSize, velocityLineEndPos.y + std::sin(angle2) * arrowHeadSize);

            // Utilisez ces points pour dessiner la pointe de la flèche
            drawList->AddTriangleFilled(point1, point2, velocityLineEndPos, IM_COL32(100,255,150,255));
        }
    }
}

void VelocityCreator::calculateVelocityAngle(CelestialObject* planet, CelestialObject* sun) {
    if(isExited){return;}
    // Calculer la direction de la planète par rapport au soleil
    glm::vec3 planetPos3D = planet->getRealPosition().toGlm();
    glm::vec3 sunPos3D = sun->getRealPosition().toGlm();
    glm::vec3 directionToSun = planetPos3D - sunPos3D;

    // Calculer l'angle de la direction
    float angle = atan2(directionToSun.z, directionToSun.x);
    velocityAngle = M_PI + angle + M_PI / 2;
}



Vec3 VelocityCreator::calculateIntersection(const glm::vec3& rayDirection, const glm::vec3& cameraPosition) {
    // Si le rayon pointe vers le haut, pas d'intersection avec le plan XZ
    if (rayDirection.y >= 0) {
        return Vec3(0, 0, 0);
    }

    // Calculer le facteur t pour l'intersection avec le plan XZ
    float t = -cameraPosition.y / rayDirection.y;

    // Calculer le point d'intersection
    glm::vec3 intersection = cameraPosition + t * rayDirection;

    // Retourner l'intersection avec le plan XZ (y = 0)
    return Vec3(intersection.x, 0, intersection.z);
}

void VelocityCreator::forceClose(){
    isExited = true;
    m_manager->Exit();
}


void VelocityCreator::generate_colors() {
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