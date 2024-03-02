

#include "glad/glad.h"
#include "engine/RenderTools/zoomTool.h"
#include <GLFW/glfw3.h>
#include "path_util.h"
#include <sstream>
#include <locale>
#include <iomanip>
#include "engine/RenderTools/soundTool.h"


ZoomTool::ZoomTool(RenderContext* renderContext) : RenderComponent(renderContext){
   // Création de la caméra avec la position, la cible et l'orientation vers le haut
   Vec3 zoomCameraPosition(0, 10, 10); // Position de la caméra
   Vec3 zoomCameraTarget(0, 0, 0);       // Cible (le soleil)
   Vec3 zoomCameraUp(0, 1, 0);           // Orientation vers le haut


   zoomCamera = new Camera(zoomCameraPosition, zoomCameraTarget, zoomCameraUp);
   zoomCamera->setContext(renderContext);


   // Réglage de l'angle de perspective pour le zoom
   double zoomAnglePerspective = 5; // Petit angle pour un zoom important
   zoomCamera->angle_perspective = zoomAnglePerspective;
   backgroundTool = new BackgroundTool(m_renderContext);
   objectsTool = new ObjectsTool(m_renderContext, zoomCamera);
   zoomLabbel = new Labbel(0.5,0.9,ImVec4(255,255,255,255),"Zoom",30.0f,0.7);
   backgroundImageTool = new BackgroundImageTool(m_renderContext);
   backgroundImageTool->m_camera = zoomCamera;
   backgroundTool->camera = zoomCamera;
   generate_colors();
   nameFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 22.0);
}
// Destructeur
ZoomTool::~ZoomTool() {
   delete zoomCamera;
   delete backgroundTool;
   delete objectsTool;
   delete zoomLabbel;
   delete title;
   delete radius1;
   delete radius1Static;
   delete radius2;
   delete mass1;
   delete mass1Static;
   delete mass2;
   delete pourcentageMass;
   delete pourcentageRadius;
   delete selectLabbel1;
   delete selectLabbel2;
   delete exitButton;
   delete nameFont;
}

