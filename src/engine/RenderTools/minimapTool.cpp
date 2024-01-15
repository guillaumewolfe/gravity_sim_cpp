
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


MinimapTool::MinimapTool(RenderContext* renderContext) : RenderComponent(renderContext){
    typeDict = m_renderContext->colorByTypeDict;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    longueur = winHeight* 0.80; // Exemple de taille
    hauteur = winHeight * 0.8; // Exemple de taille
    scale = 0.95;
    longueurScene = longueur * scale;
    hauteurScene = hauteur * scale;
    float fontsize = 17.0f;
    float fontSizeScaled = fontsize * winWidth / 1920;
    nameFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("../assets/fonts/Roboto.ttf", fontSizeScaled);
    generate_asteroids();
    generate_colors();
    generate_buttons();
}

void MinimapTool::Draw() {
glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

draw_rect();
draw_Scene();
draw_UI();
if (ImGui::IsKeyReleased(ImGuiKey_Escape)){CloseButton();}
if (ImGui::IsKeyReleased(ImGuiKey_Enter)){SelectObject(selectedObject);}
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
    ImVec4 buttonColor = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    ImageButton* imageButton = new ImageButton(0.50+0.96*longueur/(2*winWidth),0.5-0.96*hauteur/(2*winHeight),ImVec2(0.025f,0.025f),0.6f,
                        buttonColor, buttonColor,
                        "../assets/button/close.png", 0,
                            std::bind(&MinimapTool::CloseButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    imageButtons.push_back(imageButton);

    Icon* camera = new Icon(0.50+0.96*longueur/(2*winWidth),0.5-0.96*hauteur/(2*winHeight),ImVec2(0.02f,0.02f),0.35f,"../assets/button/pin.png",0.5);
    iconCamera = camera;    
}

void MinimapTool::draw_rect(){

    ImVec2 centerPos = ImVec2(winWidth * 0.5, winHeight * 0.5f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0,0,0,255), // Couleur
                            cornerRadius);    // Dessiner le premier rectangle avec coins arrondis
 
    //drawList->AddCircleFilled(centerPos, 0.96*hauteur/2, IM_COL32(0,0,0,255),100);
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(15, 20, 25, 120), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(255,255,255,40), // Couleur
                        cornerRadius,0,0.2f);
}




void MinimapTool::draw_UI(){
    for(auto& button : imageButtons){
        button->Draw();
    }
    iconCamera->Draw();
}

void MinimapTool::draw_Scene(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    scale = 0.95;
    longueurScene = longueur * scale;
    hauteurScene = hauteur * scale;

    ImVec2 centerPos = ImVec2(winWidth * 0.5, winHeight * 0.5f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueurScene * 0.5f, centerPos.y - hauteurScene * 0.5f);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueurScene * 0.5f, centerPos.y + hauteurScene * 0.5f);
    draw_planets();
}

