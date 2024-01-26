#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/CreationTools/PositionCreator.h"
#include "engine/CreationTools/CreatorManager.h"
#include "engine/Vec3.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 
#include <iomanip>
#include "engine/RenderTools/GlowTool.h"
#include "engine/RenderTools/athmosphereTool.h"
#include "engine/RenderTools/saturnRingTool.h"
#include "engine/RenderTools/uranusRingTool.h"
#include "engine/RenderTools/soundTool.h"
#include "path_util.h"
#include <random> // Include this for random number generation


PositionCreator::PositionCreator(RenderContext* renderContext, CreatorManager* manager) : StateCreator(renderContext, manager) {
    generate_buttons();
    generate_labels();
    generate_colors();
    nameFontBig = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 14.0);
    nameFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 12.0);

}

void PositionCreator::Enter(){
    CelestialObject* sun = m_renderContext->systemeSolaire->getSun();
    Vec3 positionCameraInit = Vec3(sun->getPositionSimulation().x,125,sun->getPositionSimulation().z);
    m_manager->cameraCreationPositionInit = Vec3(positionCameraInit);
    m_manager->cameraCreationTargetInit = sun->getPositionSimulation();
    m_manager->resetCamera();
    if(!(m_manager->isCreated) && !m_manager->isEditing){createNewObject();}
} 


void PositionCreator::Draw(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.3f, 0.4f, 0.3f)); // Exemple de couleur (bleu foncé)

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    checkInputs();
    DrawOrbits();
    drawBackground();
    draw_buttons();
    draw_labels();
    drawPlanets();
    drawControls();


    if (!isHoveringRectangle()){shouldUpdate=false;}
    else{shouldUpdate=true;}
    if(!positionSelected && shouldUpdate){updatePositionWithMouse();}
    checkClick();

    ImGui::End(); 
    ImGui::PopStyleColor();
} 
void PositionCreator::DrawOpenGL(){} 

