#include "engine/RenderTools/saveSimulation.h"
#include "path_util.h"
#include "engine/RenderTools/soundTool.h"
#include "save/saveTool.h"
#include <sstream> // Pour std::ostringstream
#include <iomanip> // Pour std::setprecision
#include "save/gameState.h"


void SaveSimulationTool::drawSaveSimulation(){

    drawUISaveSimulation();
    drawSolarSystem();
    drawNameInputBox();
    drawGameStatesBoxForSave();
    checkInputs();

}


void SaveSimulationTool::saveSimulationButton(){
    saveCurrentState();
}




void SaveSimulationTool::drawUISaveSimulation(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = winWidth*0.02;
    float longueurSave = winWidth* 0.55; // Exemple de taille
    float hauteurSave = winHeight * 0.6; // Exemple de taille
    ImVec2 centerPos = ImVec2(winWidth * 0.5, winHeight * 0.5);
    ImVec2 topLeftSave = ImVec2(centerPos.x - longueurSave * 0.5, centerPos.y - hauteurSave * 0.5);
    ImVec2 bottomRightSave = ImVec2(centerPos.x + longueurSave * 0.5, centerPos.y + hauteurSave * 0.5);
    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winWidth,winHeight), 
                            IM_COL32(0, 0, 0, 200), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeftSave, 
                            bottomRightSave, 
                            IM_COL32(0, 0, 0, 250), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeftSave, 
                            bottomRightSave, 
                            IM_COL32(20, 25, 30, 200), // Color
                            cornerRadius);

    //Add ling in middle vertical
    ImVec2 lineStart = ImVec2(centerPos.x, centerPos.y-hauteurSave*0.3);
    ImVec2 lineEnd = ImVec2(centerPos.x, centerPos.y+hauteurSave*0.3);
    drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 35), winWidth*0.0015);


    //Add rect in middle of "0.633854, 0.633721"
    lineStart = ImVec2(0.631771*winWidth - longueur*0.3, 0.572674*winHeight);
    lineEnd = ImVec2(0.631771*winWidth + longueur*0.3, 0.572674*winHeight);
    drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 35), winWidth*0.0015);


    saveTitleLabbel->Draw();
    saveSimulationBtn->Draw();
    ExitSaveSimulationBtn->Draw();
    saveIcon->Draw();
    quitSaveBtn->Draw();
    simulationTimeLabel->Draw();
    saveHistorySave->Draw();
    switchStateBtn->Draw();
}

void SaveSimulationTool::drawSolarSystem(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float planetRadius = winHeight*0.0065;
    hoverObject = nullptr;
    float closestDistance = std::numeric_limits<float>::max(); // Pour suivre la distance la plus proche
    bool anyPlanetClicked = false;
    draw_asteroid_belt(asteroid1Positions,winHeight*0.0010, IM_COL32(180, 150, 150, 75));
    draw_asteroid_belt(asteroid2Positions,winHeight*0.0010, IM_COL32(150, 200, 255, 75));
    CelestialObject* sun = m_renderContext->systemeSolaire->getSun();
    ImVec2 hoverObjectPosition;
    for(auto& planete : m_renderContext->systemeSolaire->objects){
        if(planete->type == 5 && planete != m_renderContext->currentCamera->selectedObject) continue; // Ne pas dessiner le centre (par exemple, le Soleil
        ImVec2 relativePosition = ImVec2(planete->getPositionSimulation().x - sun->getPositionSimulation().x, planete->getPositionSimulation().z - sun->getPositionSimulation().z);
        ImVec2 planetPos = distanceConverterSimToLog(relativePosition);
        if(planete == sun){
            planetPos = centerPosSolarSystem;
        }
        ImVec4 planetColor = m_renderContext->getTypeColor(planete->typeName);
        if (checkMouseSelection(planetPos, planete, planetRadius, closestDistance)) {
            anyPlanetClicked = true;
        }
        //DrawObject
        //DrawOrbits
        float distanceFromCenter = std::sqrt(std::pow(planetPos.x - centerPosSolarSystem.x, 2) + std::pow(planetPos.y - centerPosSolarSystem.y, 2));

        //DrawOrbit
        drawList->AddCircle(centerPosSolarSystem,distanceFromCenter, IM_COL32(planetColor.x, planetColor.y, planetColor.z,30),100,planetRadius*0.5);

        //DrawObject
        drawList->AddCircleFilled(planetPos, planetRadius, IM_COL32(planetColor.x, planetColor.y, planetColor.z, 225),100);
    
        //DrawLight
        if(planete->type == 1){
            drawSunEffect(planetPos, planetRadius);
        }else{
            drawPlanetLightNOSHADOW(planetPos, planetRadius, ImVec4(planetColor.x, planetColor.y, planetColor.z, 255));
            //Sun effect
            if(m_renderContext->systemeSolaire->getSun()->type == 1){
                ImVec2 sunPos = distanceConverterSimToLog(ImVec2(m_renderContext->systemeSolaire->getSun()->getPositionSimulation().x, m_renderContext->systemeSolaire->getSun()->getPositionSimulation().z));
                drawPlanetLight(planetPos, sunPos, planetRadius);
            }
        }
        float circleAlpha;
        if(planete == selectedObject){circleAlpha = 255;}
        if(planete == hoverObject){circleAlpha = 150;}
        if(planete == selectedObject || planete == hoverObject){
            drawList->AddCircle(planetPos, planetRadius*1.5, IM_COL32(255,255,255,circleAlpha),100,planetRadius*0.2);
        }

        //DrawSystemMaxSize
        //drawList->AddCircle(centerPosSolarSystem,radiusSolarSystem, IM_COL32(255,255,255,100),100,2.0f);

        //PLANETE NAMES
        ImFont* font = planete == selectedObject ? SelectedplanetNameInSolarSystemFont : planetNamesFont;
        ImGui::PushFont(font);
        std::string planetName = planete->name;
        float padding = planete->type==1?winHeight*0.02:winHeight*0.015;
        ImVec2 textPos = ImVec2(planetPos.x - ImGui::CalcTextSize(planetName.c_str()).x / 2, 
                                planetPos.y - planetRadius - padding-ImGui::CalcTextSize(planetName.c_str()).y / 2);
        float alpha = 100;
        if(planete == selectedObject){alpha = 225;}
        else if(planete == hoverObject){alpha = 175;}
        drawList->AddText(textPos, IM_COL32(255, 255, 255, alpha), planetName.c_str());
        ImGui::PopFont();



        //calculate distance between mouse and centerPosSolarSystem
        if (ImGui::IsMouseClicked(0) && !anyPlanetClicked) {
            selectedObject = nullptr;
        }
        if(planete == selectedObject){
            selectedObjectPosition = planetPos;
            selectedObjectColor = planetColor;
        }
        if(planete == hoverObject){
            hoverObjectPosition = planetPos;
        }
    }   
    if(selectedObject!=nullptr){
        drawInfos(selectedObject, selectedObjectPosition, selectedObjectColor);
    }else if (hoverObject != nullptr){
        drawInfos(hoverObject, hoverObjectPosition, selectedObjectColor);
    }


    std::string simulationTimeString = m_renderContext->getDateTime();
    simulationTimeLabel->UpdateText(simulationTimeString);
    simulationTimeLabel->UpdatePosition(centerPosSolarSystem.x/winWidth,0.7529);

}

