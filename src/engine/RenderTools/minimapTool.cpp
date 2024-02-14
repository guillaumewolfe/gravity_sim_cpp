
#include "glad/glad.h"
#include "engine/RenderTools/minimapTool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include "engine/RenderTools/soundTool.h"
#include "path_util.h"


MinimapTool::MinimapTool(RenderContext* renderContext) : RenderComponent(renderContext){
    typeDict = m_renderContext->colorByTypeDict;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    longueur = winHeight* 0.295; // Exemple de taille
    hauteur = winHeight * 0.295; // Exemple de taille
    scale = 0.95;
    longueurScene = longueur * scale;
    hauteurScene = hauteur * scale;
    initialCenterX = 0.0825f;
    initialCenterY = 0.845;
    float fontsize = 17.0f;
    float fontSizeScaled = fontsize * winWidth / 1920;
    nameFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaled);
    generate_asteroids();
    generate_colors();
    generate_buttons();
}
void MinimapTool::Open(){
    selectedObject = nullptr;
    selected = false;
    deplacement_X = initialCenterX;
    deplacement_Y = initialCenterY;
    zoom = 1;
    asteroid1Positions = originalAsteroid1Positions;
    asteroid2Positions = originalAsteroid2Positions;
}

void MinimapTool::Draw() {
glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

updateButtonState();
check_inputs();
draw_rect();
draw_Scene();
draw_UI();
if(cameraTransition){transitionCamera();}
ImGui::End(); 


}

//destructeur
MinimapTool::~MinimapTool(){
    for(auto& button : imageButtons){
        delete button;
    }
    delete iconCamera;
}

void MinimapTool::generate_buttons(){
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
    ImVec4 buttonColor = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    ImageButton* resetCam = new ImageButton(playSoundFunc,initialCenterX-0.85*longueur/(2*winWidth),initialCenterY-0.85*hauteur/(2*winHeight),ImVec2(0.04f,0.04f),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/changePosition.png", 0,
                            std::bind(&MinimapTool::resetCamera, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    imageButtons.push_back(resetCam);

    Icon* camera = new Icon(0.50+0.96*longueur/(2*winWidth),0.5-0.96*hauteur/(2*winHeight),ImVec2(0.02f,0.02f),0.35f,"../assets/button/pin.png",0.5);
    iconCamera = camera;    
}

void MinimapTool::updateButtonState(){
    //reset camera
    if(deplacement_X != initialCenterX or deplacement_Y != initialCenterY or zoom != 1){
        imageButtons[0]->hidden = false;
    }
    else{
        imageButtons[0]->hidden = true;
    }
    if(selectedObject != m_renderContext->currentCamera->selectedObject){
        selectedObject = m_renderContext->currentCamera->selectedObject;
    }
}
void MinimapTool::draw_rect(){

    ImVec2 centerPos = ImVec2(winWidth * initialCenterX, winHeight * initialCenterY);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0, 0, 0, 20), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 100), // Color
                            cornerRadius);

    drawList->AddRect(topLeft, 
                      ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                      IM_COL32(255, 255, 255, 40), // Color
                      cornerRadius, 0, 0.2f);
}




void MinimapTool::draw_UI(){
    for(auto& button : imageButtons){
        button->Draw();
    }
}

void MinimapTool::draw_Scene(){
    ImGui::PushFont(nameFont);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    scale = 0.95;
    longueurScene = longueur * scale;
    hauteurScene = hauteur * scale;
    ImVec2 topLeft = ImVec2(centerPos.x - longueurScene * 0.5f, centerPos.y - hauteurScene * 0.5f);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueurScene * 0.5f, centerPos.y + hauteurScene * 0.5f);
    setup_dimension();
    draw_planets();
    draw_camera();
    draw_asteroid_belt(asteroid1Positions,winHeight*0.0010, IM_COL32(180, 150, 150, 50), -7.14e-9);
    draw_asteroid_belt(asteroid2Positions,winHeight*0.0010, IM_COL32(150, 200, 255, 50), -1.25e-10);
    draw_text();
    ImGui::PopFont();
}