void MinimapTool::draw_planets(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 mousePos = ImGui::GetMousePos();
    bool isMouseClicked = ImGui::IsMouseClicked(0); // Vérifie si le bouton gauche de la souris a été cliqué
    bool isMouseDoubleClicked = ImGui::IsMouseDoubleClicked(0); // Vérifie si le bouton gauche de la souris a été double-cliqué
    float minDistance = std::numeric_limits<float>::max();
    std::string closestPlanetName;
    double scaledDistance = hauteurScene / m_renderContext->systemeSolaire->maxSize;
    double logBase = 1.1; // Base logarithmique pour la compression des distances
    double scaleAfterLog = 45.0; // Facteur d'échelle après le calcul logarithmique
    ImVec2 centerPos = ImVec2(winWidth * 0.5, winHeight * 0.5f);
    // Calculer la position initiale du centre (premier objet)
    auto& centreObjet = m_renderContext->systemeSolaire->objects.front();
    ImVec2 initialCentrePos = ImVec2(centreObjet->getPositionSimulation().x * scaledDistance,
                                     centreObjet->getPositionSimulation().z * scaledDistance);
    double centreDistance = std::sqrt(std::pow(initialCentrePos.x, 2) + std::pow(initialCentrePos.y, 2));
    double compressedCentreDistance = std::log(std::max(centreDistance, 1.0) * logBase) * scaleAfterLog;
    ImVec2 compressedCentrePos = ImVec2(initialCentrePos.x / centreDistance * compressedCentreDistance,
                                        initialCentrePos.y / centreDistance * compressedCentreDistance);

    // Décalage à appliquer à toutes les planètes pour centrer le système
    ImVec2 offset = ImVec2(centerPos.x - compressedCentrePos.x, centerPos.y - compressedCentrePos.y);
    ImGui::PushFont(nameFont);
    float radius = winHeight * 0.01;
draw_asteroid_belt(drawList, asteroid1Positions, centerPos, winHeight*0.0010, IM_COL32(180, 150, 150, 120), -7.14e-9);
draw_asteroid_belt(drawList, asteroid2Positions, centerPos, winHeight*0.0010, IM_COL32(150, 200, 255, 120), -1.25e-10);

    for(auto& planete : m_renderContext->systemeSolaire->objects){
        if(planete->type == 5) continue; // Ne pas dessiner le centre (par exemple, le Soleil
        // Calculer la position initiale
        ImVec2 initialPlanetPos = ImVec2(planete->getPositionSimulation().x * scaledDistance, 
                                         planete->getPositionSimulation().z * scaledDistance);

        // Appliquer la compression logarithmique et le facteur d'échelle
        double distance = std::sqrt(std::pow(initialPlanetPos.x, 2) + std::pow(initialPlanetPos.y, 2));
        double compressedDistance = std::log(std::max(distance, 1.0) * logBase) * scaleAfterLog;
        ImVec2 compressedPlanetPos = ImVec2(initialPlanetPos.x / distance * compressedDistance, 
                                            initialPlanetPos.y / distance * compressedDistance);

        // Calculer la position finale avec le décalage
        ImVec2 planetPos = ImVec2(compressedPlanetPos.x + offset.x, compressedPlanetPos.y + offset.y);
    if (planetPos.x - radius >= topLeft.x && planetPos.x + radius <= bottomRight.x &&
            planetPos.y - radius >= topLeft.y && planetPos.y + radius <= bottomRight.y) {

        float distanceToCenter = std::sqrt(std::pow(planetPos.x - centerPos.x, 2) + std::pow(planetPos.y - centerPos.y, 2));
        ImVec4 colorType = getTypeColor(planete->type);
        std::string planetName = planete->getName(); // Assurez-vous que getName() renvoie une chaîne de caractères
        ImVec4 color = typeDictColor[planetName]; // Utiliser le nom pour accéder à la couleur
        // Dessiner la planète
        drawList->AddCircleFilled(planetPos, radius, IM_COL32(color.x, color.y, color.z, 255),100);
        drawList->AddCircle(centerPos, distanceToCenter, IM_COL32(color.x, color.y, color.z, 10), 100, radius/2);// Assurez-vous que la méthode getName() existe
        ImVec2 textPos = ImVec2(planetPos.x - ImGui::CalcTextSize(planetName.c_str()).x / 2, 
                                planetPos.y - radius - winHeight*0.025); // 20 est l'offset en Y, ajustez selon vos besoins
        float distanceToMouse = std::sqrt(std::pow(mousePos.x - planetPos.x, 2) + std::pow(mousePos.y - planetPos.y, 2));
        if (distanceToMouse < minDistance && distanceToMouse <= radius * 2) {
            minDistance = distanceToMouse;
            closestPlanetName = planete->getName();
            selected = true;
        }
        if (isMouseClicked && distanceToMouse <= radius * 2) {
            selectedObject = planete;
            m_renderContext->currentCamera->selectedObject = selectedObject;
            *(m_renderContext->showInfo) = true;
        }
        //Soleil
        if(planete->type == 1){
            ImVec4 colorCenterDot = ImVec4(255,215,80,255);
            float numBlurCircles = 40;
            float blurIncrease = radius*0.04; // How much larger each successive blur circle is
            float initialAlpha = 20; // Starting alpha value for the outermost blur circle
            float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
            for (int i = 0; i < numBlurCircles; ++i) {
                float blurRadius = radius + blurIncrease * (i + 1);
                float alpha = initialAlpha - alphaDecrease * i;
                drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);
            }}
        else{
            ImVec2 directionToSun = ImVec2(planetPos.x - centerPos.x, planetPos.y - centerPos.y);
            float shadowAngle = atan2(directionToSun.y, directionToSun.x) + IM_PI; // Ajouter PI pour que l'ombre soit opposée au Soleil
            // Dessiner l'ombre comme un demi-cercle
            draw_half_circle_shadow(drawList, planetPos, radius, IM_COL32(0, 0, 0, 180), shadowAngle, 100);

        }
}}

    for(auto& planete : m_renderContext->systemeSolaire->objects) {
        std::string planetName = planete->getName();
        ImVec4 color = typeDictColor[planetName];
        if(planete->type == 5) continue; // Ne pas dessiner le centre (par exemple, le Soleil
        // Calculer la position initiale
        ImVec2 initialPlanetPos = ImVec2(planete->getPositionSimulation().x * scaledDistance, 
                                         planete->getPositionSimulation().z * scaledDistance);

        // Appliquer la compression logarithmique et le facteur d'échelle
        double distance = std::sqrt(std::pow(initialPlanetPos.x, 2) + std::pow(initialPlanetPos.y, 2));
        double compressedDistance = std::log(std::max(distance, 1.0) * logBase) * scaleAfterLog;
        ImVec2 compressedPlanetPos = ImVec2(initialPlanetPos.x / distance * compressedDistance, 
                                            initialPlanetPos.y / distance * compressedDistance);

        // Calculer la position finale avec le décalage
        ImVec2 planetPos = ImVec2(compressedPlanetPos.x + offset.x, compressedPlanetPos.y + offset.y);
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
        if (textPos.x >= topLeft.x && textPos.x + textSize.x <= bottomRight.x &&
            textPos.y >= topLeft.y && textPos.y + textSize.y <= bottomRight.y) {

        drawList->AddText(textPos, IM_COL32(255, 255, 255, alpha), planetName.c_str());
        drawList->AddCircle(planetPos, radius*1.2, IM_COL32(255,255,255,alphaCircle),100,winWidth*0.001);}// Assurez-vous que la méthode getName() existe
        if(selected && isMouseDoubleClicked){
            SelectObject(selectedObject);
        }
        selected=false;
    }