void ZoomTool::Draw() {
   checkInputs();
   double mouseX, mouseY;
   glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
   // Obtenir les dimensions de la fenêtre
   glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
   // Activer le scissor test et définir la zone de scissor
   glfwGetFramebufferSize(glfwGetCurrentContext(), &bufferWidth, &bufferHeight);
   float screenScale = static_cast<float>(bufferWidth) / winWidth;
   winWidth *= screenScale;
   winHeight *= screenScale;


   int zoomViewSize = winHeight / 1.5; // Taille du carré de zoom
   int halfScreenWidth = winWidth / 2;
   int fullScreenHeight = winHeight;
  
   glEnable(GL_SCISSOR_TEST);
   glScissor(halfScreenWidth, 0, halfScreenWidth, fullScreenHeight);


   // Définir la zone de vue pour la caméra de zoom sur la moitié droite
   glViewport(halfScreenWidth, 0, halfScreenWidth, fullScreenHeight);


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
   // zoomCamera->position = m_renderContext->currentCamera->getPosition();
   // zoomCamera->up = m_renderContext->currentCamera->getUp();
   zoomCamera->Update();
   // angle_pespective = m_renderContext->currentCamera->getAnglePerspective()/zoom;
   // // Réduire de moitié l'angle de perspective pour le zoom
   // zoomCamera->angle_perspective = angle_pespective;
   float zoomAspectRatio = static_cast<float>(winWidth/2) / winHeight; // Carré dans ce cas
   zoomCamera->setCustomPerspective(zoomAspectRatio);


   // // Mettre à jour la caméra de zoom
   // zoomCamera->Update();
   if(!obj1IsBigger){
       m_renderContext->currentCamera->comparingDistance = zoomCamera->followingDistance;
   }else{
       zoomCamera->followingDistance = m_renderContext->currentCamera->followingDistance;
   }
   // Rendu de la scène avec la caméra de zoom
   backgroundImageTool->Draw();
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
   if(obj1 == nullptr){return;}
   if(obj2 == nullptr){return;}
   glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
   ImGui::SetNextWindowPos(ImVec2(0, 0));
   ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    

   ImGui::Begin("ZoomToolOverlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
   ImDrawList* drawList = ImGui::GetWindowDrawList();
   checkScroll();
   //Add rect in top center of screen
   rectWidth = winWidth*0.72;
   rectHeight = winHeight*0.15;
   float rectX = winWidth/2 - rectWidth/2;
   float rectY = winHeight*1-rectHeight;
   ImVec2 currentTopLeft = ImVec2(rectX, rectY);
   ImVec2 bottomRight = ImVec2(rectX + rectWidth, rectY + rectHeight*1.4);
   float cornerRadius = winWidth*0.02;
   drawList->AddLine(ImVec2(winWidth/2, 0), ImVec2(winWidth/2, winHeight), ImColor(255, 255, 255, 100), winWidth*0.002);


   drawList->AddRectFilled(currentTopLeft,
                           bottomRight, // Le coin inférieur droit reste fixe
                           IM_COL32(0,0,0, 255), // Color
                           cornerRadius);
   drawList->AddRectFilled(currentTopLeft,
                           bottomRight, // Le coin inférieur droit reste fixe
                           IM_COL32(20, 25, 30, 150), // Color
                           cornerRadius);




   //Add lign in center of screen from top to bottom
   drawPlanets();
   //Draw labbels
   title->Draw();
   radius1->Draw();
   radius1Static->Draw();
   radius2->Draw();
   mass1->Draw();
   mass1Static->Draw();
   mass2->Draw();
   pourcentageMass->Draw();
   pourcentageRadius->Draw();
   selectLabbel1->Draw();
   selectLabbel2->Draw();
   exitButton->Draw();
   drawCombo1();
   drawCombo2();
   //Put multiple lines in the middle but with increased width and decreased alpha
   ImGui::End();
}








void ZoomTool::Open(){
    obj1 = nullptr;
    obj2 = nullptr;

    for(auto& obj : m_renderContext->systemeSolaire->objects){
        if(obj2 == nullptr){
            obj2 = obj;
        }else if(obj1 == nullptr){
            obj1 = obj;
        }else{
            if(obj->getRayon() > obj2->getRayon()){
                obj1 = obj2;
                obj2 = obj;
            }else if(obj->getRayon() > obj1->getRayon()){
                obj1 = obj;
            }
        }
    }

   selectedObject = obj1;
   changeOjects();
   zoomCamera->newFollowObject(obj2);
   m_renderContext->currentCamera->newFollowObject(obj1);
   m_renderContext->currentCamera->isComparing = true;
}


void ZoomTool::changeOjects(){
   //Check who's radius is bigger
   obj1IsBigger = obj1->getRayon() > obj2->getRayon();
   if(obj1IsBigger){
       m_renderContext->currentCamera->useCompareDistance = false;
   }else{
       m_renderContext->currentCamera->useCompareDistance = true;
   }


   bool isDifferent = m_renderContext->currentCamera->followedObject != obj1;
   if(isDifferent){
       m_renderContext->currentCamera->resetOrbits();
   }
   isDifferent = zoomCamera->followedObject != obj2;
   if(isDifferent){
       zoomCamera->resetOrbits();
   }


   zoomCamera->newFollowObject(obj2);
   m_renderContext->currentCamera->newFollowObject(obj1);

   updateLabels();
}


std::string formatNumberWithSpaces(double value) {
   std::ostringstream stream;
   stream << std::fixed << std::setprecision(1) << value;
   std::string number = stream.str();


   // Trouver la position du point (ou la fin de la chaîne si aucun point n'est trouvé)
   size_t pointPos = number.find('.');


   // Insérer des espaces tous les trois chiffres avant le point
   int insertPosition = pointPos - 3;
   while (insertPosition > 0) {
       number.insert(insertPosition, " ");
       insertPosition -= 3;
   }


   return number;
}


void ZoomTool::updateLabels(){


double mass1 = obj1->getWeight();
double mass2 = obj2->getWeight();
double radius1 = obj1->real_radius*2;
double radius2 = obj2->real_radius*2;

if(mass1 == mass2){
   pourcentageMass->isHidden = true;
}else{
   pourcentageMass->isHidden = false;
}


if(radius1 == radius2){
   pourcentageRadius->isHidden = true;
}else{
   pourcentageRadius->isHidden = false;
}

std::ostringstream mass1Stream;
mass1Stream << std::scientific << std::setprecision(2) << mass1;
std::string mass1String = mass1Stream.str();


std::ostringstream mass2Stream;
mass2Stream << std::scientific << std::setprecision(2) << mass2;
std::string mass2String = mass2Stream.str();


std::ostringstream radius1Stream;
radius1Stream << std::scientific << std::setprecision(2) << radius1;
std::string radius1String = radius1Stream.str();


std::ostringstream radius2Stream;
radius2Stream << std::scientific << std::setprecision(2) << radius2;
std::string radius2String = radius2Stream.str();


this->mass1->UpdateText(mass1String + " kg");
this->mass2->UpdateText(mass2String + " kg");
this->radius1->UpdateText(radius1String + " km");
this->radius2->UpdateText(radius2String + " km");


float Width = 0.7;
float Height = 0.15;


float distance_x = Width/5;
bool mass1IsBigger = mass1 > mass2;
bool radius1IsBigger = radius1 > radius2;


float coeffMass = mass1IsBigger ? -1.0 : 1.0;
float coeffRadius = radius1IsBigger ? -1.0 : 1.0;


const float MAX_VALUE = 999999.0f;


float massRatio = mass1IsBigger ? mass1 / mass2 : mass2 / mass1;
massRatio = (massRatio > MAX_VALUE) ? MAX_VALUE : massRatio;


float radiusRatio = radius1IsBigger ? radius1 / radius2 : radius2 / radius1;
radiusRatio = (radiusRatio > MAX_VALUE) ? MAX_VALUE : radiusRatio;






std::string massRatioString = formatNumberWithSpaces(massRatio);
std::string radiusRatioString = formatNumberWithSpaces(radiusRatio);




pourcentageMass->UpdateText("(x" + massRatioString + ")");
pourcentageRadius->UpdateText("(x" + radiusRatioString + ")");
pourcentageRadius->UpdatePosition(0.5+distance_x/2*coeffRadius,1-1.8*Height/3+0.01);
pourcentageMass->UpdatePosition(0.5+distance_x/2*coeffMass,1-1*Height/3+0.01);




}


void ZoomTool::zoomIn(float zoom){
   if(obj1IsBigger){
       m_renderContext->currentCamera->zoomByDistance(true,zoom);}
   else{zoomCamera->zoomByDistance(true,zoom);}
}


void ZoomTool::zoomOut(float zoom){
   if(obj1IsBigger){
       m_renderContext->currentCamera->zoomByDistance(false,zoom);}
   else{zoomCamera->zoomByDistance(false,zoom);}
}


void ZoomTool::Close(){
   closeButtonFonction();
   m_renderContext->currentCamera->resetPosition();
   zoomCamera->resetPosition();
}

void ZoomTool::exitButtonFunction(){
    *escapeClicked = true;
    Close();
}

void ZoomTool::setCloseButtonFonction(const std::function<void()>& func){
   closeButtonFonction = func;
}


void ZoomTool::checkInputs(){
    checkScroll();
   if(ImGui::IsKeyDown(ImGuiKey_E)){zoomIn();}
   if(ImGui::IsKeyDown(ImGuiKey_Q)){zoomOut();}
   if(ImGui::IsKeyPressed(ImGuiKey_Escape)){Close();}
   if(ImGui::IsKeyDown(ImGuiKey_A)){
       zoomCamera->orbitAroundObject(-0.01,0);
       m_renderContext->currentCamera->orbitAroundObject(-0.01,0);}
   if(ImGui::IsKeyDown(ImGuiKey_D)){
       zoomCamera->orbitAroundObject(0.01,0);
       m_renderContext->currentCamera->orbitAroundObject(0.01,0);}
   if(ImGui::IsKeyDown(ImGuiKey_W)){
       zoomCamera->orbitAroundObject(0,0.01);
       m_renderContext->currentCamera->orbitAroundObject(0,0.01);}
   if(ImGui::IsKeyDown(ImGuiKey_S)){
       zoomCamera->orbitAroundObject(0,-0.01);
       m_renderContext->currentCamera->orbitAroundObject(0,-0.01);}
   //Get mouse pos, if clicked on first half of screen, SelectedObject = obj1, else obj2
   double mouseX, mouseY;
   ImVec2 mouse_pos = ImGui::GetMousePos();
   mouseX = mouse_pos.x;
   mouseY = mouse_pos.y;
   if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
       if(mouseX < winWidth/2){
           selectedObject = obj1;


       }else{
           selectedObject = obj2;
       }
   }
   zoomCamera->orbitAroundObject(0.0001,0);
   m_renderContext->currentCamera->orbitAroundObject(0.0001,0);
}