bool SaveSimulationTool::checkMouseSelection(ImVec2 planetPos, CelestialObject* planete, float radius, float &closestDistance){
    ImVec2 mousePos = ImGui::GetMousePos();
    bool mouseClicked = ImGui::IsMouseClicked(0);
    float distanceToMouse = std::sqrt(std::pow(mousePos.x - planetPos.x, 2) + std::pow(mousePos.y - planetPos.y, 2));
    if (distanceToMouse < radius * 2) { // Vérifie si la souris est suffisamment proche
        if (distanceToMouse < closestDistance) { // Si cet objet est le plus proche jusqu'à présent
            closestDistance = distanceToMouse; // Mise à jour de la distance la plus proche
            hoverObject = planete; // Mise à jour de l'objet survolé
            if (mouseClicked) {
                selectedObject = planete; // Sélection de l'objet si cliqué
                return true; // Indique qu'une sélection a été faite
            }
        }
    }
    return false; // Aucune sélection faite
}


void SaveSimulationTool::drawNameInputBox(){
    ImGui::PushFont(SelectedplanetNameInSolarSystemFont);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95, 1, 0.95, 0.3)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95, 1, 0.95, 0.2)); // Couleur en RGBA
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.8f, 0.8f, 0.8f, 0.4f)); // Couleur de sélection du texte
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Elément sélectionné
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.7f)); // Survol
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.6f, 1.0f)); // Sélectionné
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.8f)); // Texte

    //GetPosX from solarsystemCenter in X
    float diffX = winWidth*0.5 - centerPosSolarSystem.x;

    char charBuffer[11];
    const char* buffer = "mercuremercure";
    ImVec2 textSize = ImGui::CalcTextSize(buffer, NULL, false, 0.0f);
    float widthEntry = textSize.x*1.12;
    ImGui::PushItemWidth(widthEntry);
    ImVec2 cursorPos = ImVec2(winWidth*0.5 + diffX-widthEntry/2, 0.63*winHeight-textSize.y/2);
    ImGui::SetCursorPos(cursorPos);
    std::strncpy(charBuffer, stateIDName.c_str(), sizeof(charBuffer));
    // Ensure null termination
    memset(charBuffer, 0, sizeof(charBuffer));

    // Or, if initializing with existing data, ensure it doesn't exceed 10 characters
    std::strncpy(charBuffer, stateIDName.c_str(), sizeof(charBuffer) - 1);
    charBuffer[sizeof(charBuffer) - 1] = '\0'; // Ensure null termination
    // Use the buffer with ImGui::InputText
    if (ImGui::InputText("##Planete Name", charBuffer, sizeof(charBuffer))) {
        // If InputText returns true, the buffer was modified.
        // Update the std::string with the new content of the buffer.
        stateIDName = std::string(charBuffer);
    }

    //Si le nom n'est pas vide,
    saveSimulationBtn->isDisabled = false;
    if(stateIDName == ""){
        saveSimulationBtn->isDisabled = true;
    }
    //limit the size of the name to 15 characters
ImDrawList* drawList = ImGui::GetWindowDrawList();
    //Check if the name is already used
    gameStatesIds = saveTool->getGameStatesIds();
    if(std::find(gameStatesIds.begin(), gameStatesIds.end(), stateIDName) != gameStatesIds.end()){
        saveSimulationBtn->isDisabled = true;
    std::string saveName = stateIDName+" already exist";
    ImGui::PushFont(wrongInputNameFont);
    ImVec2 textPos = ImVec2(winWidth*0.5 + diffX-ImGui::CalcTextSize(saveName.c_str()).x / 2, 0.651039*winHeight);
    drawList->AddText(textPos, IM_COL32(255, 0, 0, 255), saveName.c_str());
    ImGui::PopFont();
    
    }
    


    //Add name of input
    std::string saveName = "Save name";
    ImVec2 textPos = ImVec2(winWidth*0.5 + diffX-ImGui::CalcTextSize(saveName.c_str()).x / 2, 0.595*winHeight);

    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), saveName.c_str());
    
    ImGui::PopItemWidth();
    ImGui::PopStyleColor(7);
    ImGui::PopFont();
}

void SaveSimulationTool::drawSaveConfirmed(){
    //Draw small rectangle in middle of screen with a text "Save confirmed", Close after 2 seconds
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueurSave = winWidth* 0.1; // Exemple de taille
    float hauteurSave = winHeight * 0.1; // Exemple de taille
    ImVec2 centerPos = ImVec2(winWidth * 0.5, winHeight * 0.5);
    ImVec2 topLeftSave = ImVec2(centerPos.x - longueurSave * 0.5, centerPos.y - hauteurSave * 0.5);
    ImVec2 bottomRightSave = ImVec2(centerPos.x + longueurSave * 0.5, centerPos.y + hauteurSave * 0.5);
    ImGui::PushFont(SelectedplanetNameInSolarSystemFont);
    drawList->AddRectFilled(topLeftSave, 
                            bottomRightSave, 
                            IM_COL32(0, 0, 0, 255), // Color
                            2.0f);
    drawList->AddRectFilled(topLeftSave,

                            bottomRightSave, 
                            IM_COL32(20, 25, 30, 100), // Color
                            2.0f);
    drawList->AddRect(topLeftSave,
                        bottomRightSave, 
                        IM_COL32(255, 255, 255, 40), // Color
                        2.0f, 0, winWidth*0.0005);
    std::string text = stateIDName + " saved";
    ImVec2 textPos = ImVec2(centerPos.x - ImGui::CalcTextSize(text.c_str()).x / 2, centerPos.y - ImGui::CalcTextSize("Save confirmed").y / 2);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), text.c_str());
    ImGui::PopFont();



    //If more than 2 sec, then call Close()
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = now - timeSaveConfirmed;
    if(elapsed_seconds.count() > 2){
        Close();
    }
}

