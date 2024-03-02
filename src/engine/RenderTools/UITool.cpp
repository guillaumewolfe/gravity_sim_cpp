
#include "glad/glad.h"
#include "engine/RenderTools/UITool.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include <iomanip>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include "path_util.h"
#include <algorithm> // For std::transform
#include <cctype> // For std::tolower

UITool::UITool(RenderContext* renderContext) : RenderComponent(renderContext){
    start_time = std::chrono::system_clock::now();
    musicPlayerTool = new MusicPlayerTool(renderContext);

    float fontSize = 20.0f; // Taille de police
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    float fontSizeScaled = fontSize * winWidth / 1920;
    fontUI = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaled);
    searchIcon = new Icon(0.2f, 0.025f,ImVec2(0.025, 0.025),0.50,"../assets/button/search.png",0.7);
    selectionIcon = new Icon(0.2f, 0.025f,ImVec2(0.02, 0.02),1.0,"../assets/button/go.png",0.8);
    searchLabbel = new Labbel(0.225f,0.025f,ImVec4(255,255,255,255),
                            "Search...",14.0f,0.6f);
    searchFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 16.0f * winWidth / 1920.0f);
    typeFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), 12.0f * winWidth / 1920.0f);
}

UITool::~UITool() {
}

void UITool::Draw() {
glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
auto now = std::chrono::steady_clock::now();
frameCount++;
std::chrono::duration<float> elapsed = now - lastFrameTime;
if (elapsed.count() >= 1.0f) { // Update every second
    fps = frameCount / elapsed.count();
    frameCount = 0;
    lastFrameTime = now;
}

draw_rect();
draw_labbels();
draw_buttons();
drawSearchTool();
std::ostringstream fpsStream;
fpsStream << std::fixed << std::setprecision(2) << fps << " FPS";
ImGui::SetCursorPos(ImVec2(winWidth*0.93, winHeight*0.01));
ImGui::Text("%s", fpsStream.str().c_str());
checkScroll();
musicPlayerTool->Draw();
ImGui::End(); 


}

void UITool::draw_rect(){
    float longueur = winWidth * 0.67; // Exemple de taille
    float hauteur = winHeight * 0.06; // Exemple de taille

    // Position Y du bord supérieur du rectangle pour que le bas du rectangle soit au bas de la fenêtre
    float topY = winHeight - hauteur;

    // Position X du bord gauche pour centrer le rectangle
    float leftX = (winWidth - longueur) / 2;

    ImVec2 topLeft = ImVec2(leftX, topY);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 10.0f;

    // Dessinez le rectangle
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(20, 25, 30, 150), // Couleur
                            cornerRadius);

    drawList->AddRect(topLeft, 
                      ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                      IM_COL32(10,10,10,0), // Couleur
                      cornerRadius, 0, 2.0f);


    ImVec2 centerPos = ImVec2(leftX + longueur / 2, topY + hauteur / 2);



    bool connexionSuccess = m_renderContext->systemeSolaire->apiTool->connectionSuccess;
    if(connexionSuccess){
        drawLive(drawList, centerPos);
    } else {
        drawConnexionFailed(drawList, centerPos);
    }


}

void UITool::drawLive(ImDrawList* drawList, ImVec2 centerPos){
if(!*(m_renderContext->isLive)){return;}

float distanceLive = centerPos.x-winWidth*0.12;
std::string text = "Live";
    ImGui::PushFont(fontUI);
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    drawList->AddText(ImVec2(distanceLive+winWidth*0.035, centerPos.y-textSize.y/2), 
                        IM_COL32(150,255,150,200), // Couleur
                        text.c_str());

    drawList->AddCircleFilled(ImVec2(distanceLive+winWidth*0.025, centerPos.y), 
                                winWidth*0.004, 
                                IM_COL32(150,250,150,200), // Couleur
                                40);
    ImGui::PopFont();
}