void ZoomTool::checkScroll(){
    if(ImGui::GetIO().MouseWheel != 0){
        if(ImGui::GetIO().MouseWheel > 0){
            bool in = true;
                zoomIn(0.1);
                }
        else{
            bool in = false;
                zoomOut(0.1);
                }
    }

    if (ImGui::IsMouseDragging(0, 0.0f)) {
        ImVec2 delta = ImGui::GetMouseDragDelta(0, 0.0f);
        //Is mouse is in the first half of screenScale
        if(ImGui::GetMousePos().x < winWidth/2){
            m_renderContext->currentCamera->orbitAroundObject(-delta.x*0.005, delta.y*0.005);
        }else{
            zoomCamera->orbitAroundObject(-delta.x*0.005, delta.y*0.005);
        }
        ImGui::ResetMouseDragDelta(0);
    }
}




void ZoomTool::drawPlanets(){
float posY = winHeight-rectHeight/2;
float posX = winWidth/2;
CelestialObject* objBiggestRadius = obj1IsBigger ? obj1 : obj2;
CelestialObject* objSmallestRadius = obj1IsBigger ? obj2 : obj1;
float ratio = objBiggestRadius->getRayon()/objSmallestRadius->getRayon();


float planetRadius1 = winHeight*0.025;//Biggest
float planetRadius2 = planetRadius1;//Smallest


if(planetRadius2<winHeight*0.001){
   planetRadius2 = winHeight*0.001;
}


//Draw planet 1
ImVec2 planet1Pos = ImVec2(posX-rectWidth/2*0.9, posY);
ImVec2 planet2Pos = ImVec2(posX+rectWidth/2*0.9, posY);
ImVec4 colorCenterDotPlanet1 = typeDictColor[obj1->typeName];
ImVec4 colorCenterDotPlanet2 = typeDictColor[obj2->typeName];
ImDrawList* drawList = ImGui::GetWindowDrawList();


float object1radius = obj1IsBigger ? planetRadius1 : planetRadius2;
float object2radius = obj1IsBigger ? planetRadius2 : planetRadius1;


//Add line in between
drawList->AddCircleFilled(planet1Pos, object1radius, IM_COL32(colorCenterDotPlanet1.x,colorCenterDotPlanet1.y,colorCenterDotPlanet1.z, 255), 100);
drawList->AddCircleFilled(planet2Pos, object2radius, IM_COL32(colorCenterDotPlanet2.x,colorCenterDotPlanet2.y,colorCenterDotPlanet2.z, 255), 100);


//Add white circle if planet = SelectedPlanet


// if(selectedObject == obj1){
//     drawList->AddCircle(planet1Pos, object1radius*1.1, IM_COL32(255,255,255, 255), 100,winHeight*0.001);
// }else if(selectedObject == obj2){
//     drawList->AddCircle(planet2Pos, object2radius*1.1, IM_COL32(255,255,255, 255), 100,winHeight*0.001);
// }




//if sun
if(obj1->type == 1){
   drawSunEffect(planet1Pos, object1radius);
}else{
   drawPlanetEffect(planet1Pos, object1radius, obj1);
}
if(obj2->type == 1){
   drawSunEffect(planet2Pos, object2radius);
}else{
   drawPlanetEffect(planet2Pos, object2radius, obj2);
}






}
void ZoomTool::drawPlanetEffect(ImVec2 planetPos, float radius, CelestialObject* obj){
   ImDrawList* drawList = ImGui::GetWindowDrawList();
   ImVec4 colorCenterDot = typeDictColor[obj->typeName];
   float numBlurCircles = 20;
   float blurIncrease = radius*0.02; // How much larger each successive blur circle is
   float initialAlpha = 30; // Starting alpha value for the outermost blur circle
   float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
   for (int i = 0; i < numBlurCircles; ++i) {
       float blurRadius = radius + blurIncrease * (i + 1);
       float alpha = initialAlpha - alphaDecrease * i;
       drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);}
}


