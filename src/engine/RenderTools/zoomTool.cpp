
#include "glad/glad.h"
#include "engine/RenderTools/zoomTool.h"
#include <GLFW/glfw3.h>

ZoomTool::ZoomTool(RenderContext* renderContext) : RenderComponent(renderContext){
    // Création de la caméra avec la position, la cible et l'orientation vers le haut
    Vec3 zoomCameraPosition(0, 10, 10); // Position de la caméra
    Vec3 zoomCameraTarget(0, 0, 0);       // Cible (le soleil)
    Vec3 zoomCameraUp(0, 1, 0);           // Orientation vers le haut

    zoomCamera = new Camera(zoomCameraPosition, zoomCameraTarget, zoomCameraUp);

    // Réglage de l'angle de perspective pour le zoom
    double zoomAnglePerspective = 5; // Petit angle pour un zoom important
    zoomCamera->angle_perspective = zoomAnglePerspective;
    backgroundTool = new BackgroundTool(m_renderContext);
    objectsTool = new ObjectsTool(m_renderContext, zoomCamera);
    zoomLabbel = new Labbel(0.5,0.9,ImVec4(255,255,255,255),"Zoom",30.0f,0.7);
}

void ZoomTool::Draw() {
    checkInputs();
    double mouseX, mouseY;
    glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
    // Obtenir les dimensions de la fenêtre
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    updateCameraTargetWithMouse(mouseX, mouseY, winWidth, winHeight);
    // Activer le scissor test et définir la zone de scissor
    glfwGetFramebufferSize(glfwGetCurrentContext(), &bufferWidth, &bufferHeight);
    float screenScale = static_cast<float>(bufferWidth) / winWidth;
    winWidth *= screenScale;
    winHeight *= screenScale;

    int zoomViewSize = winHeight / 1.5; // Taille du carré de zoom
    glEnable(GL_SCISSOR_TEST);
    glScissor(winWidth/2-zoomViewSize/2, winHeight/2-zoomViewSize/2, zoomViewSize, zoomViewSize);

    // Définir la zone de vue pour la caméra de zoom
    glViewport(winWidth/2-zoomViewSize/2, winHeight/2-zoomViewSize/2, zoomViewSize, zoomViewSize);
    
    // Définir la couleur de fond et effacer la zone spécifiée par le scissor test
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fond noir
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Désactiver le scissor test après l'effacement
    glDisable(GL_SCISSOR_TEST);

    // Configurer la matrice de projection pour la caméra de zoom
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // Sauvegarder l'ancienne matrice de projection
    glLoadIdentity();
    glMultMatrixf(glm::value_ptr(zoomCamera->getProjectionMatrix()));

    // Configurer la matrice de vue pour la caméra de zoom
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Synchroniser la position et l'orientation de zoomCamera avec currentCamera
    zoomCamera->position = m_renderContext->currentCamera->getPosition();
    zoomCamera->up = m_renderContext->currentCamera->getUp();

    angle_pespective = m_renderContext->currentCamera->getAnglePerspective()/zoom;
    // Réduire de moitié l'angle de perspective pour le zoom
    zoomCamera->angle_perspective = angle_pespective;
    float zoomAspectRatio = static_cast<float>(zoomViewSize) / zoomViewSize; // Carré dans ce cas
    zoomCamera->setCustomPerspective(zoomAspectRatio);

    // Mettre à jour la caméra de zoom
    zoomCamera->Update();

    // Rendu de la scène avec la caméra de zoom
    backgroundTool->Draw();
    objectsTool->Draw();

    // Restaurer la matrice de projection pour la caméra principale
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Restaurer l'ancienne matrice de projection

    // Restaurer la matrice de vue pour la caméra principale
    glMatrixMode(GL_MODELVIEW);

    // Restaurer la vue complète pour le reste du rendu de la fenêtre
    glViewport(0, 0, winWidth, winHeight);
    glDisable(GL_SCISSOR_TEST);
    // Réactiver le test de profondeur si nécessaire
    glEnable(GL_DEPTH_TEST);

}