void UITool::drawConnexionFailed(ImDrawList* drawList, ImVec2 centerPos){
if(!*(m_renderContext->isLive)){return;}

float distanceLive = centerPos.x-winWidth*0.12;
std::string text = "Offline";
ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    drawList->AddText(ImVec2(distanceLive+winWidth*0.033-textSize.x/2, centerPos.y-winHeight*0.01), 
                        IM_COL32(255,150,150,150), // Couleur
                        text.c_str());

}



void UITool::draw_buttons(){
        // Dessinez les boutons
        for (Button* button : m_renderContext->buttons) {
            button->Draw();
        }
        for(ImageButton* button : m_renderContext->imageButtons){
            button->Draw();
        }
}
void UITool::update_time() {
    start_time = std::chrono::system_clock::now();
}

void UITool::draw_labbels() {
    // Utilisez 'start_time' au lieu de 'current_time'
    auto simulated_time = start_time;

    // Obtenez le temps de simulation en secondes et convertissez-le en entier
    auto simulation_time_seconds = static_cast<long long>(*(m_renderContext->simulationTime));

    // Ajoutez cette durée à 'start_time'
    simulated_time += std::chrono::seconds(simulation_time_seconds);
    std::time_t simulated_time_t = std::chrono::system_clock::to_time_t(simulated_time);

    // Formatez la date et l'heure
    std::ostringstream stream;
    stream << std::put_time(std::localtime(&simulated_time_t), "%Y-%m-%d %H:%M:%S");
    std::string newDateTime = stream.str();

    // Mettez à jour le texte du label avec la nouvelle date et heure
    if (m_renderContext->labels.size() > 0) {
        m_renderContext->labels[0]->UpdateText(newDateTime); 
    }

    // Mettez à jour le second label si nécessaire
    std::string newLabelText2 = m_renderContext->speedSettings[*(m_renderContext->currentSpeedIndex)].second;
    if (m_renderContext->labels.size() > 1) {
        m_renderContext->labels[1]->UpdateText(newLabelText2);
    }

    // Dessinez les labels
    for (Labbel* label : m_renderContext->labels) {
        label->Draw();
    }

    m_renderContext->setCurrentTime(simulated_time);



}


void UITool::checkScroll(){
    if(m_renderContext->mouseIsOnMinimap){return;}
    if(m_renderContext->showSaveSimulation){return;}
    if(ImGui::IsWindowHovered()){
        if(ImGui::GetIO().MouseWheel != 0){
            if(ImGui::GetIO().MouseWheel > 0){
                bool in = true;
                    m_renderContext->currentCamera->zoomByDistance(in,0.075);
                    }
            else{
                bool in = false;
                m_renderContext->currentCamera->zoomByDistance(in,0.075);
                    }
        }
    }
}