void ZoomTool::drawSunEffect(ImVec2 planetPos, float radius){
   ImDrawList* drawList = ImGui::GetWindowDrawList();
   ImVec4 colorCenterDot = ImVec4(255,215,80,255);
   float numBlurCircles = 30;
   float blurIncrease = radius*0.03; // How much larger each successive blur circle is
   float initialAlpha = 20; // Starting alpha value for the outermost blur circle
   float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
   for (int i = 0; i < numBlurCircles; ++i) {
       float blurRadius = radius + blurIncrease * (i + 1);
       float alpha = initialAlpha - alphaDecrease * i;
       drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);}
}


void ZoomTool::drawCombo1(){
   if(obj1 == nullptr){return;}
   if(obj2 == nullptr){return;}
   bool comboOpen = false;
   ImGui::PushFont(nameFont);
   ImVec2 center = pos1Name;
   center.x *= winWidth;
   center.y *= winHeight;
   ImDrawList* drawList = ImGui::GetWindowDrawList();
   ImVec2 textSize = ImGui::CalcTextSize(obj1->getName().c_str());




   ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.0)); // Couleur en RGBA
   ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.1)); // Couleur en RGBA
   ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.1f)); // Couleur de sélection du texte
   ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
   ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.3f)); // Survol
   ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 0.3f)); // Sélectionné
   ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texte
   CelestialObject* newObj = nullptr;


   ImGui::SetCursorPos(ImVec2(center.x-textSize.x/2, center.y-textSize.y/2));
   ImGui::SetNextItemWidth(textSize.x*1.2);
   if (ImGui::BeginCombo("##POV TARGET2", obj1->getName().c_str(), ImGuiComboFlags_NoArrowButton)) {
       comboOpen = true;
       for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
           if(otherObj == obj1){continue;}
           std::string name = otherObj->getName();
           std::string item = name;
           bool isSelected = (obj1->getName() == item);
           if (ImGui::Selectable(item.c_str(), isSelected)) {
               newObj = otherObj;
           }
           if (isSelected) {
               ImGui::SetItemDefaultFocus();
           }
       }
       ImGui::EndCombo();
   }


   if (newObj != nullptr) { // Vérifier si newPOVtarget n'est pas null
       obj1 = newObj;
       changeOjects();
   }


   ImGui::PopStyleColor(7); // Pop toutes les couleurs
   ImGui::PopFont();
}