void SaveSimulationTool::drawInfos(CelestialObject* planete, ImVec2 planetPos, ImVec4 color){
    if(selectedObject == nullptr && hoverObject == nullptr) return;
    CelestialObject* m_object = selectedObject!=nullptr?selectedObject:hoverObject;
    float longueurRect = winWidth*0.1;
    float hauteurRect = winHeight*0.20;
    //Offset should be at the right of the planet, exept if the rectangle will be out of the middleX
    ImVec2 offset;
    float offsetXValue = winWidth*0.025+longueurRect/2;
    if(planetPos.x + longueurRect*0.5 + offsetXValue > winWidth*0.5){
        offset = ImVec2(-offsetXValue,0);}
    else{offset = ImVec2(offsetXValue,0);}
    ImVec2 centerPos = ImVec2(planetPos.x + offset.x, planetPos.y + offset.y);
    ImVec2 topLeft = ImVec2(centerPos.x - longueurRect * 0.5, centerPos.y - hauteurRect * 0.5);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueurRect * 0.5, centerPos.y + hauteurRect * 0.5);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 5.0f;

    //Add line between planet and bottomLeftCorner with a little offset to be in the rect
    ImVec2 lineStart = ImVec2(planetPos.x, planetPos.y);
    ImVec2 lineEnd = ImVec2(topLeft.x+winWidth*0.005, centerPos.y);
    drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 20), winWidth*0.0015);
    drawList->AddCircleFilled(planetPos, winWidth*0.001, IM_COL32(255,255,255, 255), 100);

    //Draw background resetCam
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0, 0, 0, 250), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30, 100), // Color
                            cornerRadius);
    drawList->AddRect(topLeft, 
                      bottomRight, 
                      IM_COL32(255, 255, 255, 60), // Color
                      cornerRadius, 0, 0.2f);



    //Draw planet name
    ImFont* font = SelectedplanetNameInSolarSystemFont;
    ImGui::PushFont(font);
    std::string planetName = planete->name;
    ImVec2 textPos = ImVec2(centerPos.x - ImGui::CalcTextSize(planetName.c_str()).x / 2, 
                            topLeft.y + winHeight*0.0125);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 200), planetName.c_str());
    ImGui::PopFont();
    //Draw small planet in top left corner
    float radius = winHeight*0.0075;
    ImVec2 planetPosTopLeft = ImVec2(bottomRight.x-radius*3, topLeft.y+radius*3);
    color = m_renderContext->getTypeColor(planete->typeName);
    drawList->AddCircleFilled(planetPosTopLeft, radius, IM_COL32(color.x, color.y, color.z, 225),100);
    if(m_object->type == 1){
        drawSunEffect(planetPosTopLeft, radius);}
    else{drawPlanetLightNOSHADOW(planetPosTopLeft, radius, color);}
    
    //Draw an arrow from the planet to its speed direction
    Vec3 speed = planete->velocity.normalize();
    ImVec2 speedDirection = ImVec2(speed.x, speed.z);
    float arrowLength = winHeight*0.025;
    ImVec2 arrowStart = ImVec2(planetPos.x, planetPos.y);
    ImVec2 arrowEnd = ImVec2(planetPos.x + speedDirection.x*arrowLength, planetPos.y + speedDirection.y*arrowLength);
    //Draw arrow head using circleFilled and 3 points
    float arrowHeadSize = arrowLength/3;
// Drawing the arrowhead

    // Calculate normalized direction for the line
    ImVec2 lineDir = ImVec2(speedDirection.x * arrowLength, speedDirection.y * arrowLength);
    float length = sqrt(lineDir.x * lineDir.x + lineDir.y * lineDir.y);
    ImVec2 normDir = ImVec2(lineDir.x / length, lineDir.y / length);

    // Calculate the points for the arrowhead
    ImVec2 perpDir = ImVec2(-normDir.y, normDir.x); // Perpendicular direction to the line
    ImVec2 arrowPoint1 = ImVec2(arrowEnd.x + normDir.x * arrowHeadSize, arrowEnd.y + normDir.y * arrowHeadSize);
    ImVec2 arrowPoint2 = ImVec2(arrowEnd.x + perpDir.x * arrowHeadSize * 0.5f, arrowEnd.y + perpDir.y * arrowHeadSize * 0.5f);
    ImVec2 arrowPoint3 = ImVec2(arrowEnd.x - perpDir.x * arrowHeadSize * 0.5f, arrowEnd.y - perpDir.y * arrowHeadSize * 0.5f);

    // Draw the arrowhead
    if(m_object!=m_renderContext->systemeSolaire->getSun())
    {   drawList->AddLine(arrowStart, arrowEnd, IM_COL32(255, 255, 255, 200), winWidth*0.0015);
        drawList->AddTriangleFilled(arrowPoint1, arrowPoint2, arrowPoint3, IM_COL32(255, 255, 255, 200));}


    drawDistanceFromSunLogo(centerPos,longueurRect,hauteurRect,color);
    drawWeightLogo(centerPos,longueurRect,hauteurRect);
    drawDiameterLodo(centerPos,longueurRect,hauteurRect);

    //Add current time of the simulation at 

}