void UITool::drawSearchTool(){
    ImGui::PushFont(searchFont);
    searchItem();
    float longueur = winWidth*0.125;
    float hauteur = winHeight*0.035f;
    ImVec2 centerPos = ImVec2(winWidth*0.2f+longueur/2-winWidth*0.025*0.5, winHeight*0.025f);
    //draw rec

    ImVec2 topLeft = ImVec2(centerPos.x-longueur/2, centerPos.y-hauteur/2);
    ImVec2 bottomRight = ImVec2(centerPos.x+longueur/2, centerPos.y+hauteur/2);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = winWidth*0.01;

    // Dessinez le rectangle
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30, 150), // Couleur
                            cornerRadius);

    searchIcon->Draw();
    if(!isSearching)searchLabbel->Draw();
    bool isHovering = false;
    bool isClicked = false;

    //If i click on the search bar, isSearching is true
    ImVec2 mousePos = ImGui::GetMousePos();
    if(mousePos.x > topLeft.x && mousePos.x < bottomRight.x && mousePos.y > topLeft.y && mousePos.y < bottomRight.y){
        isHovering = true; 
        ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);   
    }
    if(ImGui::IsMouseClicked(0)){
        isClicked = true;
    }
    if(isHovering and isClicked){
        isSearching = true;
    }else if ((isClicked and !isHovering and !isHoveringSearchBoxes) or ImGui::IsKeyPressed(ImGuiKey_Escape)){
        isSearching = false;
        searchInput = "";
    }
    int alpha = isHovering ? 5 : 0;
    if (isClicked || isSearching) {
        alpha = 10;
    }

    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(255, 255, 255, alpha), // Couleur
                            cornerRadius);        
    drawList->AddRect(topLeft, 
                        bottomRight, 
                        IM_COL32(255, 255, 255, alpha*2), // Couleur
                        cornerRadius,0,winWidth*0.003);     

    if (isSearching) {
        // Time-based visibility toggle for the cursor
        float time = ImGui::GetTime();
        float cursorWidth = winWidth*0.0012; // Width of the cursor in pixels
        float cursorHeight = hauteur * 0.5f; // Cursor height
        ImVec2 positionCursor = ImVec2(topLeft.x+winWidth*0.024+ImGui::CalcTextSize(searchInput.c_str()).x, centerPos.y);
        ImVec2 cursorTopLeft = ImVec2(positionCursor.x - cursorWidth/2, centerPos.y - cursorHeight / 2);
        ImVec2 cursorBottomRight = ImVec2(positionCursor.x+cursorWidth/2, centerPos.y + cursorHeight / 2);

        // Flashing effect: Toggle visibility every 0.5 seconds
        if (static_cast<int>(time * 2) % 2 == 0) {
            drawList->AddRectFilled(cursorTopLeft, cursorBottomRight, IM_COL32(255, 255, 255, 150));
        }


    // Draw the current search input text
        ImVec2 textSize = ImGui::CalcTextSize(searchInput.c_str());
        drawList->AddText(ImVec2(topLeft.x + winWidth*0.024, centerPos.y - textSize.y / 2), IM_COL32(255, 255, 255, 150), searchInput.c_str());
        ImGuiIO& io = ImGui::GetIO();

        // Handle character input (new text input)
        if (!io.InputQueueCharacters.empty()) {
            for (auto c : io.InputQueueCharacters) {
                if (c > 0 && c < 0x10000) {
                    // Append valid characters to your search input
                    //Check if enough space
                    if(ImGui::CalcTextSize(searchInput.c_str()).x < longueur-winWidth*1.5*0.024){
                    searchInput += static_cast<char>(c);
                    }
                }
            }
        }

        // Handle backspace separately
        if (ImGui::IsKeyPressed(ImGuiKey_Backspace) && !searchInput.empty()) {
            searchInput.pop_back();
        }
    }
    ImGui::PopFont();
}





















//SEARCH ITEM