void MinimapTool::setup_dimension(){
    radius = winHeight * 0.004 *zoom;
    minDistance = std::numeric_limits<float>::max();
    scaledDistance = hauteurScene / m_renderContext->systemeSolaire->maxSize;
    logBase = 1.1*zoom; // Base logarithmique pour la compression des distances
    scaleAfterLog = 21.694*zoom; // Facteur d'échelle après le calcul logarithmique
    centerPos = ImVec2(winWidth * deplacement_X, winHeight * deplacement_Y);
    // Calculer la position initiale du centre (premier objet)
    const auto& centreObjet = m_renderContext->systemeSolaire->getSun();
    ImVec2 initialCentrePos = ImVec2(centreObjet->getPositionSimulation().x * scaledDistance,
                                     centreObjet->getPositionSimulation().z * scaledDistance);
    double centreDistance = std::sqrt(std::pow(initialCentrePos.x, 2) + std::pow(initialCentrePos.y, 2));
    double compressedCentreDistance = std::log(std::max(centreDistance, 1.0) * logBase) * scaleAfterLog;
    ImVec2 compressedCentrePos = ImVec2(initialCentrePos.x / centreDistance * compressedCentreDistance,
                                        initialCentrePos.y / centreDistance * compressedCentreDistance);

    // Décalage à appliquer à toutes les planètes pour centrer le système
    offset = ImVec2(centerPos.x - compressedCentrePos.x, centerPos.y - compressedCentrePos.y);
}

void MinimapTool::draw_planets(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    for(auto& planete : m_renderContext->systemeSolaire->objects){
        if(planete->type == 5 && planete != m_renderContext->currentCamera->selectedObject) continue; // Ne pas dessiner le centre (par exemple, le Soleil
    ImVec2 planetPos = distanceConverterSimToLog(ImVec2(planete->getPositionSimulation().x, planete->getPositionSimulation().z));



    if (isWithinBounds(planetPos, ImVec2(radius, radius))) {
        float distanceToCenter = std::sqrt(std::pow(planetPos.x - centerPos.x, 2) + std::pow(planetPos.y - centerPos.y, 2));
        std::string planetTypeName = planete->typeName; // Assurez-vous que getName() renvoie une chaîne de caractères
        ImVec4 color = typeDictColor[planetTypeName]; // Utiliser le nom pour accéder à la couleur
        // Dessiner la planète
        drawList->AddCircleFilled(planetPos, radius, IM_COL32(color.x, color.y, color.z, 255),100);
    int segments = 300;
    float increment = 2.0f * M_PI / segments;

    for (int i = 0; i < segments; ++i) {
        // Calculez les points de début et de fin de chaque segment
        ImVec2 startPoint = ImVec2(cos(i * increment) * distanceToCenter + centerPos.x, sin(i * increment) * distanceToCenter + centerPos.y);
        ImVec2 endPoint = ImVec2(cos((i + 1) * increment) * distanceToCenter + centerPos.x, sin((i + 1) * increment) * distanceToCenter + centerPos.y);
        // Vérifiez si au moins un des points est à l'intérieur des limites de la fenêtre
        if (isWithinBounds(startPoint, ImVec2(1, 1)) && isWithinBounds(endPoint, ImVec2(1, 1))) {
        drawList->AddLine(startPoint, endPoint, IM_COL32(color.x, color.y, color.z, 20), radius/2);

        }
        
    }
        //Soleil
        checkMouseSelection(planetPos, planete);

        if(planete->type == 1){
            drawSunEffect(planetPos);
        }
        else if(planete->type != 0){
            drawPlanetLight(planetPos);
        }
}}
}
void MinimapTool::drawSunEffect(ImVec2 planetPos){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec4 colorCenterDot = ImVec4(255,215,80,255);
    float numBlurCircles = 40;
    float blurIncrease = radius*0.04; // How much larger each successive blur circle is
    float initialAlpha = 20; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        if(isWithinBounds(planetPos,ImVec2(blurRadius,blurRadius))){
        drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);}
    }
}
void MinimapTool::drawPlanetLight(ImVec2 planetPos){
        ImVec2 directionToSun = ImVec2(planetPos.x - centerPos.x, planetPos.y - centerPos.y);
        float shadowAngle = atan2(directionToSun.y, directionToSun.x) + IM_PI; // Ajouter PI pour que l'ombre soit opposée au Soleil
        // Dessiner l'ombre comme un demi-cercle
        draw_half_circle_shadow(planetPos, radius, IM_COL32(0, 0, 0, 180), shadowAngle, 100);
}