void PositionCreator::checkInputs(){
    if (ImGui::IsKeyReleased(ImGuiKey_Escape)) {previous_state();}
    if (ImGui::IsKeyDown(ImGuiKey_E)) {zoomIn();}
    if (ImGui::IsKeyDown(ImGuiKey_Q)) {zoomOut();}
    if (ImGui::IsKeyDown(ImGuiKey_D)) {moveRight(true);}
    if (ImGui::IsKeyDown(ImGuiKey_A)) {moveRight(false);}
    if (ImGui::IsKeyDown(ImGuiKey_W)) {moveUp(true);}
    if (ImGui::IsKeyDown(ImGuiKey_S)) {moveUp(false);}
    if (ImGui::IsKeyPressed(ImGuiKey_R)) {resetCamPos();}


    if(ImGui::IsWindowHovered()){
        if(ImGui::GetIO().MouseWheel != 0){
            if(ImGui::GetIO().MouseWheel > 0 && isHoveringRectangle()){
                zoomIn(4.0);}

            else if (ImGui::GetIO().MouseWheel < 0 && isHoveringRectangle()){
                zoomOut(4.0);
            }
        }
    }
}
void PositionCreator::generate_buttons(){
       std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    float taille_x = 0.035f*1.0;
    float taille_y = 0.06f*1.0;


   Button *ReturnButton = new Button(playSoundFunc,0.13, 0.75, ImVec2(taille_x*1.2, taille_y*0.55),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Previous", 0.05,19.0f,
                               std::bind(&PositionCreator::previous_state, this),5); 

   Button *NextButton = new Button(playSoundFunc,0.165f, 0.75, ImVec2(taille_x*1.2, taille_y*0.55),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Select", 0.2,20.0f,
                               std::bind(&PositionCreator::next_state, this),5);  


    buttons.push_back(ReturnButton);
    buttons.push_back(NextButton);
    buttons[0]->UpdatePosition(0.13, 0.75);
    buttons[1]->hidden=true;
    //Image Buttons

    taille_x = 0.035f;
    taille_y = 0.06f;
    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    ImageButton* resetPosition = new ImageButton(playSoundFunc,0.075f, 0.45, ImVec2(taille_x, taille_y),0.5,
                        button_color,button_color,
                        "../assets/button/reset2.png", 0,std::bind(&PositionCreator::AdjustPosition,this),
                            3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    resetPosition->hidden=true;
    ImageButton* closeButton = new ImageButton(playSoundFunc,0.193878, 0.223069,ImVec2(0.025f,0.025f),0.6f,
                        button_color, button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&PositionCreator::forceClose, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
        
    icon = new Icon(0.0666667, 0.223069,ImVec2(0.025f,0.025f),0.35f,"../assets/button/position.png",0.85);


    imageButtons.push_back(resetPosition);
    imageButtons.push_back(closeButton);



    //Icons:

    Icon* reset = new Icon(0.08f, 0.57f,ImVec2(0.025f,0.025f),0.35f,"../assets/button/keys/r.png",0.75);
    Icon* wasd = new Icon(0.08f, 0.61f,ImVec2(0.025f,0.025f),0.35f,"../assets/button/keys/wasd.png",0.75);
    Icon* zoom = new Icon(0.08f, 0.65f,ImVec2(0.025f,0.025f),0.35f,"../assets/button/keys/scroll.png",0.75);
    Icon* mouse = new Icon(0.08f, 0.69f,ImVec2(0.025f,0.025f),0.35f,"../assets/button/keys/mouse.png",0.75);


    iconsControls.push_back(reset);
    iconsControls.push_back(wasd);
    iconsControls.push_back(zoom);
    iconsControls.push_back(mouse);
}

void PositionCreator::drawPlanets(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 centerPosPlanet = ImVec2(centerPos.x-winWidth*0.025,centerPos.y-winHeight*0.08);
    ImVec2 posSun = ImVec2(centerPosPlanet.x, centerPosPlanet.y*0.875);
    ImVec2 planetPos = ImVec2(centerPosPlanet.x, centerPosPlanet.y*1.125);
    float sunRadius = winWidth * 0.008;
    float planetRadius = winWidth * 0.008;

    CelestialObject* planet = m_manager->newCreatedObject;

    ImVec4 sunColor = typeDictColor[sun->typeName];
    ImVec4 planetColor = typeDictColor[planet->typeName];

    ImVec2 sunTextSize = ImGui::CalcTextSize(sun->getName().c_str());
    ImVec2 planetTextSize = ImGui::CalcTextSize(planet->getName().c_str());

    //Draw sun and planet
    drawList->AddLine(posSun, planetPos, IM_COL32(255, 255, 255, 100), winWidth * 0.001);
    drawList->AddCircleFilled(posSun, sunRadius, IM_COL32(sunColor.x, sunColor.y, sunColor.z, 255));
    drawList->AddCircleFilled(planetPos, planetRadius, IM_COL32(planetColor.x, planetColor.y, planetColor.z, 255));

    drawList->AddText(ImVec2(posSun.x - sunTextSize.x / 2, posSun.y - sunRadius*4), IM_COL32(255, 255, 255, 200), sun->getName().c_str());
    drawList->AddText(ImVec2(planetPos.x - planetTextSize.x / 2, planetPos.y + planetRadius*2), IM_COL32(255, 255, 255, 200), planet->getName().c_str());

    //Draw effects for bottomRight
    if(sun->type == 1){
        drawSunEffect(posSun, sunRadius);
    }
    if(planet->type != 1 && sun->type == 1){
        drawPlanetLightNOSHADOW(planetPos, planetRadius, planet);
    }else if(planet->type == 1){
        drawSunEffect(planetPos, planetRadius);
    }



}

void PositionCreator::resetCamPos(){
    m_manager->resetCamera();
}
void PositionCreator::zoomIn(float zoomFactor){
    Vec3 actualPos = m_renderContext->currentCamera->getPosition();
    if(actualPos.y>31.25){
    actualPos.y /= (1.0+0.01*zoomFactor); }
    m_manager->cameraCreationPosition = actualPos;
    m_manager->updateCamera();
}
void PositionCreator::zoomOut(float zoomFactor){
    Vec3 actualPos = m_renderContext->currentCamera->getPosition();
    if(actualPos.y<2500){
    actualPos.y *= (1.0+0.01*zoomFactor);}
    m_manager->cameraCreationPosition = actualPos;
    m_manager->updateCamera();
}
void PositionCreator::moveUp(bool up){
Vec3 actualPos = m_renderContext->currentCamera->getPosition();
Vec3 actualTarget = m_renderContext->currentCamera->getTarget();
    if(up){
        actualPos.z -= 0.5;
        actualTarget.z -= 0.5;
    }else{
        actualPos.z += 0.5;
        actualTarget.z += 0.5;
    }
m_manager->cameraCreationPosition = actualPos;
m_manager->cameraCreationTarget = actualTarget;
m_manager->updateCamera();
}
void PositionCreator::moveRight(bool right){
Vec3 actualPos = m_renderContext->currentCamera->getPosition();
Vec3 actualTarget = m_renderContext->currentCamera->getTarget();
    if(right){
        actualPos.x += 0.5;
        actualTarget.x += 0.5;
    }else{
        actualPos.x -= 0.5;
        actualTarget.x -= 0.5;
    }
m_manager->cameraCreationPosition = actualPos;
m_manager->cameraCreationTarget = actualTarget;
m_manager->updateCamera();
}

void PositionCreator::generate_labels(){
    Labbel *MessageLabel = new Labbel(0.13f,0.255f,ImVec4(255,255,255,255),
                            "Position",25.0f,0.9f);
    Labbel *DistanceStaticLabel = new Labbel(0.075f,0.35f,ImVec4(255,255,255,255),
                        "Distance from star",21.0f,0.7f);
    Labbel *DistanceLabel = new Labbel(0.155f,0.42f,ImVec4(255,255,255,150),
                    "2 AU",27.0f,0.8f);



    Labbel *reset = new Labbel(0.11, 0.57,ImVec4(255,255,255,255),
                            "Reset View",20.0f,0.7f);
    Labbel *wasdLabel = new Labbel(0.11, 0.61,ImVec4(255,255,255,255),
                            "Move Camera",20.0f,0.7f);
    Labbel *zoomLabel = new Labbel(0.11, 0.65,ImVec4(255,255,255,255),
                            "Zoom with scroll",20.0f,0.7f);
    Labbel *mouseLabel = new Labbel(0.11, 0.69,ImVec4(255,255,255,255),
                            "Click to Select/Unselect",18.0f,0.7f);   



    labbels.push_back(MessageLabel);
    labbels.push_back(DistanceLabel);
    labbels.push_back(reset);
    labbels.push_back(wasdLabel);
    labbels.push_back(zoomLabel);
    labbels.push_back(mouseLabel);

    reset->alignLeft = true;
    wasdLabel->alignLeft = true;
    zoomLabel->alignLeft = true;
    mouseLabel->alignLeft = true;
    //labbels.push_back(ZoomLabel);
}
void PositionCreator::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;

    centerPos = ImVec2(winWidth * 0.13f, winHeight * 0.5f);
    float longueur = winWidth* 0.16; // Exemple de taille
    float hauteur = winHeight * 0.60; // Exemple de taille

    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);   
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0, 0, 0, 255), // Couleur
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
bool PositionCreator::isHoveringRectangle(){

    // Position Y du bord supérieur du rectangle pour que le bas du rectangle soit au bas de la fenêtre
    float topY = centerPosMidSquare.y - hauteurMidSquare/2;
    float leftX = centerPosMidSquare.x - longueurMidSquare/2;
    float rightX = centerPosMidSquare.x + longueurMidSquare/2;
    float bottomY = centerPosMidSquare.y + hauteurMidSquare/2;
    ImVec2 topLeft = ImVec2(leftX, topY);
    ImVec2 bottomRight = ImVec2(rightX, bottomY);
    return (ImGui::IsMouseHoveringRect(topLeft, bottomRight));
}