void UITool::searchItem() {
    isHoveringSearchBoxes = false;
    foundObjects.clear();
    if(!isSearching) return;
    if(searchInput.empty()) return;
    // Convert search input to lower case for case-insensitive comparison
    std::string searchInputLower = searchInput;
    std::transform(searchInputLower.begin(), searchInputLower.end(), searchInputLower.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    for (auto& planete : m_renderContext->systemeSolaire->objects) {
        // Convert planet name to lower case
        std::string nameLower = planete->getName();
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        // Perform a case-insensitive search
        if (nameLower.find(searchInputLower) != std::string::npos) {
            foundObjects.push_back(planete);
        }
    }
    DrawSearchBoxes();

}

void UITool::DrawSearchBoxes(){

    if(foundObjects.size() == 0){return;}

    float xPos = winWidth*0.2f;
    float hauteur = winHeight*0.06f;
    float longueur = winWidth*0.125;
    float padding = winHeight*0.0075f;
    ImVec2 centerPos = ImVec2(xPos+longueur/2-winWidth*0.025*0.5, winHeight*0.025f);
    ImVec2 topLeft = ImVec2(centerPos.x-longueur/2, centerPos.y-hauteur/2);
    ImVec2 bottomRight = ImVec2(centerPos.x+longueur/2, centerPos.y+hauteur/2);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = winWidth*0.005;


    //Big rectangle 
    int totalIndex = foundObjects.size();
    float paddingSquare = winHeight*0.01f;
    cornerRadius = winWidth*0.01;
    if(totalIndex > 0){
        ImVec2 topLeft = ImVec2(centerPos.x-longueur/2-paddingSquare, centerPos.y-winHeight*0.035f/2-paddingSquare/2);
        ImVec2 bottomRight = ImVec2(centerPos.x+longueur/2+paddingSquare, centerPos.y+hauteur/2+hauteur*(totalIndex)+padding*(totalIndex)+paddingSquare/4);
        ImVec2 mousePos = ImGui::GetMousePos();
        isHoveringSearchBoxes = mousePos.x > topLeft.x && mousePos.x < bottomRight.x && mousePos.y > topLeft.y && mousePos.y < bottomRight.y;
        drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0, 0, 0, 75), // Couleur
                            cornerRadius, 0);
        drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30,175), // Couleur
                            cornerRadius, 0);
        drawList->AddRect(topLeft, 
                            bottomRight, 
                            IM_COL32(255,255,255,0), // Couleur
                            cornerRadius, 0, winWidth*0.0015);
    }




    //Small Rectangles
    int index = 0;
    for(auto& object: foundObjects){
        ImVec2 topLeft = ImVec2(centerPos.x-longueur/2-paddingSquare/2, centerPos.y-hauteur/2+hauteur*(index+1)+padding*index+padding*0.5f);
        ImVec2 bottomRight = ImVec2(centerPos.x+longueur/2+paddingSquare/2, centerPos.y+hauteur/2+hauteur*(index+1)+padding*index+padding*0.5f);
        ImVec2 mousePos = ImGui::GetMousePos();
        bool isHovering = mousePos.x > topLeft.x && mousePos.x < bottomRight.x && mousePos.y > topLeft.y && mousePos.y < bottomRight.y;
        bool isClicked = ImGui::IsMouseClicked(0);
        bool isMouseReleased = ImGui::IsMouseReleased(0);
        float alpha = isHovering ? 100 : 0;
        if(isClicked) alpha = 200;
        cornerRadius = winWidth*0.0075;
        drawList->AddRectFilled(topLeft, 
                                bottomRight, 
                                IM_COL32(255, 255, 255, alpha*0.2), // Couleur
                                cornerRadius);
        ImVec2 point1 = ImVec2(topLeft.x+paddingSquare/2, topLeft.y+hauteur*0.25);
        ImVec2 point2 = ImVec2(topLeft.x+paddingSquare/2, bottomRight.y-hauteur*0.25);
        drawList->AddLine(point1, point2, IM_COL32(255,255,255, alpha*4), winWidth*0.002);


        ImVec2 centerPos = ImVec2(topLeft.x + longueur / 2, topLeft.y + hauteur / 2);
        ImVec2 textSize = ImGui::CalcTextSize(object->getName().c_str());
        ImVec2 textPos = ImVec2(centerPos.x - textSize.x / 2, centerPos.y-hauteur*(0.5-0.333) - textSize.y / 2);
        std::string text = object->getName();
        ImGui::PushFont(searchFont);
        float alphaText = isHovering ? 255 : 175;
        drawList->AddText(textPos, IM_COL32(255, 255, 255, alphaText), text.c_str());
        ImGui::PopFont();
        //Add type name
        ImGui::PushFont(typeFont);
        std::string type = m_renderContext->colorByTypeDict[object->type].first;
        ImVec2 typeSize = ImGui::CalcTextSize(type.c_str());
        ImVec2 typePos = ImVec2(centerPos.x - typeSize.x / 2, centerPos.y+hauteur*(0.5-0.333) - typeSize.y / 2);
        ImVec4 color = m_renderContext->colorByTypeDict[object->type].second;
        drawList->AddText(typePos, IM_COL32(color.x, color.y, color.z, alphaText*0.8), type.c_str());
        ImGui::PopFont(); 
        index++;


        if(isMouseReleased && isHovering){
            planetSelectionSearch(object);
        }

        if(isHovering){
            selectionIcon->UpdatePosition((centerPos.x+longueur/2.5)/winWidth, centerPos.y/winHeight);
            selectionIcon->Draw();
        }
    }



}

