#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/namesTool.h"


NameTool::NameTool(RenderContext* renderContext) : RenderComponent(renderContext){
    initLabbels();
}

void NameTool::initLabbels() {
    for (auto& object : m_renderContext->systemeSolaire->objects) {
        std::string planetName = object->getName(); // Obtenez le nom de la planète

        // Paramètres pour le Labbel - ces valeurs peuvent être ajustées selon vos besoins
        float xPercent = 0.5f; // Exemple de position X (en pourcentage de l'écran)
        float yPercent = 0.5f; // Exemple de position Y
        ImVec4 color = ImVec4(255, 255, 255, 255); // Couleur blanche
        float fontSize = 20.0f; // Taille de police
        float alpha = 1.0f; // Opacité

        // Créez un nouveau Labbel
        Labbel* newLabel = new Labbel(xPercent, yPercent, color, planetName, fontSize, alpha);

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


void NameTool::updateLabelPositions() {
    if (labbels.size() != m_renderContext->systemeSolaire->objects.size()) {
        return; // Assurez-vous qu'il y a un label pour chaque objet planétaire
    }

    glm::mat4 viewMatrix = m_renderContext->currentCamera->getViewMatrix();
    glm::mat4 projectionMatrix = m_renderContext->currentCamera->getProjectionMatrix();
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

    for (size_t i = 0; i < m_renderContext->systemeSolaire->objects.size(); ++i) {
        auto& object = m_renderContext->systemeSolaire->objects[i];
        glm::vec3 planetPos3D = object->getPositionSimulation().toGlm();
        float planetRadius = object->getRayon();

        // Convertir les coordonnées 3D en coordonnées de clip
        glm::vec4 clipCoords = projectionMatrix * viewMatrix * glm::vec4(planetPos3D, 1.0f);

        // Convertir en NDC
        glm::vec3 ndc = glm::vec3(clipCoords) / clipCoords.w;

        // Convertir en coordonnées d'écran
        float xPercent = (ndc.x + 1.0f) / 2.0f;
        float yPercent = (1.0f - ndc.y) / 2.0f;

        // Calculer une position approximative du haut de la planète
        glm::vec4 topOfPlanetPos = glm::vec4(planetPos3D, 1.0f) + glm::vec4(0, planetRadius, 0, 0);
        topOfPlanetPos = projectionMatrix * viewMatrix * topOfPlanetPos;
        topOfPlanetPos /= topOfPlanetPos.w;

        float topYPercent = (1.0f - topOfPlanetPos.y) / 2.0f;
        topYPercent -= 0.02f; // Ajustement pour monter le label

        // Vérifier si le label devrait être visible
        if (xPercent >= 0.0f && xPercent <= 1.0f) {
            labbels[i]->UpdateAlpha(1.0f); // L'objet est visible à l'écran
            labbels[i]->UpdatePosition(xPercent, topYPercent);
        } else {
            labbels[i]->UpdateAlpha(0.0f); // L'objet n'est pas visible à l'écran
        }
    }
}