bool PositionCreator::isWithinBounds(const ImVec2& objectPos, const ImVec2& objectSize) {
    // Calculate the corners of the mid square
    ImVec2 sizeMidSquare = ImVec2(longueurMidSquare, hauteurMidSquare);
    float topYMidSquare = centerPosMidSquare.y - sizeMidSquare.y / 2;
    float leftXMidSquare = centerPosMidSquare.x - sizeMidSquare.x / 2;
    float rightXMidSquare = centerPosMidSquare.x + sizeMidSquare.x / 2;
    float bottomYMidSquare = centerPosMidSquare.y + sizeMidSquare.y / 2;

    // Calculate the corners of the object
    float topYObject = objectPos.y - objectSize.y / 2;
    float leftXObject = objectPos.x - objectSize.x / 2;
    float rightXObject = objectPos.x + objectSize.x / 2;
    float bottomYObject = objectPos.y + objectSize.y / 2;

    // Check if the object is within the bounds of the mid square
    bool isWithinHorizontalBounds = leftXObject >= leftXMidSquare && rightXObject <= rightXMidSquare;
    bool isWithinVerticalBounds = topYObject >= topYMidSquare && bottomYObject <= bottomYMidSquare;

    return isWithinHorizontalBounds && isWithinVerticalBounds;
}
void PositionCreator::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
    for (ImageButton* btn : imageButtons) {
        btn->Draw();
    }
    icon->Draw();
    for(Icon* icon : iconsControls){
        icon->Draw();
    }
}