void SaveSimulationTool::drawDistanceFromSunLogo(ImVec2 centerPos,float longueurRec,float hauteurRec,ImVec4 color){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 position = ImVec2(centerPos.x-longueurRec/2+winHeight*0.0075*3,centerPos.y-hauteurRec/2*0.3);

    CelestialObject* m_object = selectedObject!=nullptr?selectedObject:hoverObject;
    float radius = winHeight*0.0035;
    float diffHauteur = winHeight*0.02;
    ImVec2 planetPos = ImVec2(position.x, position.y+diffHauteur);
    ImVec2 sunPos = ImVec2(position.x, position.y-diffHauteur);
    CelestialObject* sun = m_renderContext->systemeSolaire->getSun();
    ImVec4 sunColor = m_renderContext->getTypeColor(sun->typeName);



    drawList->AddCircleFilled(planetPos, radius, IM_COL32(color.x, color.y, color.z, 225),100);
    if(m_object->type == 1){
        drawSunEffect(planetPos, radius);}
    else{drawPlanetLightNOSHADOW(planetPos, radius, color);}
    drawList->AddCircleFilled(sunPos, radius, IM_COL32(color.x, color.y, color.z, 225),100);
    if(sun->type == 1){
        drawSunEffect(sunPos, radius);}
    else{drawPlanetLightNOSHADOW(sunPos, radius, sunColor);}


    verticalArrow->UpdatePosition(position.x/winWidth,position.y/winHeight);
    verticalArrow->Draw();

    //Compute distance in AU
    ImGui::PushFont(planetNamesFont);
    Vec3 planetePos = m_object->getRealPosition();
    Vec3 sunPosition = m_renderContext->systemeSolaire->getSun()->getRealPosition();
    float distance = std::sqrt(std::pow(planetePos.x - sunPosition.x, 2) + std::pow(planetePos.y - sunPosition.y, 2) + std::pow(planetePos.z - sunPosition.z, 2));
    float distanceAU = distance / 149597870000.7;
    std::string distanceString = std::to_string(distanceAU);
    distanceString = distanceString.substr(0, distanceString.find(".") + 3);
    distanceString += " AU";
    if(m_object == sun){
        distanceString = "-";
    }
    ImVec2 textSize = ImGui::CalcTextSize(distanceString.c_str());
    ImVec2 textPos = ImVec2(centerPos.x - ImGui::CalcTextSize(distanceString.c_str()).x / 2, 
                            position.y-textSize.y/2);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 150), distanceString.c_str());
    ImGui::PopFont();
}

void SaveSimulationTool::drawWeightLogo(ImVec2 centerPos,float longueurRec,float hauteurRec){
    CelestialObject* m_object = selectedObject!=nullptr?selectedObject:hoverObject;
    ImVec2 position = ImVec2(centerPos.x-longueurRec/2+winHeight*0.0075*3,centerPos.y+hauteurRec/2*0.25);
    weightIcon->UpdatePosition(position.x/winWidth,position.y/winHeight);

    //Add weight text in scientific notation with 2 decimal
    std::ostringstream weightStream;
    // Définissez la précision à 2 chiffres après la virgule et activez le format scientifique
    weightStream << std::scientific << std::setprecision(2);
    weightStream << m_object->getWeight() << " kg";
    std::string weightString = weightStream.str();

    // Calcul de la position du texte (comme dans votre code)
    ImGui::PushFont(planetNamesFont);
    ImVec2 textSize = ImGui::CalcTextSize(weightString.c_str());
    ImVec2 textPos = ImVec2(centerPos.x - textSize.x / 2, position.y - textSize.y / 2);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    // Affichez le texte avec ImGui
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 150), weightString.c_str());
    ImGui::PopFont();

    weightIcon->Draw();


}

void SaveSimulationTool::drawDiameterLodo(ImVec2 centerPos,float longueurRec,float hauteurRec){
    //Same thing as mass but for diameter
    CelestialObject* m_object = selectedObject!=nullptr?selectedObject:hoverObject;
    ImVec2 position = ImVec2(centerPos.x-longueurRec/2+winHeight*0.0075*3,centerPos.y+hauteurRec/2*0.7);
    diameterIcon->UpdatePosition(position.x/winWidth,position.y/winHeight);

    std::ostringstream diameterStream;
    diameterStream << std::scientific << std::setprecision(2);
    diameterStream << m_object->real_radius/1000 << " km";
    std::string diameterString = diameterStream.str();

    ImGui::PushFont(planetNamesFont);
    ImVec2 textSize = ImGui::CalcTextSize(diameterString.c_str());
    ImVec2 textPos = ImVec2(centerPos.x - textSize.x / 2, position.y - textSize.y / 2);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float radius = winHeight*0.0075;
    ImVec4 color = m_renderContext->getTypeColor(m_object->typeName);
    drawList->AddCircleFilled(position, radius, IM_COL32(color.x, color.y, color.z, 100),100);
    if(m_object->type == 1){
        drawSunEffect(position, radius);}
    else{drawPlanetLightNOSHADOW(position, radius, color);}
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 150), diameterString.c_str());
    ImGui::PopFont();

    diameterIcon->Draw();
}


