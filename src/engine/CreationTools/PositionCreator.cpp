#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/CreationTools/PositionCreator.h"
#include "engine/CreationTools/CreatorManager.h"
#include "engine/Vec3.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 
#include "engine/RenderTools/GlowTool.h"
#include "engine/RenderTools/athmosphereTool.h"
#include "engine/RenderTools/saturnRingTool.h"


PositionCreator::PositionCreator(RenderContext* renderContext, CreatorManager* manager) : StateCreator(renderContext, manager) {
    generate_buttons();
    generate_labels();
}

void PositionCreator::Enter(){
    m_manager->resetCamera();
    if(!(m_manager->isCreated)){createNewObject();}
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
    if (isHoveringRectangle()){shouldUpdate=false;}
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
}
void PositionCreator::generate_buttons(){
   Button *ReturnButton = new Button(0.05f, 0.57, ImVec2(0.045, 0.03),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.27f, 0.17f, 0.17f, 1.0f),
                               "Previous", 0.0,19.0f,
                               std::bind(&PositionCreator::previous_state, this),3); 

   Button *NextButton = new Button(0.1f, 0.57, ImVec2(0.04, 0.03),
                                ImVec4(0.1f, 0.1f, 0.1f, 1.0f),
                                ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                               "Select", 0.0,19.0f,
                               std::bind(&PositionCreator::next_state, this),3);  

   Button *AdjustPositionButton = new Button(0.075f, 0.5, ImVec2(0.04, 0.03),
                                ImVec4(0.05f, 0.05f, 0.05f, 1.0f),
                                ImVec4(0.27f, 0.27f, 0.17f, 1.0f),
                               "Adjust", 0.0,18.0f,
                               std::bind(&PositionCreator::AdjustPosition, this),3);
   Button *ZoomInButton = new Button(0.105f, 0.9, ImVec2(0.02, 0.02),
                                ImVec4(0.1f, 0.1f, 0.1f, 1.0f),
                                ImVec4(0.17f, 0.17f, 0.17f, 1.0f),
                               "+", 0.0,25.0f,
                               std::bind(&PositionCreator::zoomIn, this),3,false,ImVec4(255,255,255,255),true);  
    Button *ZoomOutButton = new Button(0.045f, 0.9, ImVec2(0.02, 0.02),
                                ImVec4(0.1f, 0.1f, 0.1f, 1.0f),
                                ImVec4(0.17f, 0.17f, 0.17f, 1.0f),
                            "-", 0.0,28.0f,
                            std::bind(&PositionCreator::zoomOut, this),3,false,ImVec4(255,255,255,255),true); 
    Button *ResetPosButton = new Button(0.075f, 0.97, ImVec2(0.04, 0.03),
                                ImVec4(0.1f, 0.1f, 0.1f, 1.0f),
                                ImVec4(0.27f, 0.27f, 0.17f, 1.0f),
                        "Reset", 0.0,19.0f,
                        std::bind(&PositionCreator::resetCamPos, this),3);
    buttons.push_back(ReturnButton);
    buttons.push_back(NextButton);
    buttons.push_back(AdjustPositionButton);
    buttons.push_back(ZoomInButton);
    buttons.push_back(ZoomOutButton);
    buttons.push_back(ResetPosButton);
    buttons[1]->UpdateLabelColor(75,230,125,200);
    buttons[0]->UpdateLabelColor(230,100,75,200);
    buttons[2]->UpdateLabelColor(230,230,125,200);
    buttons[0]->UpdatePosition(0.075,0.57);
    buttons[1]->hidden=true;
    buttons[2]->hidden=true;
    buttons[5]->UpdateLabelColor(230,230,125,200);
}