void PositionCreator::draw_labels(){
    updateDistanceLabel();
        for (Labbel* label : labbels) {
        label->Draw();
    }
}

void PositionCreator::updateDistanceLabel(){
    if(m_manager->newCreatedObject == nullptr){return;}
    double distance = m_manager->newCreatedObject->getPositionSimulation().norm();
    distance = distance/m_renderContext->systemeSolaire->scale;
    distance = distance/149597870e3;

    std::ostringstream distanceString;
    distanceString << std::fixed << std::setprecision(2) << distance << " AU";
    std::string distance2String = distanceString.str();
    labbels[1]->UpdateText(distance2String);  
}

void PositionCreator::next_state(){
    m_manager->ChangeState("VelocityCreator");
    m_manager->newCreatedObject->distance_initiale = m_manager->newCreatedObject->getRealPosition().norm();
}
void PositionCreator::previous_state(){
    if(m_manager->isEditing){
        m_manager->Exit();}
    else{
    m_manager->ChangeState("TextureCreator");
    removeNewObject();}
}

void PositionCreator::Exit(){
    positionSelected = false;
    positionSelected = false;
    buttons[1]->hidden=true;
    //imageButtons[0]->hidden=true;
    buttons[0]->UpdatePosition(0.13, 0.75);
} 

void PositionCreator::createNewObject(){
    CelestialObject* newObj = m_manager->newCreatedObject;
    if (newObj) {
        m_renderContext->systemeSolaire->addObject(newObj);
        m_renderContext->systemeSolaire->setRayon(newObj);
        newObj->distanceScale=m_renderContext->systemeSolaire->scale;
        newObj->isCreated=true;
        m_manager->isCreated=true;
    }
}


void PositionCreator::removeNewObject(){
    m_renderContext->systemeSolaire->removeObject(m_manager->newCreatedObject);
    m_manager->isCreated=false;
}


void PositionCreator::createEffects(){
    CelestialObject* newObj = m_manager->newCreatedObject;
    if(newObj->getName()=="Saturn"){
        newObj->saturnRingTool = new SaturnRingTool(newObj,m_renderContext);
    }else if(newObj->getName()=="Earth"){
        newObj->athmosphereTool = new AthmosphereTool(newObj,m_renderContext);}

    else if(newObj->getName()=="Sun"){
        newObj->glowTool = new GlowTool(newObj,m_renderContext);
    }
    else if(newObj->getName()=="Uranus"){
        newObj->uranusRingTool = new UranusRingTool(newObj,m_renderContext);
    }
}