void SaveSimulationTool::drawGameStatesBoxForSave(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueur = winWidth*0.2;
    float hauteur = winHeight*0.225;
    float diffx = winWidth*0.5-centerPosSolarSystem.x;
    saveHistorySave->UpdatePosition(0.5+diffx/winWidth,0.3);
    ImVec2 centerPos = ImVec2(winWidth * 0.5+diffx, winHeight * 0.443);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5, centerPos.y - hauteur * 0.5);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueur * 0.5, centerPos.y + hauteur * 0.5);
    //drawList->AddRect(topLeft, bottomRight, IM_COL32(255,255,255,125),0,0,5.0f);

    //Draw text
    float posY = 0.347868*winHeight;
    float diffY = winHeight*0.036;
    float posXID = centerPos.x - 0.085*winWidth; 
    float posXName = centerPos.x - 0.06*winWidth;
    float posXDate = centerPos.x;
    float posXNumber = centerPos.x + 0.07*winWidth;

    //Create a table with GameStates. The first row a the titles : ID, Name, CreationDate, Number of planets
    ImGui::PushFont(saveTableFont);
    ImVec2 textPosID = ImVec2(posXID, posY);
    ImVec2 textPosName = ImVec2(posXName, posY);
    ImVec2 textPosDate = ImVec2(posXDate, posY);
    ImVec2 textPosNumber = ImVec2(posXNumber, posY);
    drawList->AddText(ImVec2(textPosID.x-ImGui::CalcTextSize("ID").x/2,textPosID.y), IM_COL32(255, 255, 255, 255), "ID");
    drawList->AddText(ImVec2(textPosName.x-ImGui::CalcTextSize("Name").x/2,textPosName.y), IM_COL32(255, 255, 255, 255), "Name");
    drawList->AddText(ImVec2(textPosDate.x-ImGui::CalcTextSize("Creation Date").x/2,textPosDate.y), IM_COL32(255, 255, 255, 255), "Creation Date");
    drawList->AddText(ImVec2(textPosNumber.x-ImGui::CalcTextSize("# of objects").x/2,textPosNumber.y), IM_COL32(255, 255, 255, 255), "# of objects");

    //Add line
    drawList->AddLine(ImVec2(centerPos.x-longueur*0.45, posY+diffY/1.5), ImVec2(centerPos.x+longueur*0.45, posY+diffY/1.5), IM_COL32(255, 255, 255, 100), winWidth*0.001);
    //Draw the gameStates

    //DrawRectAtMousePos
    ImVec2 mousePos = ImGui::GetMousePos();

    // Calculate the actual number of items to display, considering the offset and max limit
    if (gameStates.size() > maxGameStateShown) {
        idOffset = std::min(idOffset, static_cast<int>(gameStates.size()) - maxGameStateShown);
    } else {
        // If there are not enough game states to require scrolling, reset offset to 0
        idOffset = 0;
    }

    int itemsToDisplay = std::min(maxGameStateShown, static_cast<int>(gameStates.size()) - idOffset);
    
    //If empty, write "Empty in middle"
    if(gameStates.empty()){
        ImVec2 textPos = ImVec2(centerPos.x-ImGui::CalcTextSize("Empty").x/2,centerPos.y-winHeight*0.05-ImGui::CalcTextSize("Empty").y/2);
        ImGui::PushFont(SelectedplanetNameInSolarSystemFont);
        drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), "Empty");
        ImGui::PopFont();
    }
    bool shouldDrawSelected = false;
    GameState* gameStateToDraw = nullptr;
    for(int i = 0; i < itemsToDisplay; i++){
        int actualIndex = i + idOffset; // Adjust index based on the offset
        GameState* gameState = gameStates[actualIndex];
        std::string idString = "#"+std::to_string(actualIndex + 1); // Adjust ID display to match actual index + 1
        std::string name = gameState->getStateNameId();
        std::string date = gameState->getDateCreated();
        std::string number = std::to_string(gameState->getCelestialObjectsData().size());

        // Calculate positions based on actualIndex to maintain consistent spacing
        ImVec2 rowTopLeft = ImVec2(centerPos.x - longueur * 0.5, posY + diffY * (actualIndex + 1 - idOffset) - ImGui::CalcTextSize(idString.c_str()).y / 2);
        ImVec2 rowBottomRight = ImVec2(centerPos.x + longueur * 0.5, posY + diffY * (actualIndex + 2 - idOffset) - ImGui::CalcTextSize(idString.c_str()).y / 2);

        // Highlighting logic remains the same
        float alpha = 0;

        if (mousePos.x >= rowTopLeft.x && mousePos.x <= rowBottomRight.x && mousePos.y >= rowTopLeft.y && mousePos.y <= rowBottomRight.y) {
            if(currentGameStateSave != gameState)drawList->AddRectFilled(rowTopLeft, rowBottomRight, IM_COL32(255, 255, 255, 25)); // Highlight color
            alpha = 50;
            shouldDrawSelected = true;
            gameStateToDraw = gameState;
            //If mouseClicked, set currentGameStateSave to gameState
            if (ImGui::IsMouseClicked(0)) {
                currentGameStateSave = gameState;
                alpha = 100;
            }
        }
        if(currentGameStateSave == gameState){
            drawList->AddRectFilled(rowTopLeft, rowBottomRight, IM_COL32(225, 255, 225, 50)); // Highlight color
            alpha = 100;
        }

        // Adjust text positions based on actualIndex and idOffset
        ImVec2 textPosID = ImVec2(posXID, posY + diffY * (actualIndex + 1 - idOffset));
        ImVec2 textPosName = ImVec2(posXName, posY + diffY * (actualIndex + 1 - idOffset));
        ImVec2 textPosDate = ImVec2(posXDate, posY + diffY * (actualIndex + 1 - idOffset));
        ImVec2 textPosNumber = ImVec2(posXNumber, posY + diffY * (actualIndex + 1 - idOffset));

        // Drawing text
        drawList->AddText(ImVec2(textPosID.x-ImGui::CalcTextSize(idString.c_str()).x/2,textPosID.y), IM_COL32(255, 255, 255, 100+alpha), idString.c_str());
        drawList->AddText(ImVec2(textPosName.x-ImGui::CalcTextSize(name.c_str()).x/2,textPosName.y), IM_COL32(255, 255, 255, 150+alpha), name.c_str());
        drawList->AddText(ImVec2(textPosDate.x-ImGui::CalcTextSize(date.c_str()).x/2,textPosDate.y), IM_COL32(255, 255, 255, 100+alpha), date.c_str());
        drawList->AddText(ImVec2(textPosNumber.x-ImGui::CalcTextSize(number.c_str()).x/2,textPosNumber.y), IM_COL32(255, 255, 255, 150+alpha), number.c_str());
    }

    if(gameStates.size()>maxGameStateShown){
        float scrollbarWidth = winWidth*0.15*0.15*0.05; // La largeur de la barre de défilement
        ImVec2 centerPosScrollBar =  ImVec2(0.743229*winWidth, 0.460756*winHeight);
        // Calcul de la hauteur de la barre de défilement en fonction du nombre d'éléments visibles
        float totalItemsHeight = hauteur/2.5; // La hauteur totale disponible pour les éléments
        ImVec2 topLeftScrollBar = ImVec2(centerPosScrollBar.x - scrollbarWidth/2, centerPosScrollBar.y - totalItemsHeight * 0.75);
        ImVec2 bottomRightScrollBar = ImVec2(centerPosScrollBar.x + scrollbarWidth/2, centerPosScrollBar.y + totalItemsHeight * 0.75);
        float visibleItemsRatio = static_cast<float>(maxGameStateShown) / static_cast<float>(gameStates.size());
        float scrollbarHeight = totalItemsHeight * visibleItemsRatio;

        // Calcul de la position Y de la barre de défilement en fonction de idOffset
        float scrollbarPositionRatio = gameStates.empty() ? 0.0f : static_cast<float>(idOffset) / static_cast<float>(gameStates.size() - maxGameStateShown);
        float scrollbarYPosition = topLeft.y + (totalItemsHeight - scrollbarHeight) * scrollbarPositionRatio;
        float scrollbarWidthActive = 3*scrollbarWidth;
        // Dessiner le fond de la barre de défilement
        drawList->AddRectFilled(topLeftScrollBar, bottomRightScrollBar, IM_COL32(255, 255, 255, 100));

                // Dessiner la barre de défilement
        float scrollbarActiveYPosition = topLeftScrollBar.y + (bottomRightScrollBar.y - topLeftScrollBar.y - scrollbarHeight) * scrollbarPositionRatio;
        ImVec2 topLeftScrollBarActive = ImVec2(centerPosScrollBar.x - scrollbarWidthActive / 2, scrollbarActiveYPosition);
        ImVec2 bottomRightScrollBarActive = ImVec2(centerPosScrollBar.x + scrollbarWidthActive / 2, scrollbarActiveYPosition + scrollbarHeight);

        // Utilisez une couleur distincte pour la barre de défilement active pour la différencier du fond
        float detectionWidth = scrollbarWidth * 10; // Increase the width for hover detection
        ImVec2 detectionTopLeft = ImVec2(centerPosScrollBar.x - detectionWidth / 2, topLeftScrollBar.y);
        ImVec2 detectionBottomRight = ImVec2(centerPosScrollBar.x + detectionWidth / 2, bottomRightScrollBar.y);

        // Check if mouse is hovering over the widened detection area
        bool isHoveringScrollBar = mousePos.x >= detectionTopLeft.x && mousePos.x <= detectionBottomRight.x && mousePos.y >= detectionTopLeft.y && mousePos.y <= detectionBottomRight.y;
        if (isHoveringScrollBar) {
            // Increase alpha when hovering
            drawList->AddRectFilled(topLeftScrollBarActive, bottomRightScrollBarActive, IM_COL32(255, 255, 255, 255), winWidth*0.002); // Hover alpha = 255

            if (ImGui::IsMouseDown(0)) {
                // Adjust the offset based on the mouse position relative to the scrollbar's top and bottom
                float mouseRelativeToScrollbar = (mousePos.y - detectionTopLeft.y) / (detectionBottomRight.y - detectionTopLeft.y);
                int newOffset = static_cast<int>(mouseRelativeToScrollbar * (gameStates.size() - maxGameStateShown + 1)); // +1 to ensure reachability of last item
                newOffset = std::max(0, std::min(newOffset, static_cast<int>(gameStates.size()) - maxGameStateShown));
                
                if(newOffset != idOffset) {
                    // Calculate the direction of scroll based on newOffset vs. current idOffset
                    int scrollDirection = (newOffset > idOffset) ? 1 : -1;
                    while(idOffset != newOffset) {
                        changeScrollOffset(scrollDirection);
                    }
                }
            }
        } else {
            // Default scrollbar alpha = 200
            drawList->AddRectFilled(topLeftScrollBarActive, bottomRightScrollBarActive, IM_COL32(255, 255, 255, 200), winWidth*0.002);
        }
    }

    //Check if mouse is hovering main rectangle (use longueur, hauteur et centerPos)
    if(mousePos.x > centerPos.x-longueur*0.5 && mousePos.x < centerPos.x+longueur*0.5 && mousePos.y > centerPos.y-hauteur*0.5 && mousePos.y < centerPos.y+hauteur*0.5){
        if(ImGui::GetIO().MouseWheel > 0){
            changeScrollOffset(-1);
        }else if(ImGui::GetIO().MouseWheel < 0){
            changeScrollOffset(1);
        }
    }
    ImGui::PopFont();
    if(shouldDrawSelected)drawSelectedState(gameStateToDraw);
}

