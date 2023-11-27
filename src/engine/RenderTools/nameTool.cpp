#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/namesTool.h"


NameTool::NameTool(RenderContext* renderContext) : RenderComponent(renderContext){
    initLabbels();
    alpha = 0.6f;
}

void NameTool::initLabbels() {
    for (auto& object : m_renderContext->systemeSolaire->objects) {
        std::string planetName = object->getName(); // Obtenez le nom de la planète

        // Paramètres pour le Labbel - ces valeurs peuvent être ajustées selon vos besoins
        float xPercent = 0.5f; // Exemple de position X (en pourcentage de l'écran)
        float yPercent = 0.5f; // Exemple de position Y
        ImVec4 color = ImVec4(255, 255, 255, 255); // Couleur blanche
        float fontSize = 20.0f; // Taille de police

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
    if (labbels.empty() || m_renderContext->systemeSolaire->objects.empty()) {
        return; // Vérifiez qu'il y a au moins un label et un objet planétaire
    }

    glm::mat4 viewMatrix = m_renderContext->currentCamera->getViewMatrix();
    glm::mat4 projectionMatrix = m_renderContext->currentCamera->getProjectionMatrix();
    int screenWidth, screenHeight;
    auto followedObject = m_renderContext->currentCamera->followedObject;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);
    for (size_t i = 0; i < m_renderContext->systemeSolaire->objects.size(); ++i) {
        auto& object = m_renderContext->systemeSolaire->objects[i];
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


        //if(object->getName()=="Earth"){std::cout<<"XY: "<<xPercent<<" "<<topYPercent<<std::endl;}
        // Vérifier si le label devrait être visible
        if(object == followedObject){
            labbels[i]->UpdateAlpha(0.0f);
        }
        else if (ndc.x >= -1.0f && ndc.x <= 1.0f && ndc.y >= -1.0f && ndc.y <= 1.0f && ndc.z >= 0.0f && ndc.z <= 1.0f) {
            labbels[i]->UpdateAlpha(alpha); // L'objet est visible à l'écran
            labbels[i]->UpdatePosition(xPercent, topYPercent);
        } else {
            labbels[i]->UpdateAlpha(0.0f); // L'objet n'est pas visible à l'écran
        }
    }
}