void PositionCreator::drawControls(){

}
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void PositionCreator::drawOrbitGrids(const ImVec2& sunScreenPos, float maxRadius) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    int numGrids = 5; // Number of circular grids
    float spacing = maxRadius / numGrids; // Spacing between each grid

    for (int i = 1; i <= numGrids; ++i) {
        float radius = spacing * i;
        drawList->AddCircle(sunScreenPos, radius, IM_COL32(255, 255, 255, 50)); // Change color and alpha as needed
    }
}
void PositionCreator::DrawOrbits() {
    drawMainBackground();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    auto& objects = m_renderContext->systemeSolaire->objects;
    CelestialObject* centerObject = m_renderContext->systemeSolaire->getSun();
    sun = centerObject;
    // Obtenir la position du soleil (premier objet)
    const glm::vec3& sunPos3D = centerObject->getPositionSimulation().toGlm();

    // Calculer les coordonnées d'écran du soleil
    glm::mat4 viewMatrix = m_renderContext->currentCamera->getViewMatrix();
    glm::mat4 projectionMatrix = m_renderContext->currentCamera->getProjectionMatrix();
    glm::vec4 clipCoords = projectionMatrix * viewMatrix * glm::vec4(sunPos3D, 1.0f);
    glm::vec3 ndc = glm::vec3(clipCoords) / clipCoords.w;
    float xPercentSun = (ndc.x + 1.0f) / 2.0f;
    float yPercentSun = (1.0f - ndc.y) / 2.0f;

    ImVec2 center(xPercentSun* winWidth, yPercentSun * winHeight); // Utiliser les coordonnées d'écran du soleil comme centre

ImGui::PushFont(nameFont);
    // Parcourez tous les objets pour dessiner leurs orbites
    for (const auto& object : objects) {
        if(object->type == 5 && object != m_manager->newCreatedObject){continue;}
        if(!isHoveringRectangle() && object == m_manager->newCreatedObject && !positionSelected){continue;}
        glm::vec3 planetPos3D = object->getPositionSimulation().toGlm();

        // Calculer la position de la planète par rapport au soleil
        glm::vec3 relativePos = planetPos3D;

        // Convertir les coordonnées 3D en coordonnées de clip
        clipCoords = projectionMatrix * viewMatrix * glm::vec4(relativePos, 1.0f);
        ndc = glm::vec3(clipCoords) / clipCoords.w;

        // Convertir en coordonnées d'écran
        float xPercent = (ndc.x + 1.0f) / 2.0f;
        float yPercent = (1.0f - ndc.y) / 2.0f;

        ImVec2 planetScreenPos = ImVec2(xPercent * winWidth, yPercent * winHeight);
        float radius = std::sqrt(std::pow(center.x - planetScreenPos.x, 2) + std::pow(center.y - planetScreenPos.y, 2));

        float planetRadius = winWidth * 0.006;
        ImVec4 planetColor = typeDictColor[object->typeName];
        float distanceFromSun = pow(pow(planetScreenPos.x-center.x,2)+pow(planetScreenPos.y-center.y,2),0.5);
            if((object!=sun && object!= m_manager->newCreatedObject) && distanceFromSun<planetRadius*2){
                continue;}
        ImU32 color = (object == m_manager->newCreatedObject) ? IM_COL32(255, 255, 255,100) : IM_COL32(255, 255, 255, 20);
        float tickness = (object == m_manager->newCreatedObject) ? winWidth * 0.002 : winWidth * 0.001;

        ImVec2 textSize = ImGui::CalcTextSize(object->getName().c_str());
        float alphaText = (object == m_manager->newCreatedObject) ? 255 : 100;
        int segments = 200;
        float increment = 2.0f * M_PI / segments;
        ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
        ImVec4 colorOrbit = object == m_manager->newCreatedObject ? ImVec4(255, 255, 255, 150) : ImVec4(255, 255, 255, 100);

        for (int i = 0; i < segments; ++i) {
            // Calculez les points de début et de fin de chaque segment
            ImVec2 startPoint = ImVec2(cos(i * increment) * radius + center.x, sin(i * increment) * radius + center.y);
            ImVec2 endPoint = ImVec2(cos((i + 1) * increment) * radius + center.x, sin((i + 1) * increment) * radius + center.y);
            // Vérifiez si au moins un des points est à l'intérieur des limites de la fenêtre
            if (isWithinBounds(startPoint, ImVec2(1, 1)) && isWithinBounds(endPoint, ImVec2(1, 1))) {
            drawList->AddLine(startPoint, endPoint, IM_COL32(colorOrbit.x,colorOrbit.y,colorOrbit.z,colorOrbit.w), tickness);

            }}

        if(object == m_manager->newCreatedObject){
// Boucle pour dessiner la ligne droite de la planète au centre
for (int i = 0; i < segments; ++i) {
    float t1 = static_cast<float>(i) / segments;
    float t2 = static_cast<float>(i + 1) / segments;

    // Calculez les points de début et de fin de chaque segment
    ImVec2 startPoint = ImVec2(lerp(planetScreenPos.x, center.x, t1), lerp(planetScreenPos.y, center.y, t1));
    ImVec2 endPoint = ImVec2(lerp(planetScreenPos.x, center.x, t2), lerp(planetScreenPos.y, center.y, t2));

    // Vérifiez si au moins un des points est à l'intérieur des limites de la fenêtre
    if (isWithinBounds(startPoint, ImVec2(1, 1)) && isWithinBounds(endPoint, ImVec2(1, 1))) {
        drawList->AddLine(startPoint, endPoint, IM_COL32(colorOrbit.x, colorOrbit.y, colorOrbit.z, colorOrbit.w), tickness);
    }
}
        }


        if(isWithinBounds(planetScreenPos,ImVec2(planetRadius*2,planetRadius*2))){
        drawList->AddCircleFilled(planetScreenPos, planetRadius, IM_COL32(planetColor.x, planetColor.y, planetColor.z, 255));}
        
        ImVec2 textPosition = ImVec2(planetScreenPos.x - textSize.x / 2, planetScreenPos.y - planetRadius*4);
        if(isWithinBounds(ImVec2(textPosition.x+textSize.x/2,textPosition.y), textSize)){
        drawList->AddText(textPosition, IM_COL32(255, 255, 255, alphaText), object->getName().c_str());}
        //DrawEffects

        if(object->type == 1){
            if(isWithinBounds(planetScreenPos,ImVec2(planetRadius*1.16*2,planetRadius*1.16*2)))
            drawSunEffect(planetScreenPos, planetRadius);
        }else if(object->type != 1 && object->type != 0 && sun->type == 1){
            if(isWithinBounds(planetScreenPos,ImVec2(planetRadius*2,planetRadius*2))){
            drawPlanetLight(planetScreenPos, center, planetRadius);}
        }

    }
    ImGui::PopFont();
}