void SaveSimulationTool::changeScrollOffset(int offset){
    if (offset==1){
        idOffset += 1;
        if(idOffset > gameStates.size() - maxGameStateShown){
            idOffset = gameStates.size() - maxGameStateShown;
        }
    }else if(offset==-1){
        idOffset -= 1;
        if(idOffset < 0){
            idOffset = 0;
        }
    }
}





































void SaveSimulationTool::setupDimension(){
    centerPosSolarSystem = ImVec2(winWidth * 0.365, winHeight * 0.5);
    radiusSolarSystem = winHeight * 0.175;
    minDistanceMouseDetection = std::numeric_limits<float>::max();
    scaledDistance = radiusSolarSystem / m_renderContext->systemeSolaire->maxSize;
    logBase = 1.05; // Base logarithmique pour la compression des distances)
    adjustLogScale();
    generate_asteroids();
}

void SaveSimulationTool::adjustLogScale() {
    //FONCTION : FIND AFTERLOG SCALE FOR SOLAR SYSTEM TO OCCUPIE THE WHOLE radiusSolarSystem
    // Trouver la distance maximale entre le centre et l'objet le plus éloigné
    double maxDistance = 0.0;
    for (const auto& objet : m_renderContext->systemeSolaire->objects) {
        ImVec2 objetPos = ImVec2(objet->getPositionSimulation().x, objet->getPositionSimulation().z);
        double distance = std::sqrt(std::pow(objetPos.x, 2) + std::pow(objetPos.y, 2));
        if (distance > maxDistance) {
            maxDistance = distance;
        }
    }
    maxDistance *= scaledDistance;
    if (maxDistance > 0) { // Éviter la division par zéro
        double compressedMaxDistance = std::log(std::max(maxDistance, 1.0) * logBase);
        scaleAfterLog = radiusSolarSystem / compressedMaxDistance*0.95; // 0.9 pour laisser un peu d'espace
    }

}

void SaveSimulationTool::loadingIndicator(){
    //Draw a loading indicator in the middle of the screen
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 centerPos = ImVec2(winWidth * 0.5, winHeight * 0.5);
    float longueur = winWidth*0.2;
    float hauteur = winHeight*0.15;
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5, centerPos.y - hauteur * 0.5);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueur * 0.5, centerPos.y + hauteur * 0.5);
    float cornerRadius = winWidth*0.0075;

    std::string loadingString = "Loading ";
    loadingString+=currentLoadState->getStateNameId();
    loadingString+="...";

    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winWidth,winHeight), 
                            IM_COL32(0, 0, 0, 225), // Color
                            2.0f); 
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0, 0, 0, 225), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft,
                            bottomRight, 
                            IM_COL32(20, 25, 30, 100), // Color
                            cornerRadius);
    loadingLabbel->UpdateText(loadingString);
    loadingLabbel->Draw();
    notificationIcon->Draw();
}