void MinimapTool::draw_camera(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 cameraSimPosition = ImVec2(
        m_renderContext->currentCamera->getPosition().x,
        m_renderContext->currentCamera->getPosition().z
    );
    ImVec2 finalCameraPos = distanceConverterSimToLog(cameraSimPosition);
    iconCamera->UpdatePosition(finalCameraPos.x/winWidth,finalCameraPos.y/winHeight);
    // Texte à afficher au-dessus de l'icône de la caméra
    std::string cameraText = "You";

    // Calculer la position du texte
    float textOffsetY = iconCamera->getSize().y*1.3*winHeight; // Ajustez cette valeur selon vos besoins
    ImVec2 textPos = ImVec2(finalCameraPos.x - ImGui::CalcTextSize(cameraText.c_str()).x / 2, 
                            finalCameraPos.y - textOffsetY);
    float textSize = ImGui::CalcTextSize(cameraText.c_str()).x;
    // Vérifier si le texte est dans les limites de la fenêtre
    float iconSize = iconCamera->getSize().x*winWidth*0.35f;
    if (isWithinBounds(textPos, ImVec2(textSize, 0))) {
        // Dessiner le texte
        drawList->AddText(textPos, IM_COL32(220, 255, 220, 200), cameraText.c_str());
    }
    if(isWithinBounds(finalCameraPos,ImVec2(iconSize/2,iconSize/2))){
        iconCamera->Draw();
    }
}

void MinimapTool::draw_text(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    for(auto& planete : m_renderContext->systemeSolaire->objects) {
        std::string planetName = planete->getName();
        ImVec4 color = typeDictColor[planetName];
        if(planete->type == 5 && planete != m_renderContext->currentCamera->selectedObject) continue; // Ne pas dessiner le centre (par exemple, le Soleil
        // Calculer la position finale avec le décalage
        ImVec2 planetPos = distanceConverterSimToLog(ImVec2(planete->getPositionSimulation().x, planete->getPositionSimulation().z));
        float distanceToCenter = std::sqrt(std::pow(planetPos.x - centerPos.x, 2) + std::pow(planetPos.y - centerPos.y, 2));
        // Déterminer l'opacité du texte
        float alpha = (planetName == closestPlanetName) ? 175 : 100;
        float alphaCircle = (planetName == closestPlanetName) ? 50 : 0;
        if(planete==selectedObject){
            alpha=255;
            alphaCircle=150;
        }
        // Position et dessin du texte
        ImVec2 textPos = ImVec2(planetPos.x - ImGui::CalcTextSize(planetName.c_str()).x / 2, 
                                planetPos.y - radius - winHeight*0.025);
        ImVec2 textSize = ImGui::CalcTextSize(planetName.c_str());

        // Vérifier si la position du texte est dans les limites de la minimap
        if (isWithinBounds(textPos, textSize)) {
        drawList->AddText(textPos, IM_COL32(255, 255, 255, alpha), planetName.c_str());
        drawList->AddCircle(planetPos, radius*1.2, IM_COL32(255,255,255,alphaCircle),100,winWidth*0.001);}// Assurez-vous que la méthode getName() existe
    }
}





void MinimapTool::generate_asteroids() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    float scale = hauteurScene*0.9 / 30.0f; // Convertir les unités astronomiques en pixels

    // Ceinture Principale d'Astéroïdes
    float rayonInterieur1 = 18.0f * scale/2;
    float rayonExterieur1 = 20.0f * scale/2;
    int numAsteroides1 = 1000;
    generate_asteroid_belt(gen, dis, rayonInterieur1, rayonExterieur1, numAsteroides1, asteroid1Positions);
    originalAsteroid1Positions = asteroid1Positions; // Stocker les positions originales
    // Ceinture de Kuiper
    float rayonInterieur2 = 30.0f * scale/2; // Commence à la position de Neptune
    float rayonExterieur2 = 33.0f * scale/2; // S'étend au-delà de Neptune
    int numAsteroides2 = 700;
    generate_asteroid_belt(gen, dis, rayonInterieur2, rayonExterieur2, numAsteroides2, asteroid2Positions);
    originalAsteroid2Positions = asteroid2Positions; // Stocker les positions originales
}

void MinimapTool::generate_asteroid_belt(std::mt19937& gen, std::uniform_real_distribution<>& dis, float rayonInterieur, float rayonExterieur, int numAsteroides, std::vector<ImVec2>& asteroidPositions) {
    for (int i = 0; i < numAsteroides; ++i) {
        float angle = dis(gen) * 360.0f;
        float rayon = rayonInterieur + dis(gen) * (rayonExterieur - rayonInterieur);
        float x = rayon * cos(angle * M_PI / 180.0f);
        float y = rayon * sin(angle * M_PI / 180.0f);
        asteroidPositions.push_back(ImVec2(x, y));
    }
}

