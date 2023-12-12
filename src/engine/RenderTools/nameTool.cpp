#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/namesTool.h"


NameTool::NameTool(RenderContext* renderContext) : RenderComponent(renderContext){
    alpha = 0.6f;
    customFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("../assets/fonts/Roboto.ttf", 14.0f);
    initLabbels();
}

void NameTool::initLabbels() {
    for (auto& object : m_renderContext->systemeSolaire->objects) {
        std::string planetName = object->getName(); // Obtenez le nom de la planète

        // Paramètres pour le Labbel - ces valeurs peuvent être ajustées selon vos besoins
        float xPercent = 0.5f; // Exemple de position X (en pourcentage de l'écran)
        float yPercent = 0.5f; // Exemple de position Y
        ImVec4 color = ImVec4(255, 255, 255, 255); // Couleur blanche
        float fontSize = 15.0f; // Taille de police

        // Créez un nouveau Labbel
        Labbel* newLabel = new Labbel(xPercent, yPercent, color, planetName, fontSize, alpha, customFont);

        // Ajoutez-le au vecteur
        labbels.push_back(newLabel);
    }
    
}


void NameTool::Draw() {
ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

updateLabelPositions();
for (Labbel* label : labbels) {
    label->Draw();
}
detectClickAndPrintName();
ImGui::End(); 
}

void NameTool::synchronizeLabels() {
    auto& objects = m_renderContext->systemeSolaire->objects;

    // Supprimer tous les labels existants
    for (Labbel* label : labbels) {
        delete label;
    }
    labbels.clear();
    initLabbels();
}



void NameTool::updateLabelPositions() {
    if (labbels.empty() || m_renderContext->systemeSolaire->objects.empty()) {
        return; // Vérifiez qu'il y a au moins un label et un objet planétaire
    }
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    auto& objects = m_renderContext->systemeSolaire->objects;
    if (labbels.size() != objects.size()) {
        synchronizeLabels();
    }


    glm::mat4 viewMatrix = m_renderContext->currentCamera->getViewMatrix();
    glm::mat4 projectionMatrix = m_renderContext->currentCamera->getProjectionMatrix();
    auto followedObject = m_renderContext->currentCamera->followedObject;
    glfwGetWindowSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);
    for (size_t i = 0; i < objects.size(); ++i) {
        auto& object = objects[i];
        glm::vec3 planetPos3D = object->getPositionSimulation().toGlm();
        ImVec4 circleColor = m_renderContext->colorByTypeDict[object->type].second;
        // Convertir les coordonnées 3D en coordonnées de clip
        glm::vec4 clipCoords = projectionMatrix * viewMatrix * glm::vec4(planetPos3D, 1.0f);
        // Convertir en NDC
        glm::vec3 ndc = glm::vec3(clipCoords) / clipCoords.w;
        // Convertir en coordonnées d'écran
        float xPercent = (ndc.x + 1.0f) / 2.0f;
        float yPercent = (1.0f - ndc.y) / 2.0f;
        float planetRadius = object->getRayon();
        // Calculer une position approximative du haut de la planète
        glm::vec4 topOfPlanetPos = projectionMatrix * viewMatrix * glm::vec4(planetPos3D + glm::vec3(0, planetRadius, 0), 1.0f);
        topOfPlanetPos /= topOfPlanetPos.w;
        float topYPercent = (1.0f - topOfPlanetPos.y) / 2.0f;
        topYPercent -= 0.02f; // Ajuster légèrement vers le haut

        if(object == followedObject) {
            labbels[i]->UpdateAlpha(0.0f);
        }
        else if (ndc.x >= -1.0f && ndc.x <= 1.0f && ndc.y >= -1.0f && ndc.y <= 1.0f && ndc.z >= 0.0f && ndc.z <= 1.0f) {
            if(object == m_renderContext->currentCamera->selectedObject){
            labbels[i]->UpdateAlpha(1.0f);
            if(!*(m_renderContext->isCreating)){
            drawList->AddCircle(ImVec2(xPercent * screenWidth, yPercent * screenHeight), screenWidth*0.0030, IM_COL32(circleColor.x, circleColor.y, circleColor.z, 255),32,2.0f);}
        }else if(object == hoverObject){
            labbels[i]->UpdateAlpha(0.8f);
            if(!*(m_renderContext->isCreating)){
            drawList->AddCircle(ImVec2(xPercent * screenWidth, yPercent * screenHeight), screenWidth*0.0030, IM_COL32(circleColor.x, circleColor.y, circleColor.z, ((1-alpha)/2+alpha)*255),32,2.0f);}}
        else{
            labbels[i]->UpdateAlpha(alpha); 
            if(!*(m_renderContext->isCreating)){
            drawList->AddCircle(ImVec2(xPercent * screenWidth, yPercent * screenHeight), screenWidth*0.0030, IM_COL32(circleColor.x, circleColor.y, circleColor.z, alpha*255),32,2.0f);}
        } // Object is visible on screen
            labbels[i]->UpdatePosition(xPercent, topYPercent);
        } else {
            labbels[i]->UpdateAlpha(0.0f); // Object is not visible on screen
        }
    }

    constexpr float minDistance = 0.01f; // Distance minimale pour éviter la superposition (à ajuster selon vos besoins)
    constexpr float verticalShiftAmount = 0.02f; // Quantité de déplacement vertical pour éviter la superposition
    for (size_t i = 0; i < labbels.size(); ++i) {
        for (size_t j = i + 1; j < labbels.size(); ++j) {
            ImVec2 pos1 = labbels[i]->getPosition();
            ImVec2 pos2 = labbels[j]->getPosition();

            // Calculer la distance euclidienne entre les étiquettes
            float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2));

            // Vérifier si les étiquettes se superposent
            if (distance < minDistance) {
                // Déplacer l'étiquette la plus basse vers le haut
                if (pos1.y > pos2.y) {
                    labbels[j]->UpdatePosition(pos2.x, pos2.y - verticalShiftAmount);
                } else {
                    labbels[i]->UpdatePosition(pos1.x, pos1.y - verticalShiftAmount);
                }
            }
        }
    }
}

