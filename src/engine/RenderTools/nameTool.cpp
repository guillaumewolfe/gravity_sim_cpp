#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/namesTool.h"


NameTool::NameTool(RenderContext* renderContext) : RenderComponent(renderContext){
    alpha = 0.55f;
    customFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("../assets/fonts/Ubuntu.ttf", 14.0f);
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
    auto& objects = m_renderContext->systemeSolaire->objects;
    if (labbels.size() != objects.size()) {
        synchronizeLabels();
    }


    glm::mat4 viewMatrix = m_renderContext->currentCamera->getViewMatrix();
    glm::mat4 projectionMatrix = m_renderContext->currentCamera->getProjectionMatrix();
    int screenWidth, screenHeight;
    auto followedObject = m_renderContext->currentCamera->followedObject;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);
    for (size_t i = 0; i < objects.size(); ++i) {
        auto& object = objects[i];
        glm::vec3 planetPos3D = object->getPositionSimulation().toGlm();

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
            labbels[i]->UpdateAlpha(alpha); // Object is visible on screen
            labbels[i]->UpdatePosition(xPercent, topYPercent);
        } else {
            labbels[i]->UpdateAlpha(0.0f); // Object is not visible on screen
        }
    }
    constexpr float minDistance = 0.02f; // Distance minimale pour éviter la superposition (à ajuster selon vos besoins)
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
                if (pos1.y < pos2.y) {
                    labbels[j]->UpdatePosition(pos2.x, pos2.y - verticalShiftAmount);
                } else {
                    labbels[i]->UpdatePosition(pos1.x, pos1.y - verticalShiftAmount);
                }
            }
        }
    }
}