void MinimapTool::draw_asteroid_belt(const std::vector<ImVec2>& asteroidPositions, float tailleAsteroide, ImU32 color, float rotation_par_seconde) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float time = *(m_renderContext->simulationTime);
    
    // Angle de rotation total basé sur le temps de simulation et la rotation par seconde
    float angle_total = rotation_par_seconde * time * 2.0f * M_PI;

    for (const ImVec2& pos : asteroidPositions) {
        // Rotation de l'astéroïde
        float x_rotated = cos(angle_total) * pos.x - sin(angle_total) * pos.y;
        float y_rotated = sin(angle_total) * pos.x + cos(angle_total) * pos.y;

        // Dessiner l'astéroïde à sa nouvelle position
        if(isWithinBounds(ImVec2(x_rotated + centerPos.x, y_rotated + centerPos.y), ImVec2(tailleAsteroide*2,tailleAsteroide*2))){
        drawList->AddCircleFilled(ImVec2(x_rotated + centerPos.x, y_rotated + centerPos.y), tailleAsteroide, color);}
    }
}

ImVec4 MinimapTool::getTypeColor(int type) {
    return typeDict[type].second;
}

void MinimapTool::generate_colors() {
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

void MinimapTool::draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments) {
    // Calcule les angles pour le demi-cercle d'ombre
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    CelestialObject* sun = nullptr;
    for(auto& object : m_renderContext->systemeSolaire->objects){
        if(object->type == 1){
            sun = object;
            break;
        }
    }
    if (sun == nullptr){
        drawList->AddCircleFilled(center, radius, color, num_segments);
    }
    else{
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
    drawList->PathFillConvex(color);}
}

void MinimapTool::drawGravityField(CelestialObject* object, ImVec2 position, float radius, float scale) {

}




void MinimapTool::setCloseButtonFunction(const std::function<void()>& func) {
    closeFunction = func;
}

void MinimapTool::CloseButton(){
    selectedObject = nullptr;
    closeFunction();
}

void MinimapTool::SelectObject(CelestialObject* object) {
    if(object == nullptr) {return;}
    m_renderContext->currentCamera->newFollowObject(object);
    *(m_renderContext->showInfo) = true;
}

void MinimapTool::check_inputs(){
    isMouseClicked = false;
    isMouseDoubleClicked = false;
    if (ImGui::IsKeyReleased(ImGuiKey_Enter)){SelectObject(selectedObject);}
    mousePos = ImGui::GetMousePos();
    isMouseClicked = ImGui::IsMouseClicked(0); // Vérifie si le bouton gauche de la souris a été cliqué
    isMouseDoubleClicked = ImGui::IsMouseDoubleClicked(0); // Vérifie si le bouton gauche de la souris a été double-cliqué


    //Check if mouse is on minimap:
    if(isWithinBounds(mousePos,ImVec2(1,1))){
        m_renderContext->mouseIsOnMinimap = true;
    //MouseDrag pour le déplacement
    if (ImGui::IsMouseDragging(0, 0.0f)) {
        ImVec2 delta = ImGui::GetMouseDragDelta(0, 0.0f);
        moveMap(true, true, delta.x/winWidth);
        moveMap(false, true, delta.y/winHeight);
        ImGui::ResetMouseDragDelta(0);
    }

    //MouseWheel pour le Zoom
    float mouseWheel = ImGui::GetIO().MouseWheel;
    if(mouseWheel > 0.0f) {
        Zoom(true); // Zoom avant pour un défilement vers le haut
    } else if(mouseWheel < 0.0f) {
        Zoom(false); // Zoom arrière pour un défilement vers le bas
    }}else{
        m_renderContext->mouseIsOnMinimap = false;
    
    }
}

ImVec2 MinimapTool::distanceConverterSimToLog(ImVec2 positionInitiale){
        // Calculer la position initiale
        ImVec2 initialPos = ImVec2(positionInitiale.x * scaledDistance, 
                                         positionInitiale.y * scaledDistance);

        // Appliquer la compression logarithmique et le facteur d'échelle
        double distance = std::sqrt(std::pow(initialPos.x, 2) + std::pow(initialPos.y, 2));
        double compressedDistance = std::log(std::max(distance, 1.0) * logBase) * scaleAfterLog;
        ImVec2 compressedPlanetPos = ImVec2(initialPos.x / distance * compressedDistance, 
                                            initialPos.y / distance * compressedDistance);

        // Calculer la position finale avec le décalage
        ImVec2 planetPos = ImVec2(compressedPlanetPos.x + offset.x, compressedPlanetPos.y + offset.y);
        return planetPos;
}

bool MinimapTool::isWithinBounds(ImVec2 position, ImVec2 size){
    return (position.x-size.x >= topLeft.x && position.x + size.x <= bottomRight.x &&
    position.y-size.y >= topLeft.y && position.y + size.y <= bottomRight.y);
}