glm::vec2 NameTool::convert3DPosToScreenPos(const glm::vec3& pos3D, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, int screenWidth, int screenHeight) {
    // Convertir les coordonnées 3D en coordonnées de clip
    glm::vec4 clipCoords = projectionMatrix * viewMatrix * glm::vec4(pos3D, 1.0f);
    
    // Convertir en NDC
    glm::vec3 ndc = glm::vec3(clipCoords) / clipCoords.w;

    // Convertir en coordonnées d'écran en pourcentage
    float xPercent = (ndc.x + 1.0f) / 2.0f;
    float yPercent = (1.0f - ndc.y) / 2.0f;

    return glm::vec2(xPercent * screenWidth, yPercent * screenHeight);
}

void NameTool::detectClickAndPrintName() {
    if(*(m_renderContext->isCreating) || *(m_renderContext->showOptions) || *(m_renderContext->showSettings) or *(m_renderContext->showOptions)){
        return;
    }
    CelestialObject* closestPlanet = nullptr;
    glm::mat4 viewMatrix = m_renderContext->currentCamera->getViewMatrix();
    glm::mat4 projectionMatrix = m_renderContext->currentCamera->getProjectionMatrix();

    ImVec2 mousePos = ImGui::GetMousePos();
    float mouseXPercent = mousePos.x / screenWidth;
    float mouseYPercent = mousePos.y / screenHeight;


    float closestDistance = std::numeric_limits<float>::max();
    std::string closestPlanetName = "";
    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        glm::vec3 planetPos3D = object->getPositionSimulation().toGlm();
        glm::vec2 screenPos = convert3DPosToScreenPos(planetPos3D, viewMatrix, projectionMatrix, screenWidth, screenHeight);

        // Convertir les positions de la planète en pourcentages
        float planetXPercent = screenPos.x / screenWidth;
        float planetYPercent = screenPos.y / screenHeight;

        // Calculer la distance entre la souris et la planète
        float distance = std::hypot(mouseXPercent - planetXPercent, mouseYPercent - planetYPercent);

        // Mettre à jour la planète la plus proche et sa distance
        if (distance < closestDistance) {
            closestDistance = distance;
            closestPlanetName = object->getName();
            closestPlanet = object;
        }
    }

    // Seuil de détection
    float clickThreshold = 0.05f; // Ajustez ce seuil selon vos besoins

    if(closestDistance < clickThreshold){
        hoverObject = closestPlanet;
    }else{
        hoverObject = nullptr;
    }
    // Vérifier si la planète la plus proche est dans le seuil de clic
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && closestDistance < clickThreshold) {
        m_renderContext->currentCamera->selectedObject = closestPlanet;
        *(m_renderContext->showInfo) = true;
    }if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && closestDistance < clickThreshold){
        m_renderContext->currentCamera->newFollowObject(closestPlanet);
        *(m_renderContext->showInfo) = true;
    }
}