void SaveSimulationTool::adjustLogScaleLOAD(std::vector<CelestialObjectData> objects) {
    //FONCTION : FIND AFTERLOG SCALE FOR SOLAR SYSTEM TO OCCUPIE THE WHOLE radiusSolarSystem
    // Trouver la distance maximale entre le centre et l'objet le plus éloigné
    double maxDistance = 0.0;
    double scale = m_renderContext->systemeSolaire->scale;
    for (const auto& objet : objects) {
        ImVec2 objetPos = ImVec2(objet.position.x*scale, objet.position.z*scale);
        double distance = std::sqrt(std::pow(objetPos.x, 2) + std::pow(objetPos.y, 2));
        if (distance > maxDistance) {
            maxDistance = distance;
        }
    }
    maxDistance *= scaledDistance;
    if (maxDistance > 0) { // Éviter la division par zéro
        double compressedMaxDistance = std::log(std::max(maxDistance, 1.0) * logBase);
        scaleAfterLog = radiusSolarSystem / compressedMaxDistance*0.95; // 0.9 pour laisser un peu d'espace
    }

}

ImVec2 SaveSimulationTool::distanceConverterSimToLog(ImVec2 positionInitiale){
        // Calculer la position initiale
        ImVec2 initialPos = ImVec2(positionInitiale.x * scaledDistance, 
                                         positionInitiale.y * scaledDistance);

        // Appliquer la compression logarithmique et le facteur d'échelle
        double distance = std::sqrt(std::pow(initialPos.x, 2) + std::pow(initialPos.y, 2));
        double compressedDistance = std::log(std::max(distance, 1.0) * logBase) * scaleAfterLog;
        ImVec2 compressedPlanetPos = ImVec2(initialPos.x / distance * compressedDistance, 
                                            initialPos.y / distance * compressedDistance);

        // Calculer la position finale avec le décalage
        //Sun always in center, offect with sun to account for this 
        
        ImVec2 planetPos = ImVec2(compressedPlanetPos.x+centerPosSolarSystem.x, compressedPlanetPos.y+centerPosSolarSystem.y);
        return planetPos;
}



//Planets and others accessoiries

void SaveSimulationTool::drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, ImVec4 colorCenterDot){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float numBlurCircles = 10;
    float blurIncrease = radius*0.08; // How much larger each successive blur circle is
    float initialAlpha = 60; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 50);}
}

void SaveSimulationTool::drawSunEffect(ImVec2 planetPos, float radius){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec4 colorCenterDot = ImVec4(255,215,80,255);
    float numBlurCircles = 20;
    float blurIncrease = radius*0.1; // How much larger each successive blur circle is
    float initialAlpha = 50; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 50);}
}

void SaveSimulationTool::drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius){
        ImVec2 directionToSun = ImVec2(planetPos.x - sunPos.x, planetPos.y - sunPos.y);
        float shadowAngle = atan2(directionToSun.y, directionToSun.x) + IM_PI; // Ajouter PI pour que l'ombre soit opposée au Soleil
        // Dessiner l'ombre comme un demi-cercle
        draw_half_circle_shadow(planetPos, radius, IM_COL32(20, 25, 30, 100), shadowAngle, 100);
}



void SaveSimulationTool::draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments) {
    // Calcule les angles pour le demi-cercle d'ombre
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    radius*=1.6;
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


void SaveSimulationTool::generate_asteroids() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    asteroid1Positions.clear();
    asteroid2Positions.clear();
    float scale = radiusSolarSystem*2 / 30.0f; // Convertir les unités astronomiques en pixels

    // Ceinture Principale d'Astéroïdes
    float rayonInterieur1 = 17.0f * scale/2;
    float rayonExterieur1 = 19.0f * scale/2;
    int numAsteroides1 = 1000;
    generate_asteroid_belt(gen, dis, rayonInterieur1, rayonExterieur1, numAsteroides1, asteroid1Positions);
    // Ceinture de Kuiper
    float rayonInterieur2 = 30.0f * scale/2; // Commence à la position de Neptune
    float rayonExterieur2 = 33.0f * scale/2; // S'étend au-delà de Neptune
    int numAsteroides2 = 1000;
    generate_asteroid_belt(gen, dis, rayonInterieur2, rayonExterieur2, numAsteroides2, asteroid2Positions);
}
void SaveSimulationTool::generate_asteroid_belt(std::mt19937& gen, std::uniform_real_distribution<>& dis, float rayonInterieur, float rayonExterieur, int numAsteroides, std::vector<ImVec2>& asteroidPositions) {
    for (int i = 0; i < numAsteroides; ++i) {
        float angle = dis(gen) * 360.0f;
        float rayon = rayonInterieur + dis(gen) * (rayonExterieur - rayonInterieur);
        float x = rayon * cos(angle * M_PI / 180.0f);
        float y = rayon * sin(angle * M_PI / 180.0f);
        asteroidPositions.push_back(ImVec2(x, y));
    }
}

void SaveSimulationTool::draw_asteroid_belt(const std::vector<ImVec2>& asteroidPositions, float tailleAsteroide, ImU32 color) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    for (const ImVec2& pos : asteroidPositions) {
        // Dessiner l'astéroïde à sa nouvelle position
        drawList->AddCircleFilled(ImVec2(pos.x + centerPosSolarSystem.x, centerPosSolarSystem.y + pos.y), tailleAsteroide, color);
    }
}





//Checkinputs
void SaveSimulationTool::checkInputs(){

    //If saveState
    if(currentState == State::SaveSimulation){
    
    //If up arrow is pressed, increase the offset
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
        changeScrollOffset(-1);
    }
    //If down arrow is pressed, decrease the offset
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
        changeScrollOffset(1);
    }}

    /*
    else if (currentState == State::LoadSimulation){
        int direction = 0;
        bool keyPressed = false;
        if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))){ direction = -1; keyPressed = true;}
        if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))){ direction = 1; keyPressed = true;}
        if(keyPressed)nextSelectedState(direction);
    }*/



}