void ZoomTool::drawCombo2(){
   bool comboOpen = false;
   ImGui::PushFont(nameFont);
   ImVec2 center = pos2Name;
   center.x *= winWidth;
   center.y *= winHeight;
   ImDrawList* drawList = ImGui::GetWindowDrawList();
   ImVec2 textSize = ImGui::CalcTextSize(obj2->getName().c_str());




   ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.0)); // Couleur en RGBA
   ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.1)); // Couleur en RGBA
   ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.1f)); // Couleur de sélection du texte
   ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
   ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.3f)); // Survol
   ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 0.3f)); // Sélectionné
   ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texte
   CelestialObject* newObj = nullptr;


   ImGui::SetCursorPos(ImVec2(center.x-textSize.x/2, center.y-textSize.y/2));
   ImGui::SetNextItemWidth(textSize.x*1.2);
   if (ImGui::BeginCombo("##POV TARGET", obj2->getName().c_str(), ImGuiComboFlags_NoArrowButton)) {
       comboOpen = true;
       for (auto& otherObj : m_renderContext->systemeSolaire->objects) {
           if(otherObj == obj2){continue;}
           std::string name = otherObj->getName();
           std::string item = name;
           bool isSelected = (obj2->getName() == item);
           if (ImGui::Selectable(item.c_str(), isSelected)) {
               newObj = otherObj;
           }
           if (isSelected) {
               ImGui::SetItemDefaultFocus();
           }
       }
       ImGui::EndCombo();
   }


   if (newObj != nullptr) { // Vérifier si newPOVtarget n'est pas null
       obj2 = newObj;
       changeOjects();
   }


   ImGui::PopStyleColor(7); // Pop toutes les couleurs
   ImGui::PopFont();
}