void PositionCreator::drawMainBackground(){
    longueurMidSquare = winWidth * 0.45; // Exemple de taille
    hauteurMidSquare = winHeight * 0.6; // Exemple de taille
    centerPosMidSquare = ImVec2(winWidth * 0.5f, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPosMidSquare.x - longueurMidSquare * 0.5f, centerPosMidSquare.y - hauteurMidSquare * 0.5f);
    float cornerRadius = 10.0f;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueurMidSquare, topLeft.y + hauteurMidSquare), 
                            IM_COL32(0,0,0, 255), // Couleur
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueurMidSquare, topLeft.y + hauteurMidSquare), 
                            IM_COL32(20, 25, 30, 100), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueurMidSquare, topLeft.y + hauteurMidSquare), 
                        IM_COL32(255,255,255,40), // Couleur
                        cornerRadius,0,0.2f);
}

Vec3 PositionCreator::calculateIntersection(const glm::vec3& rayDirection, const glm::vec3& cameraPosition) {
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
void PositionCreator::updatePositionWithMouse(){
    double mouseX, mouseY;
    mouseX = ImGui::GetMousePos().x;
    mouseY = ImGui::GetMousePos().y;


    float x = (2.0f * mouseX) / winWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / winHeight;
    glm::vec3 ray_nds = glm::vec3(x, y, 1.0f); 


    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);


    glm::vec4 ray_eye = glm::inverse(m_renderContext->currentCamera->getProjectionMatrix()) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);


    glm::vec3 ray_wor = glm::inverse(m_renderContext->currentCamera->getViewMatrix()) * ray_eye;
    ray_wor = glm::normalize(ray_wor);

    glm::vec3 cameraPosition = m_renderContext->currentCamera->getPosition().toGlm();
    Vec3 newSimulationPosition = calculateIntersection(ray_wor, cameraPosition);

    m_manager->newCreatedObject->updatePositionSimulation(newSimulationPosition);
}

void PositionCreator::checkClick(){
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !positionSelected && isHoveringRectangle()){
        //Position Sélectionnée
        positionSelected = true;
        buttons[0]->UpdatePosition(0.095, 0.75);  
        buttons[1]->hidden=false;
        //imageButtons[0]->hidden=false;
        return;
    }
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && positionSelected && isHoveringRectangle()){
        AdjustPosition();
    }
}


void PositionCreator::AdjustPosition(){
    positionSelected = false;
    buttons[1]->hidden=true;
    buttons[0]->UpdatePosition(0.13, 0.75);
    //imageButtons[0]->hidden=true;
}

void PositionCreator::forceClose(){
    m_manager->Exit();
}


void PositionCreator::generate_colors() {
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

void PositionCreator::drawSunEffect(ImVec2 planetPos, float radius){
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

void PositionCreator::drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, CelestialObject* obj){
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



void PositionCreator::draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments) {
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

void PositionCreator::drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius){
        ImVec2 directionToSun = ImVec2(planetPos.x - sunPos.x, planetPos.y - sunPos.y);
        float shadowAngle = atan2(directionToSun.y, directionToSun.x) + IM_PI; // Ajouter PI pour que l'ombre soit opposée au Soleil
        // Dessiner l'ombre comme un demi-cercle
        draw_half_circle_shadow(planetPos, radius, IM_COL32(0, 0, 0, 100), shadowAngle, 100);
}

void PositionCreator::cameraHauteurMode(){

}