void SaveSimulationTool::drawSelectedState(GameState *gameState){
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 offSet = ImVec2(0.12*winWidth,-0.05*winHeight);
    ImVec2 centerPos = ImVec2(mousePos.x+offSet.x,mousePos.y+offSet.y);
    float dimension = winHeight*0.3;
    float systemDiameter = dimension*0.8;
    ImVec2 topLeft = ImVec2(centerPos.x - dimension * 0.5, centerPos.y - dimension * 0.5);
    ImVec2 bottomRight = ImVec2(centerPos.x + dimension * 0.5, centerPos.y + dimension * 0.5);
    float planetRadius = systemDiameter * 0.02;
    //DrawRect square background   
    float cornerRadius = 10.0f;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0, 0, 0, 250), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30, 100), // Color
                            cornerRadius);

    std::vector<CelestialObjectData> objectList = gameState->getCelestialObjectsData();

    // Filtrer pour exclure les lunes dès le début
    std::vector<CelestialObjectData> filteredObjectList;
    std::copy_if(objectList.begin(), objectList.end(), std::back_inserter(filteredObjectList), [](const CelestialObjectData& obj) {
        return obj.typeName != "Moon";
    });

    // Trouver le Soleil
    CelestialObjectData sun = gameState->getSunData(nullptr,objectList);
    Vec3 sunPos = sun.position;

    // Trier les objets filtrés par distance au Soleil
    std::sort(filteredObjectList.begin(), filteredObjectList.end(), [&sunPos](const CelestialObjectData& a, const CelestialObjectData& b) {
        float distA = std::sqrt(std::pow(a.position.x - sunPos.x, 2) + std::pow(a.position.z - sunPos.z, 2));
        float distB = std::sqrt(std::pow(b.position.x - sunPos.x, 2) + std::pow(b.position.z - sunPos.z, 2));
        return distA < distB;
    });

    float maxDistance = systemDiameter / 2;
    // Calculer l'incrément basé sur le nombre d'objets après filtrage
    float increment = maxDistance / (filteredObjectList.size() - 1); // Exclure le Soleil du calcul
    float currentDistance = 0;

    for (auto& planet : filteredObjectList) {
        if(planet.typeName == "Moon") continue;
        ImVec2 planetPos;
        float angle = atan2(planet.position.z - sunPos.z, planet.position.x - sunPos.x);

        if (planet.name == sun.name) {
            planetPos = centerPos; // Le Soleil reste au centre
        } else {
            // Positionner les planètes linéairement en fonction de leur distance
            currentDistance += increment;
            planetPos = ImVec2(centerPos.x + cos(angle)*currentDistance, centerPos.y+ sin(angle)*currentDistance); // Exemple simplifié sur l'axe X
        }

        ImVec4 planetColor = m_renderContext->getTypeColor(planet.typeName);
        drawList->AddCircleFilled(planetPos, planetRadius, IM_COL32(planetColor.x, planetColor.y, planetColor.z, 255));
        //Draw effets
        if(planet.typeName == "Sun"){
            drawSunEffect(planetPos, planetRadius);
        }else{
            drawPlanetLightNOSHADOW(planetPos, planetRadius, ImVec4(planetColor.x, planetColor.y, planetColor.z, 255));
            //Sun effect
            ImVec2 sunPos = ImVec2(centerPos.x, centerPos.y);
            drawPlanetLight(planetPos, sunPos, planetRadius);
        }

        // Optionnel: Dessiner les orbites comme lignes ou cercles autour du soleil
        drawList->AddCircle(centerPos, currentDistance, IM_COL32(planetColor.x, planetColor.y, planetColor.z, 30), 100, planetRadius * 0.5);

        //add planete name
        ImFont* font = wrongInputNameFont;
        ImGui::PushFont(font);
        std::string planetName = planet.name;
        ImVec2 textPos = ImVec2(planetPos.x - ImGui::CalcTextSize(planetName.c_str()).x / 2, 
                                planetPos.y - planetRadius - winHeight*0.01-ImGui::CalcTextSize(planetName.c_str()).y / 2);

        drawList->AddText(textPos, IM_COL32(255, 255, 255, 150), planetName.c_str());
        ImGui::PopFont();
    }
    //Add simulation Date
    ImGui::PushFont(planetNamesFont);
    std::string date = gameState->getSimulationTimeStampString();
    ImVec2 textPos = ImVec2(centerPos.x-ImGui::CalcTextSize(date.c_str()).x/2,centerPos.y+dimension*0.5-winHeight*0.025);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 200), date.c_str());
    ImGui::PopFont();

    ImGui::PushFont(planetNamesFont);
    //Add simulation Name on top
    std::string name = gameState->getStateNameId();
    ImVec2 textPosName = ImVec2(centerPos.x-ImGui::CalcTextSize(name.c_str()).x/2,centerPos.y-dimension*0.5+winHeight*0.025-ImGui::CalcTextSize(name.c_str()).y);
    drawList->AddText(textPosName, IM_COL32(255, 255, 255, 255), name.c_str());
    ImGui::PopFont();


}




ImVec2 SaveSimulationTool::distanceConverterSimToLogSmallSystem(ImVec2 position, float solarSystemSize){
    // Compute scale for the initial position
    float scale = solarSystemSize / m_renderContext->systemeSolaire->maxSize * m_renderContext->systemeSolaire->scale;
    ImVec2 scaledPosition = ImVec2(position.x * scale, position.y * scale);

    // Apply logarithmic compression and scaling
    double distance = std::hypot(scaledPosition.x, scaledPosition.y);
    if (distance < 1.0) distance = 1.0; // Prevent log(0) and ensure minimum distance
    double compressedDistance = log(distance * logBase) * 0.1;
    ImVec2 compressedPos = ImVec2(scaledPosition.x* (compressedDistance / distance), scaledPosition.y * (compressedDistance / distance));

    // Adjust position relative to solar system center
    ImVec2 finalPosition = ImVec2(compressedPos.x + centerPosSolarSystem.x, compressedPos.y + centerPosSolarSystem.y);
    return finalPosition;
}