void ZoomTool::generate_colors() {
      std::function<void(std::string)> playSoundFunc =
   std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);
   rectWidth = 0.7;
   rectHeight = 0.15;
   float rectX = winWidth/2 - rectWidth/2;
   float rectY = winHeight*1-rectHeight;


    float top = 1-0.15;
    float top1_3 = 1-0.15/3;
    float top2_3 = 1-0.15*2/3;
    float diff = 0.01;
   float distance_x = rectWidth/5;
   title = new Labbel(0.5,1-0.13+diff,ImVec4(255,255,255,255),"Comparison",27.0f,0.7);
   radius1 = new Labbel(0.5-distance_x,1-1.8*rectHeight/3+diff,ImVec4(255,255,255,255),"200 KM",24.0f,0.7);
   radius1Static = new Labbel(0.5,1-1.8*rectHeight/3+diff,ImVec4(255,255,255,255),"Diameter",22.0f,0.3);
   radius2 = new Labbel(0.5+distance_x,1-1.8*rectHeight/3+diff,ImVec4(255,255,255,255),"200 KM",24.0f,0.7);


   mass1 = new Labbel(0.5-distance_x,1-1*rectHeight/3+diff,ImVec4(255,255,255,255),"3400 KG",24.0f,0.7);
   mass1Static = new Labbel(0.5,1-1*rectHeight/3+diff,ImVec4(255,255,255,255),"Mass",22.0f,0.3);
   mass2 = new Labbel(0.5+distance_x,1-1*rectHeight/3+diff,ImVec4(255,255,255,255),"26700 KG",24.0f,0.7);
  
   pourcentageMass = new Labbel(0.5-distance_x/2,1-1*rectHeight/3+diff,ImVec4(150,255,150,255),"(x10003)",22.0f,0.6);
   pourcentageRadius = new Labbel(0.5-distance_x/2,1-1.8*rectHeight/3+diff,ImVec4(150,255,150,255),"(x231)",22.0f,0.6);




   selectLabbel1 = new Labbel(0.5-rectWidth/2*0.725,1-0.10,ImVec4(255,255,255,255),"Select:",19.0f,0.6);
   selectLabbel2 = new Labbel(0.5+rectWidth/2*0.725,1-0.10,ImVec4(255,255,255,255),"Select:",19.0f,0.6);


   pos1Name = ImVec2(0.5-rectWidth/2*0.725,1-rectHeight/2);
   pos2Name = ImVec2(0.5+rectWidth/2*0.725,1-rectHeight/2);


   exitButton = new Button(playSoundFunc,0.075, 1-0.15/2, ImVec2(0.04, 0.04),
                              ImVec4(75.0/255.0, 25.0/255.0, 30.0/255.0, 1.0f),
                              ImVec4(75.0/255.0, 25.0/255.0, 30.0/255.0,  1.0f),
                              "Exit", 0.6f,20.0,
                              std::bind(&ZoomTool::exitButtonFunction, this),0);










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