ImVec2 cameraSimPosition = ImVec2(
    m_renderContext->currentCamera->getPosition().x * scaledDistance,
    m_renderContext->currentCamera->getPosition().z * scaledDistance
);

// Appliquer la compression logarithmique et le facteur d'échelle
double cameraDistance = std::sqrt(std::pow(cameraSimPosition.x, 2) + std::pow(cameraSimPosition.y, 2));
double compressedCameraDistance = std::log(std::max(cameraDistance, 1.0) * logBase) * scaleAfterLog;
ImVec2 compressedCameraPos = ImVec2(
    cameraSimPosition.x / cameraDistance * compressedCameraDistance,
    cameraSimPosition.y / cameraDistance * compressedCameraDistance
);

// Calculer la position finale avec le décalage
ImVec2 finalCameraPos = ImVec2(
    compressedCameraPos.x + offset.x,
    compressedCameraPos.y + offset.y
);
iconCamera->UpdatePosition(finalCameraPos.x/winWidth,finalCameraPos.y/winHeight);
// Texte à afficher au-dessus de l'icône de la caméra
std::string cameraText = "You";

// Calculer la position du texte
float textOffsetY = iconCamera->getSize().y*1.3*winHeight; // Ajustez cette valeur selon vos besoins
ImVec2 textPos = ImVec2(finalCameraPos.x - ImGui::CalcTextSize(cameraText.c_str()).x / 2, 
                        finalCameraPos.y - textOffsetY);