void ZoomTool::updateCameraTargetWithMouse(int mouseX, int mouseY, int screenWidth, int screenHeight) {
    // Centre de l'écran
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // Déplacement de la souris par rapport au centre de l'écran
    float deltaX = centerX-mouseX;
    float deltaY = centerY-mouseY;

    float ratioAspect = static_cast<float>(screenWidth) / screenHeight;
    float anglePerPixelX = m_renderContext->currentCamera->getAnglePerspective() * ratioAspect / screenWidth;
    float anglePerPixelY = m_renderContext->currentCamera->getAnglePerspective() / screenHeight;

    float yaw = deltaX * anglePerPixelX;
    float pitch = deltaY * anglePerPixelY;

    // Convertir les angles en radians
    float yawRadians = glm::radians(yaw);
    float pitchRadians = glm::radians(pitch);

    // Calculer les axes de rotation
    glm::vec3 cameraPos = m_renderContext->currentCamera->position.toGlm();
    glm::vec3 cameraTarget = m_renderContext->currentCamera->target.toGlm();
    glm::vec3 viewDirection = glm::normalize(cameraTarget - cameraPos);
    glm::vec3 horizontalAxis = glm::normalize(glm::cross(viewDirection, glm::vec3(0, 1, 0)));
    glm::vec3 verticalAxis = glm::normalize(glm::cross(horizontalAxis, viewDirection));

    // Appliquer les rotations
    glm::quat pitchQuat = glm::angleAxis(pitchRadians, horizontalAxis);
    glm::quat yawQuat = glm::angleAxis(yawRadians, verticalAxis);
    glm::quat tempQuat = yawQuat * pitchQuat;
    glm::vec3 newViewDirection = tempQuat * viewDirection;
    float distance = glm::distance(cameraPos, cameraTarget);
    distance = 1500;
    // Mettre à jour la cible de la caméra de zoom
    Vec3 newTarget = Vec3(newViewDirection.x, newViewDirection.y, newViewDirection.z) * distance + m_renderContext->currentCamera->position;
    zoomCamera->setTarget(newTarget);
}

void ZoomTool::drawImGui() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));

    ImGui::Begin("ZoomToolOverlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Calcul du centre de l'écran
    ImVec2 center(winWidth / 2.0f, winHeight / 2.0f);

    // Rayon pour le cercle extérieur (suffisamment grand pour couvrir les coins de l'écran)
    float outerRadius = sqrtf(winWidth * winWidth + winHeight * winHeight) / 2.0f; 

    // Rayon pour le cercle intérieur (la taille de la zone visible)
    float innerRadius = winHeight/3.0; // Ajustez selon vos besoins

    // Épaisseur de l'anneau
    float thickness = outerRadius - innerRadius;

    // Dessiner le cercle (anneau)
    drawList->AddCircle(center, outerRadius, IM_COL32(7.5, 12, 12, 255), 100, 2*thickness);
    drawList->AddCircle(center, innerRadius, IM_COL32(255,255,255, 150), 100, winWidth*0.001);
    
    float lineLength = innerRadius; // Ajustez la longueur selon vos besoins
    drawList->AddLine(ImVec2(center.x, center.y - lineLength), ImVec2(center.x, center.y + lineLength), IM_COL32(255,255,255, 100), winWidth * 0.00075);
    drawList->AddLine(ImVec2(center.x - lineLength, center.y), ImVec2(center.x + lineLength, center.y), IM_COL32(255,255,255, 100), winWidth * 0.00075);

    // Dessiner les sous-sections
    float subSectionLength = winWidth * 0.01f; // Ajustez selon vos besoins
    int numSubSections = 5; // Nombre de sous-sections de chaque côté du centre

    for (int i = 1; i <= numSubSections; ++i) {
        float offset = (lineLength / (numSubSections + 1)) * i;
        drawList->AddLine(ImVec2(center.x - subSectionLength / 2, center.y - offset), ImVec2(center.x + subSectionLength / 2, center.y - offset), IM_COL32(255,255,255, 100), winWidth * 0.00075);
        drawList->AddLine(ImVec2(center.x - subSectionLength / 2, center.y + offset), ImVec2(center.x + subSectionLength / 2, center.y + offset), IM_COL32(255,255,255, 100), winWidth * 0.00075);
        drawList->AddLine(ImVec2(center.x - offset, center.y - subSectionLength / 2), ImVec2(center.x - offset, center.y + subSectionLength / 2), IM_COL32(255,255,255, 100), winWidth * 0.00075);
        drawList->AddLine(ImVec2(center.x + offset, center.y - subSectionLength / 2), ImVec2(center.x + offset, center.y + subSectionLength / 2), IM_COL32(255,255,255, 100), winWidth * 0.00075);
    }
    
    
    char buf[64];
    sprintf(buf, "Zoom: %.0fx", zoom); // Formatage du texte avec la valeur de zoom
    zoomLabbel->UpdateText(buf);
    zoomLabbel->Draw();
    ImGui::End();
}


void ZoomTool::Open(){
    zoom = 10;
}

void ZoomTool::zoomIn(){
    if(zoom < 50){
    zoom *= 1.01;}
}

void ZoomTool::zoomOut(){
    if(zoom > 1){
    zoom *= 0.99;}
}

void ZoomTool::Close(){
    closeButtonFonction();
}

void ZoomTool::setCloseButtonFonction(const std::function<void()>& func){
    closeButtonFonction = func;
}

void ZoomTool::checkInputs(){
    if(ImGui::IsKeyDown(ImGuiKey_E)){zoomIn();}
    if(ImGui::IsKeyDown(ImGuiKey_Q)){zoomOut();}
    if(ImGui::IsKeyPressed(ImGuiKey_Escape)){Close();}
}