void UITool::planetSelectionSearch(CelestialObject* planet){
    m_renderContext->currentCamera->newFollowObject(planet);
    m_renderContext->currentCamera->selectedObject = planet;
    *(m_renderContext->showInfo) = true;
    *(m_renderContext->isOrbiting) = true;
    isSearching = false;
    searchInput = "";
}



void UITool::drawPlanet(){
    if(foundObjects.size() == 0){return;}
    if(searchInput == ""){return;}
    float hauteur = winHeight*0.06f;
    float rayon = 0.25f*hauteur;
    float xPos = winWidth*0.2f;
    float longueur = winWidth*0.125;
    float padding = winHeight*0.0075f;
    float numSlices = 40;
    float numSegments = 40;
    ImVec2 centerPos = ImVec2(xPos+longueur/2-winWidth*0.025*0.5, winHeight*0.025f);
    int index = 0;



    for(auto& object: foundObjects){

        ImVec2 planetPos = ImVec2(0.2f*winWidth, winHeight-(centerPos.y+hauteur*(index+1)+padding*index+padding*0.5f));        

        Vec3 translation = Vec3(planetPos.x, planetPos.y,0);
        float scaledRadius = rayon;
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        int winWidth, winHeight;
        glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
        glOrtho(0, winWidth, 0, winHeight, -scaledRadius, scaledRadius);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();


        glTranslatef(translation.x, translation.y, translation.z);
        glRotatef(90,1,0,0);
        glRotatef(Rotation,0,0,-1);
        if(Rotation==360){Rotation-=360;}

        glBindTexture(GL_TEXTURE_2D, object->getTexture());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glEnable(GL_TEXTURE_2D);


        const float PI = 3.14159265358979323846;
        const float PI_2 = PI * 2.0f;

        for (int i = 0; i < numSlices; ++i) {
            float phi1 = static_cast<float>(i) / static_cast<float>(numSlices) * PI;
            float phi2 = static_cast<float>(i + 1) / static_cast<float>(numSlices) * PI;

            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= numSegments; ++j) {
                float theta = static_cast<float>(j) / static_cast<float>(numSegments) * PI_2;

                float x1 = scaledRadius * sinf(phi1) * cosf(theta);
                float y1 = scaledRadius * sinf(phi1) * sinf(theta);
                float z1 = scaledRadius * cosf(phi1);

                float x2 = scaledRadius * sinf(phi2) * cosf(theta);
                float y2 = scaledRadius * sinf(phi2) * sinf(theta);
                float z2 = scaledRadius * cosf(phi2);

                // Calcul des coordonnées de texture en fonction de l'angle theta et phi
                float u1 = static_cast<float>(j) / static_cast<float>(numSegments);
                float v1 = static_cast<float>(i) / static_cast<float>(numSlices);

                float u2 = static_cast<float>(j) / static_cast<float>(numSegments);
                float v2 = static_cast<float>(i + 1) / static_cast<float>(numSlices);

                glTexCoord2f(u1, v1);
                glVertex3f(x1, y1, z1);

                glTexCoord2f(u2, v2);
                glVertex3f(x2, y2, z2);
            }
            glEnd();
        }

        glDisable(GL_TEXTURE_2D);

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        index++;
    }
    Rotation+=0.2;
}