void MinimapTool::checkMouseSelection(ImVec2 planetPos, CelestialObject* planete){
    float distanceToMouse = std::sqrt(std::pow(mousePos.x - planetPos.x, 2) + std::pow(mousePos.y - planetPos.y, 2));
    if (distanceToMouse < minDistance && distanceToMouse <= radius * 2) { //Planète la plus proche de la souris
        minDistance = distanceToMouse;
        closestPlanetName = planete->getName();
        selected = true;
    }
    if (isMouseClicked && distanceToMouse <= radius * 2) {//Si on click, on la sélectionne
        selectedObject = planete;
        m_renderContext->currentCamera->selectedObject = selectedObject;
        *(m_renderContext->showInfo) = true;
    }
    if(selected && isMouseDoubleClicked){//Si on doubleClick, on "follow"
        SelectObject(selectedObject);
    }
    selected = false;
}

void MinimapTool::moveMap(bool isX, bool isY, float value){
    //deplacement max de 1 et min de 0:
    float depassement = 0.25;
    if(isX){
        if(deplacement_X+value>initialCenterX+depassement){deplacement_X=initialCenterX+depassement;}
        else if(deplacement_X+value<initialCenterX-depassement){deplacement_X=initialCenterX-depassement;}
        else{deplacement_X+=value;}
    }
    else{
        if(deplacement_Y+value>initialCenterY+depassement){deplacement_Y=initialCenterY+depassement;}
        else if(deplacement_Y+value<initialCenterY-depassement){deplacement_Y=initialCenterY-depassement;}
        else{deplacement_Y+=value;}
    }
}

void MinimapTool::resetCamera(){
    cameraTransition = true;
}

void MinimapTool::transitionCamera() {
    const float accelerationFactor = 0.1; // Facteur d'accélération à ajuster selon vos besoins

    // Calcul de la vitesse ajustée pour X
    float distanceX = std::abs(deplacement_X - initialCenterX);
    float adjustedSpeedX = transitionSpeed + accelerationFactor * distanceX;

    // Calcul de la vitesse ajustée pour Y
    float distanceY = std::abs(deplacement_Y - initialCenterY);
    float adjustedSpeedY = transitionSpeed + accelerationFactor * distanceY;

    // Appliquer le déplacement avec la vitesse ajustée pour X
    if (deplacement_X > initialCenterX) {
        deplacement_X -= adjustedSpeedX;
        if (deplacement_X < initialCenterX) {
            deplacement_X = initialCenterX;
        }
    } else if (deplacement_X < initialCenterX) {
        deplacement_X += adjustedSpeedX;
        if (deplacement_X > initialCenterX) {
            deplacement_X = initialCenterX;
        }
    }

    // Appliquer le déplacement avec la vitesse ajustée pour Y
    if (deplacement_Y > initialCenterY) {
        deplacement_Y -= adjustedSpeedY;
        if (deplacement_Y < initialCenterY) {
            deplacement_Y = initialCenterY;
        }
    } else if (deplacement_Y < initialCenterY) {
        deplacement_Y += adjustedSpeedY;
        if (deplacement_Y > initialCenterY) {
            deplacement_Y = initialCenterY;
        }
    }
    if(zoom>1){
        zoom*=0.98;
    }
    else if(zoom<1){
        zoom*=1.02;}
    bool zoomOk;
    if(zoom>0.99 && zoom<1.01){zoomOk=true;zoom=1;}
    else{zoomOk=false;}
    // Vérifier si la position cible est atteinte
    if (deplacement_X == initialCenterX && deplacement_Y == initialCenterY && zoomOk) {
        cameraTransition = false;
    }
    applyZoomToAsteroids();
}

void MinimapTool::Zoom(bool in){
    if(in){
        zoom *= 0.99;
    }
    else{
        zoom *= 1.01;
    }
    applyZoomToAsteroids();
}

void MinimapTool::applyZoomToAsteroids() {
    for (size_t i = 0; i < asteroid1Positions.size(); ++i) {
        asteroid1Positions[i].x = originalAsteroid1Positions[i].x * zoom;
        asteroid1Positions[i].y = originalAsteroid1Positions[i].y * zoom;
    }
    for (size_t i = 0; i < asteroid2Positions.size(); ++i) {
        asteroid2Positions[i].x = originalAsteroid2Positions[i].x * zoom;
        asteroid2Positions[i].y = originalAsteroid2Positions[i].y * zoom;
    }
}