// Vérifier si le texte est dans les limites de la fenêtre
if (textPos.x >= topLeft.x && textPos.x + ImGui::CalcTextSize(cameraText.c_str()).x <= bottomRight.x &&
    textPos.y >= topLeft.y && textPos.y + ImGui::CalcTextSize(cameraText.c_str()).y <= bottomRight.y) {
    // Dessiner le texte
    drawList->AddText(textPos, IM_COL32(220, 255, 220, 200), cameraText.c_str());
}
    ImGui::PopFont();
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

    // Ceinture de Kuiper
    float rayonInterieur2 = 30.0f * scale/2; // Commence à la position de Neptune
    float rayonExterieur2 = 33.0f * scale/2; // S'étend au-delà de Neptune
    int numAsteroides2 = 700;
    generate_asteroid_belt(gen, dis, rayonInterieur2, rayonExterieur2, numAsteroides2, asteroid2Positions);
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

void MinimapTool::draw_asteroid_belt(ImDrawList* drawList, const std::vector<ImVec2>& asteroidPositions, ImVec2 centerPos, float tailleAsteroide, ImU32 color, float rotation_par_seconde) {
    float time = *(m_renderContext->simulationTime);
    
    // Angle de rotation total basé sur le temps de simulation et la rotation par seconde
    float angle_total = rotation_par_seconde * time * 2.0f * M_PI;

    for (const ImVec2& pos : asteroidPositions) {
        // Rotation de l'astéroïde
        float x_rotated = cos(angle_total) * pos.x - sin(angle_total) * pos.y;
        float y_rotated = sin(angle_total) * pos.x + cos(angle_total) * pos.y;

        // Dessiner l'astéroïde à sa nouvelle position
        drawList->AddCircleFilled(ImVec2(x_rotated + centerPos.x, y_rotated + centerPos.y), tailleAsteroide, color);
    }
}

ImVec4 MinimapTool::getTypeColor(int type) {
    return typeDict[type].second;
}

void MinimapTool::generate_colors() {
    // Modifier le dictionnaire pour utiliser des chaînes de caractères comme clés
    typeDictColor["Black Hole"] = ImVec4(0, 0, 0, 1);
    typeDictColor["Sun"] = ImVec4(250, 250, 120, 1); // Jaune pour une étoile par exemple
    typeDictColor["Mercury"] = ImVec4(150, 150, 150, 1);
    typeDictColor["Venus"] = ImVec4(155, 100, 25, 1); // Orange pour Venus
    typeDictColor["Earth"] = ImVec4(50, 100, 255, 1); // Bleu pour la Terre
    typeDictColor["Mars"] = ImVec4(200, 63, 33, 1); // Rougeâtre pour Mars
    typeDictColor["Jupiter"] = ImVec4(225, 175, 135, 1);
    typeDictColor["Saturn"] = ImVec4(175, 175, 135, 1); // Jaune pour Saturne
    typeDictColor["Uranus"] = ImVec4(50, 50, 255, 1);
    typeDictColor["Neptune"] = ImVec4(50, 50, 255, 1); // Bleu pour Neptune
    typeDictColor["Ice"] = ImVec4(103, 225, 255, 1); // Bleu clair pour les objets glacés
    typeDictColor["Rock"] = ImVec4(200, 200, 200, 1); // Gris pour les objets rocheux
    typeDictColor["Volcanic"] = ImVec4(200, 200, 200, 1); // Gris foncé pour les objets volcaniques
}

void MinimapTool::draw_half_circle_shadow(ImDrawList* drawList, ImVec2 center, float radius, ImU32 color, float angle, int num_segments) {
    // Calcule les angles pour le demi-cercle d'ombre
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
    CloseButton();
    m_renderContext->currentCamera->newFollowObject(object);
    *(m_renderContext->showInfo) = true;
}