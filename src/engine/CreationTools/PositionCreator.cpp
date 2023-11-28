#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/CreationTools/PositionCreator.h"
#include "engine/CreationTools/CreatorManager.h"
#include "engine/Vec3.h"


PositionCreator::PositionCreator(RenderContext* renderContext, CreatorManager* manager) : StateCreator(renderContext, manager) {
    generate_buttons();
    generate_labels();
}

void PositionCreator::Enter(){
    m_renderContext->currentCamera->chosePositionMode();
    createNewObject();
} 


void PositionCreator::Draw(){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.3f, 0.4f, 0.3f)); // Exemple de couleur (bleu foncé)

    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    if (ImGui::IsKeyReleased(ImGuiKey_Escape)) {previous_state();}
    drawBackground();
    draw_buttons();
    draw_labels();
    DrawOrbits();

    ImGui::End(); 
    ImGui::PopStyleColor();
} 
void PositionCreator::DrawOpenGL(){} 




void PositionCreator::generate_buttons(){
   Button *ReturnButton = new Button(0.05f, 0.57, ImVec2(0.05, 0.045),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                               
                               "Previous", 0.00f,22.0f,
                               std::bind(&PositionCreator::previous_state, this));  

   Button *NextButton = new Button(0.1f, 0.57, ImVec2(0.05, 0.04),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               "Next", 0.0f,23.0f,
                               std::bind(&PositionCreator::next_state, this));  
    buttons.push_back(ReturnButton);
    buttons.push_back(NextButton);
    buttons[1]->UpdateLabelColor(100,255,150,200);
    buttons[0]->UpdateLabelColor(255,125,100,200);
}



void PositionCreator::generate_labels(){
    Labbel *MessageLabel = new Labbel(0.075f,0.25f,ImVec4(255,255,255,255),
                            "Choose position",22.0f,0.9f);
    Labbel *DistanceStaticLabel = new Labbel(0.075f,0.35f,ImVec4(255,255,255,255),
                        "Distance from star",21.0f,0.7f);
    Labbel *DistanceLabel = new Labbel(0.075f,0.38f,ImVec4(175,255,225,200),
                    "2 AU",20.0f,0.7f);
    labbels.push_back(MessageLabel);
    labbels.push_back(DistanceStaticLabel);
    labbels.push_back(DistanceLabel);
}
void PositionCreator::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 30.0f;


    float longueur = winWidth* 0.14; // Exemple de taille
    float hauteur = winHeight * 0.40; // Exemple de taille

    ImVec2 centerPos = ImVec2(winWidth * 0.075f, winHeight * 0.4f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);


    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(0, 0, 0, 255), // Couleur
                            cornerRadius);

    float cornerRadiusAdjustment = 10.0f;
    drawList->AddRect(topLeft,
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur),
                        IM_COL32(100, 100, 100, 150), // Couleur de remplissage
                        cornerRadius,0,3.0f); // Ajustez le rayon ici
}

void PositionCreator::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
}

void PositionCreator::draw_labels(){
        for (Labbel* label : labbels) {
        label->Draw();
    }
}

void PositionCreator::next_state(){

}
void PositionCreator::previous_state(){
    m_manager->ChangeState("TextureCreator");
    removeNewObject();
}

void PositionCreator::Exit(){
    m_renderContext->currentCamera->resetPosition();
} 

void PositionCreator::createNewObject(){
    CelestialObject* newObj = m_manager->newCreatedObject;
    if (newObj) {
        m_renderContext->systemeSolaire->addObject(newObj);
        newObj->position_simulation=Vec3(-50,0,0);
    }
}


void PositionCreator::removeNewObject(){
    m_renderContext->systemeSolaire->removeObject(m_manager->newCreatedObject);
}



void PositionCreator::DrawOrbits() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 center(winWidth * 0.5f, winHeight * 0.5f); // Centre de la fenêtre
    auto& objects = m_renderContext->systemeSolaire->objects;
    // Parcourez tous les objets pour dessiner leurs orbites
    for (const auto& object : objects) {
        glm::vec3 planetPos3D = object->getPositionSimulation().toGlm();

        // Convertir les coordonnées 3D en coordonnées de clip
        glm::mat4 viewMatrix = m_renderContext->currentCamera->getViewMatrix();
        glm::mat4 projectionMatrix = m_renderContext->currentCamera->getProjectionMatrix();
        glm::vec4 clipCoords = projectionMatrix * viewMatrix * glm::vec4(planetPos3D, 1.0f);
        glm::vec3 ndc = glm::vec3(clipCoords) / clipCoords.w;

        // Convertir en coordonnées d'écran
        float xPercent = (ndc.x + 1.0f) / 2.0f;
        float yPercent = (1.0f - ndc.y) / 2.0f;
        
        ImVec2 planetScreenPos = ImVec2(xPercent * winWidth, yPercent * winHeight);
        float radius = std::sqrt(std::pow(center.x - planetScreenPos.x, 2) + std::pow(center.y - planetScreenPos.y, 2));

        ImU32 color = IM_COL32(255, 0, 0, 100); // Couleur de l'orbite
        int num_segments = 300; // Qualité du cercle
        drawList->AddCircle(center, radius, color, num_segments);
    }
}