void PositionCreator::resetCamPos(){
    m_manager->resetCamera();
}
void PositionCreator::zoomIn(){
    Vec3 actualPos = m_renderContext->currentCamera->getPosition();
    if(actualPos.y>31.25){
    actualPos.y /= 1.01;}
    m_manager->cameraCreationPosition = actualPos;
    m_manager->updateCamera();
}
void PositionCreator::zoomOut(){
    Vec3 actualPos = m_renderContext->currentCamera->getPosition();
    if(actualPos.y<2000){
    actualPos.y *= 1.01;}
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
    Labbel *MessageLabel = new Labbel(0.075f,0.25f,ImVec4(255,255,255,255),
                            "Choose position",22.0f,0.9f);
    Labbel *DistanceStaticLabel = new Labbel(0.075f,0.35f,ImVec4(255,255,255,255),
                        "Distance from star",21.0f,0.7f);
    Labbel *DistanceLabel = new Labbel(0.075f,0.40f,ImVec4(150,255,220,200),
                    "2 AU",25.0f,0.8f);
    Labbel *CameraLabel = new Labbel(0.075f,0.85f,ImVec4(255,255,255,255),
                            "Camera",22.0f,0.9f);
    Labbel *ZoomLabel = new Labbel(0.075f,0.9f,ImVec4(255,255,255,255),
                        "Zoom",18.0f,0.9f);
    labbels.push_back(MessageLabel);
    labbels.push_back(DistanceStaticLabel);
    labbels.push_back(DistanceLabel);
    labbels.push_back(CameraLabel);
    labbels.push_back(ZoomLabel);
}
void PositionCreator::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;


    float longueur = winWidth* 0.14; // Exemple de taille
    float hauteur = winHeight * 0.40; // Exemple de taille

    centerPos = ImVec2(winWidth * 0.075f, winHeight * 0.4f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);   


    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(7.5, 7.5, 7.5, 175), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft,
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur),
                        IM_COL32(50, 50, 50, 0), // Couleur de remplissage
                        cornerRadius,0,1.5f); // Ajustez le rayon ici
    
 //Camera
    hauteur *= 0.45;
    centerPos = ImVec2(winWidth * 0.075f, winHeight * 0.9f);
    topLeftCamera = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);   
    bottomRightCamera = ImVec2(topLeftCamera.x + longueur, topLeftCamera.y + hauteur);  

    drawList->AddRectFilled(topLeftCamera, 
                        ImVec2(topLeftCamera.x + longueur, topLeftCamera.y + hauteur), 
                            IM_COL32(7.5, 7.5, 7.5, 175), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeftCamera,
                        ImVec2(topLeftCamera.x + longueur, topLeftCamera.y + hauteur),
                        IM_COL32(50, 50, 50, 0), // Couleur de remplissage
                        cornerRadius,0,1.5f); // Ajustez le rayon ici
}
bool PositionCreator::isHoveringRectangle(){
    return (ImGui::IsMouseHoveringRect(topLeft, bottomRight) || ImGui::IsMouseHoveringRect(topLeftCamera, bottomRightCamera) );
}

void PositionCreator::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
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
    labbels[2]->UpdateText(distance2String);  
}

void PositionCreator::next_state(){
    m_manager->ChangeState("VelocityCreator");
    m_manager->newCreatedObject->distance_initiale = m_manager->newCreatedObject->getRealPosition().norm();
}
void PositionCreator::previous_state(){
    m_manager->ChangeState("TextureCreator");
    removeNewObject();
}

void PositionCreator::Exit(){
    positionSelected = false;
    positionSelected = false;
    buttons[1]->hidden=true;
    buttons[2]->hidden=true;
    buttons[0]->UpdatePosition(0.075,0.57); 
} 

void PositionCreator::createNewObject(){
    CelestialObject* newObj = m_manager->newCreatedObject;
    if (newObj) {
        m_renderContext->systemeSolaire->addObject(newObj);
        m_renderContext->systemeSolaire->setRayon(newObj);
        newObj->distanceScale=m_renderContext->systemeSolaire->scale;
        newObj->isCreated=true;
        m_manager->isCreated=true;
        createEffects(); //Ajout du glow ou de l'athmosphere ou des anneaux
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
}

void PositionCreator::DrawOrbits() {
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

        ImU32 color = (object == m_manager->newCreatedObject) ? IM_COL32(100,255,150,130) : IM_COL32(255, 255, 255, 80);
        float tickness = (object == m_manager->newCreatedObject) ? winWidth * 0.002 : winWidth * 0.00075;
        int num_segments = 300; // Qualité du cercle
        if (object != m_manager->newCreatedObject) {
            drawList->AddCircle(center, radius, color, num_segments, tickness);
        }

        if (object == m_manager->newCreatedObject && (!isHoveringRectangle() || positionSelected)) {
            drawList->AddCircle(center, radius, color, num_segments, tickness);
            ImU32 arrowColor = IM_COL32(100,255,150,130);
            ImVec2 objectScreenPos = ImVec2(xPercent * winWidth, yPercent * winHeight);
            drawList->AddLine(center, objectScreenPos, arrowColor, winWidth * 0.002);
            ImU32 color = IM_COL32(100,255,150,255);
            drawList->AddCircleFilled(objectScreenPos, winWidth * 0.0025, color);
        }
    }
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
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !isHoveringRectangle()){
        positionSelected = true;
        buttons[0]->UpdatePosition(0.05,0.57);    
        buttons[1]->hidden=false;
        buttons[2]->hidden=false;
    }
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && positionSelected && !isHoveringRectangle()){
        AdjustPosition();
    }
}


void PositionCreator::AdjustPosition(){
    positionSelected = false;
    buttons[1]->hidden=true;
    buttons[2]->hidden=true;
    buttons[0]->UpdatePosition(0.075,0.57); 
}
