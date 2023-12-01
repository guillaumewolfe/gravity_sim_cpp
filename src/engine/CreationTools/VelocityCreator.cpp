#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/CreationTools/VelocityCreator.h"
#include "engine/CreationTools/CreatorManager.h"
#include "engine/Vec3.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 



VelocityCreator::VelocityCreator(RenderContext* renderContext, CreatorManager* manager) : StateCreator(renderContext, manager) {
    generate_buttons();
    generate_labels();
    generate_sliders();
}

void VelocityCreator::Enter(){
    velocityValue =0;
    velocityAngle = 0;
    computeEscapeVelocitySpeed();
    m_renderContext->currentCamera->creationMode();
    m_manager->updateCamera();
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

    calculateVelocityVector();
    DrawOrbits();
    drawBackground();
    draw_buttons();
    draw_labels();
    draw_sliders();

    ImGui::End(); 
} 
void VelocityCreator::DrawOpenGL(){} 




void VelocityCreator::generate_buttons(){
   Button *ReturnButton = new Button(0.05f, 0.57, ImVec2(0.05, 0.045),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                               
                               "Previous", 0.00f,20.0f,
                               std::bind(&VelocityCreator::previous_state, this));  

   Button *NextButton = new Button(0.1f, 0.57, ImVec2(0.05, 0.04),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               "Select", 0.0f,20.0f,
                               std::bind(&VelocityCreator::next_state, this));  


    buttons.push_back(ReturnButton);
    buttons.push_back(NextButton);
    buttons[1]->UpdateLabelColor(100,255,150,200);
    buttons[0]->UpdateLabelColor(255,125,100,200);

}

void VelocityCreator::generate_sliders(){
    Slider *VelocitySlider = new Slider(0.075f,0.38,ImVec2(0.14*0.8, 0.40*0.015),ImVec4(225,225,225,225),
                                ImVec4(100,255,150,200),&velocityValue,ImVec2(0,1),0);
    
    sliders.push_back(VelocitySlider);
}



void VelocityCreator::generate_labels(){
    Labbel *MessageLabel = new Labbel(0.075f,0.25f,ImVec4(255,255,255,255),
                            "Choose Velocity",22.0f,0.9f);
    
    Labbel *VelocityLabel = new Labbel(0.075f,0.45f,ImVec4(100,255,150,200),
                        "",24.0f,0.8f);
    
    Labbel *infoOrbitLabel = new Labbel(0.075f,0.34f,ImVec4(150,255,220,200),
                    "Stationary",19.0f,0.8f);

    labbels.push_back(MessageLabel);
    labbels.push_back(VelocityLabel);
    labbels.push_back(infoOrbitLabel);
}
void VelocityCreator::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 2.0f;


    float longueur = winWidth* 0.14; // Exemple de taille
    float hauteur = winHeight * 0.40; // Exemple de taille

    centerPos = ImVec2(winWidth * 0.075f, winHeight * 0.4f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    bottomRight = ImVec2(topLeft.x + longueur, topLeft.y + hauteur);



    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(7.5, 7.5, 7.5, 255), // Couleur
                            cornerRadius);

    float cornerRadiusAdjustment = 10.0f;
    drawList->AddRect(topLeft,
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur),
                        IM_COL32(50, 50, 50, 150), // Couleur de remplissage
                        cornerRadius,0,3.0f); // Ajustez le rayon ici

}

void VelocityCreator::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
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
    m_manager->ChangeState("ParametersCreator");
}
void VelocityCreator::previous_state(){
    m_manager->ChangeState("PositionCreator");
}
void VelocityCreator::setNewObjVelocity(){
    Vec3 newVel;
    newVel.x = finalVelocityValue * cos(velocityAngle);
    newVel.z = finalVelocityValue * sin(velocityAngle);
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
            Vec3 velocityDirection(std::cos(velocityAngle), 0, std::sin(velocityAngle));
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
            float pointBaseAngle = velocityAngle + M_PI; // Ajouter π pour inverser la direction de la pointe

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

