#include "engine/RenderTools/saveSimulation.h"
#include "path_util.h"
#include "engine/RenderTools/soundTool.h"
#include "save/saveTool.h"
#include <sstream> // Pour std::ostringstream
#include <iomanip> // Pour std::setprecision
#include "save/gameState.h"

void SaveSimulationTool::drawLoadSimulation(){
    if(shouldLoad){load();}
    if(isLoading){
        loadingIndicator();
        shouldLoad = true;
        return;}
    drawUILoadSimulation();
    drawPlanetSystemLoad();
    drawGameStatesBoxForLoad();
    checkInputs();
    if(confirmRemoveState){
        drawConfirmRemove();
    }

}

void SaveSimulationTool::drawUILoadSimulation(){
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

    loadTitleLabbel->Draw();
    saveIcon->Draw();
    quitSaveBtn->Draw();
    switchStateBtn->Draw();
    loadBtn->Draw();
    //Cout offset/number of states
    //If mouse clicked
    

 
}


void SaveSimulationTool::drawPlanetSystemLoad(){
    if(currentLoadState == nullptr)return;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    adjustLogScaleLOAD(currentLoadState->getCelestialObjectsData());

    std::vector<CelestialObjectData> celestialObjects = currentLoadState->getCelestialObjectsData();
    if(celestialObjects.empty()){
        //Add empty in middle
        ImGui::PushFont(planetNamesFont);
        ImVec2 centerPos = centerPosSolarSystem;
        std::string emptyText = "No objects in this state";
        ImVec2 textSize = ImGui::CalcTextSize(emptyText.c_str());
        ImVec2 textPos = ImVec2(centerPos.x - textSize.x / 2, centerPos.y - textSize.y / 2);
        drawList->AddText(textPos, IM_COL32(255, 255, 255, 150), emptyText.c_str());
        ImGui::PopFont();
        return;
    }
    float planetRadius = winHeight*0.0065;

    //Draw system solar size
    CelestialObjectData sun = currentLoadState->getSunData(nullptr,celestialObjects);
    float scale = m_renderContext->systemeSolaire->scale;
    ImVec2 sunPos = ImVec2(sun.position.x, sun.position.y);
    
    ImVec2 sunPosScaled = ImVec2(sunPos.x*scale, sunPos.y*scale);
    bool anyPlanetClicked = false;
    hoverObjectData = nullptr;
    ImVec2 hoverObjectPosition;
    float closestDistance = std::numeric_limits<float>::max(); // Pour suivre la distance la plus proche
    //Loop through celestialObjects and draw them
    draw_asteroid_belt(asteroid1Positions,winHeight*0.0010, IM_COL32(180, 150, 150, 75));
    draw_asteroid_belt(asteroid2Positions,winHeight*0.0010, IM_COL32(150, 200, 255, 75));
    for (auto& celestialObject : celestialObjects) {
        if(celestialObject.typeName == "Moon") continue; // Do not draw moons 
        ImVec2 relativePosition = ImVec2(celestialObject.position.x*scale - sunPosScaled.x, celestialObject.position.z*scale - sunPosScaled.y);
        ImVec2 planetPos = distanceConverterSimToLog(relativePosition);
        ImVec4 planetColor = m_renderContext->getTypeColor(celestialObject.typeName);
        if(checkMouseSelectionLoad(planetPos, &celestialObject, planetRadius, closestDistance)){
            anyPlanetClicked = true;
        }
        if(celestialObject.name == sun.name){
            planetPos = centerPosSolarSystem;
        }
        //DrawOrbit
        float distanceFromCenter = std::sqrt(std::pow(planetPos.x - centerPosSolarSystem.x, 2) + std::pow(planetPos.y - centerPosSolarSystem.y, 2));
        drawList->AddCircle(centerPosSolarSystem,distanceFromCenter, IM_COL32(planetColor.x, planetColor.y, planetColor.z,30),100,planetRadius*0.5);
        drawSpeedArrow(celestialObject, planetPos, sun);
        //DrawObject
        drawList->AddCircleFilled(planetPos, planetRadius, IM_COL32(planetColor.x, planetColor.y, planetColor.z, 225),100);
        if(celestialObject.typeName == "Sun"){
            drawSunEffect(planetPos, planetRadius);
        }else{
            drawPlanetLightNOSHADOW(planetPos, planetRadius, ImVec4(planetColor.x, planetColor.y, planetColor.z, 255));
            //Sun effect
            if(sun.typeName == "Sun" && celestialObject.typeName != "BlackHole"){
                drawPlanetLight(planetPos, centerPosSolarSystem, planetRadius);
            }
        }

        float circleAlpha = 100;
        if(&celestialObject == selectedObjectData){circleAlpha = 255;}
        else if(&celestialObject == hoverObjectData){circleAlpha = 150;}
        if(&celestialObject == selectedObjectData || &celestialObject == hoverObjectData){
            drawList->AddCircle(planetPos, planetRadius*1.5, IM_COL32(255,255,255,circleAlpha),100,planetRadius*0.2);
        }
        //Add names
        ImFont* font = &celestialObject == selectedObjectData ? SelectedplanetNameInSolarSystemFont : planetNamesFont;
        ImGui::PushFont(font);
        std::string planetName = celestialObject.name;
        float padding = celestialObject.typeName=="Sun"?winHeight*0.02:winHeight*0.015;
        ImVec2 textPos = ImVec2(planetPos.x - ImGui::CalcTextSize(planetName.c_str()).x / 2, 
                                planetPos.y - planetRadius - padding-ImGui::CalcTextSize(planetName.c_str()).y / 2);
        float alpha = 100;
        if(&celestialObject == selectedObjectData){alpha = 225;}
        else if(&celestialObject == hoverObjectData){alpha = 175;}
        drawList->AddText(textPos, IM_COL32(255, 255, 255, alpha), planetName.c_str());
        ImGui::PopFont();

        if(hoverObjectData!=nullptr && hoverObjectData->name == celestialObject.name){
            hoverObjectPosition = planetPos;
        }
    }
    //Add date and time
    ImGui::PushFont(planetNamesFont);
    std::string date = currentLoadState->getSimulationTimeStampString();
    ImVec2 textPos = ImVec2(centerPosSolarSystem.x-ImGui::CalcTextSize(date.c_str()).x/2,centerPosSolarSystem.y+winHeight*0.25);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 220), date.c_str());
    ImGui::PopFont();

    //Add GameStateName on top
    ImGui::PushFont(planetNamesFont);
    std::string gameStateName = currentLoadState->getStateNameId();
    ImVec2 textPosGameState = ImVec2(centerPosSolarSystem.x-ImGui::CalcTextSize(gameStateName.c_str()).x/2,centerPosSolarSystem.y+winHeight*0.225-ImGui::CalcTextSize(gameStateName.c_str()).y/2);
    drawList->AddText(textPosGameState, IM_COL32(255, 255, 255, 220), gameStateName.c_str());
    ImGui::PopFont();

    if(hoverObjectData != nullptr){
        drawInfosLoad(hoverObjectData, &sun, hoverObjectPosition);
    }

    //Draw infos

    

}


bool SaveSimulationTool::checkMouseSelectionLoad(ImVec2 planetPos, CelestialObjectData* planete, float radius, float &closestDistance){
    ImVec2 mousePos = ImGui::GetMousePos();
    bool mouseClicked = ImGui::IsMouseClicked(0);
    float distanceToMouse = std::sqrt(std::pow(mousePos.x - planetPos.x, 2) + std::pow(mousePos.y - planetPos.y, 2));
    if (distanceToMouse < radius * 2) { // Vérifie si la souris est suffisamment proche
        if (distanceToMouse < closestDistance) { // Si cet objet est le plus proche jusqu'à présent
            closestDistance = distanceToMouse; // Mise à jour de la distance la plus proche
            hoverObjectData = planete; // Mise à jour de l'objet survolé
            if (mouseClicked) {
                //selectedObjectData = planete; // Sélection de l'objet si cliqué
                return true; // Indique qu'une sélection a été faite
            }
        }
    }
    return false; // Aucune sélection faite
}


void SaveSimulationTool::drawConfirmRemove(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueur = winWidth * 0.275; // Exemple de taille
    float hauteur = winHeight * 0.175; // Exemple de taille
    float cornerRadius = winWidth*0.01;
    ImVec2 topLeft = ImVec2(winWidth * 0.5 - longueur * 0.5, winHeight * 0.5 - hauteur * 0.5);
    ImVec2 bottomRight = ImVec2(winWidth * 0.5 + longueur * 0.5, winHeight * 0.5 + hauteur * 0.5);

    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winHeight*2,winHeight*2), 
                            IM_COL32(0, 0, 0, 200), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0, 0, 0, 250), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30, 200), // Color
                            cornerRadius);

    confirmRemoveButton->Draw();
    removeLabbel->Draw();
    cancelRemoveButton->Draw();
}

void SaveSimulationTool::confirmRemove(){
    std::string phrase = "Do you wish to remove " + currentLoadState->getStateNameId() + " ?";
    removeLabbel->UpdateText(phrase);
    confirmRemoveState = true;
}

void SaveSimulationTool::cancelRemove(){
    confirmRemoveState = false;
}

void SaveSimulationTool::removeState(){
    confirmRemoveState = false;
    if(currentLoadState == nullptr)return;

    SaveTool saveTool = SaveTool();
    saveTool.removeGameState(currentLoadState->getGameStateId());

    gameStates.erase(std::remove(gameStates.begin(), gameStates.end(), currentLoadState), gameStates.end());
    currentLoadState = nullptr;
}










void SaveSimulationTool::drawInfosLoad(CelestialObjectData* planete, CelestialObjectData* sun, ImVec2 planetPos){
    CelestialObjectData* m_object = planete;
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
    ImVec4 color = m_renderContext->getTypeColor(planete->typeName);
    drawList->AddCircleFilled(planetPosTopLeft, radius, IM_COL32(color.x, color.y, color.z, 225),100);
    if(m_object->typeName == "Sun"){
        drawSunEffect(planetPosTopLeft, radius);}
    else{drawPlanetLightNOSHADOW(planetPosTopLeft, radius, color);}
    


    drawDistanceFromSunLogoLoad(planete, sun, centerPos,longueurRect,hauteurRect,color);
    drawWeightLogoLoad(planete,centerPos,longueurRect,hauteurRect);
    drawDiameterLodoLoad(planete,centerPos,longueurRect,hauteurRect);

    //Add current time of the simulation at 

}
void SaveSimulationTool::drawDistanceFromSunLogoLoad(CelestialObjectData* planete,CelestialObjectData* sun, ImVec2 centerPos,float longueurRec,float hauteurRec,ImVec4 color){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 position = ImVec2(centerPos.x-longueurRec/2+winHeight*0.0075*3,centerPos.y-hauteurRec/2*0.3);

    CelestialObjectData* m_object = planete;
    float radius = winHeight*0.0035;
    float diffHauteur = winHeight*0.02;
    ImVec2 planetPos = ImVec2(position.x, position.y+diffHauteur);
    ImVec2 sunPos = ImVec2(position.x, position.y-diffHauteur);
    ImVec4 sunColor = m_renderContext->getTypeColor(sun->typeName);



    drawList->AddCircleFilled(planetPos, radius, IM_COL32(color.x, color.y, color.z, 225),100);
    if(m_object->typeName == "Sun"){
        drawSunEffect(planetPos, radius);}
    else{drawPlanetLightNOSHADOW(planetPos, radius, color);}
    drawList->AddCircleFilled(sunPos, radius, IM_COL32(color.x, color.y, color.z, 225),100);
    if(sun->typeName == "Sun"){
        drawSunEffect(sunPos, radius);}
    else{drawPlanetLightNOSHADOW(sunPos, radius, sunColor);}


    verticalArrow->UpdatePosition(position.x/winWidth,position.y/winHeight);
    verticalArrow->Draw();

    //Compute distance in AU
    ImGui::PushFont(planetNamesFont);
    Vec3 planetePos = m_object->position;
    Vec3 sunPosition = sun->position;
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
void SaveSimulationTool::drawWeightLogoLoad(CelestialObjectData* planete, ImVec2 centerPos,float longueurRec,float hauteurRec){
    CelestialObjectData* m_object = planete;
    ImVec2 position = ImVec2(centerPos.x-longueurRec/2+winHeight*0.0075*3,centerPos.y+hauteurRec/2*0.25);
    weightIcon->UpdatePosition(position.x/winWidth,position.y/winHeight);

    //Add weight text in scientific notation with 2 decimal
    std::ostringstream weightStream;
    // Définissez la précision à 2 chiffres après la virgule et activez le format scientifique
    weightStream << std::scientific << std::setprecision(2);
    weightStream << m_object->mass << " kg";
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

void SaveSimulationTool::drawDiameterLodoLoad(CelestialObjectData* planete, ImVec2 centerPos,float longueurRec,float hauteurRec){
    //Same thing as mass but for diameter
    CelestialObjectData* m_object = planete;
    ImVec2 position = ImVec2(centerPos.x-longueurRec/2+winHeight*0.0075*3,centerPos.y+hauteurRec/2*0.7);
    diameterIcon->UpdatePosition(position.x/winWidth,position.y/winHeight);

    std::ostringstream diameterStream;
    diameterStream << std::scientific << std::setprecision(2);
    diameterStream << m_object->radius/1000 << " km";
    std::string diameterString = diameterStream.str();

    ImGui::PushFont(planetNamesFont);
    ImVec2 textSize = ImGui::CalcTextSize(diameterString.c_str());
    ImVec2 textPos = ImVec2(centerPos.x - textSize.x / 2, position.y - textSize.y / 2);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float radius = winHeight*0.0075;
    ImVec4 color = m_renderContext->getTypeColor(m_object->typeName);
    drawList->AddCircleFilled(position, radius, IM_COL32(color.x, color.y, color.z, 100),100);
    if(m_object->typeName == "Sun"){
        drawSunEffect(position, radius);}
    else{drawPlanetLightNOSHADOW(position, radius, color);}
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 150), diameterString.c_str());
    ImGui::PopFont();

    diameterIcon->Draw();
}

void SaveSimulationTool::drawSpeedArrow(CelestialObjectData planete, ImVec2 planetPos, CelestialObjectData sun){
    //Draw an arrow from the planet to its speed direction
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Vec3 speed = planete.velocity;
    ImVec2 speedDirection = ImVec2(speed.x, speed.z);
    //Normalize the speed direction
    float norm = sqrt(speedDirection.x * speedDirection.x + speedDirection.y * speedDirection.y);
    speedDirection = ImVec2(speedDirection.x / norm, speedDirection.y / norm);
    float arrowLength = winHeight*0.015;
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
    if(planete.name != sun.name)
    {   drawList->AddLine(arrowStart, arrowEnd, IM_COL32(255, 255, 255, 200), winWidth*0.001);
        drawList->AddTriangleFilled(arrowPoint1, arrowPoint2, arrowPoint3, IM_COL32(255, 255, 255, 200));}

}



void SaveSimulationTool::nextSelectedState(int direction){
    return;
    if(gameStates.empty())return;
    if(currentLoadState == nullptr){
        currentLoadState = gameStates[0];
        return;
    }

    int currentIndex = -1;
    for(int i = 0; i < gameStates.size(); i++){
        if(gameStates[i]->getStateNameId() == currentLoadState->getStateNameId()){
            currentIndex = i;
            break;
        }
    }
    if(currentIndex == -1){
        currentLoadState = gameStates[0];
        return;
    }
    int newIndex = currentIndex + direction;
    if(newIndex < 0){
        newIndex = 0;
    }
    if(newIndex >= gameStates.size()){
        newIndex = gameStates.size()-1;
    }
    currentLoadState = gameStates[newIndex];

    //Also change offset to be able to see the selected state
    if(newIndex < idOffset){
        idOffset = newIndex;
    }
    if(newIndex >= idOffset + maxGameStateShown){
        idOffset = newIndex - maxGameStateShown + 1;
    }
}

















































































void SaveSimulationTool::drawGameStatesBoxForLoad(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueur = winWidth*0.2;
    float hauteur = winHeight*0.225;
    float diffx = winWidth*0.5-centerPosSolarSystem.x;
    saveHistorySave->UpdatePosition(0.5+diffx/winWidth,0.3);
    ImVec2 centerPos = ImVec2(winWidth * 0.5+diffx, winHeight * 0.443);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5, centerPos.y - hauteur * 0.5);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueur * 0.5, centerPos.y + hauteur * 0.5);
    //drawList->AddRect(topLeft, bottomRight, IM_COL32(255,255,255,125),0,0,5.0f);

    if(gameStates.empty()){
        //Add empty in middle
        ImGui::PushFont(planetNamesFont);
        std::string emptyText = "No saved states";
        ImVec2 textSize = ImGui::CalcTextSize(emptyText.c_str());
        ImVec2 textPos = ImVec2(centerPos.x - textSize.x / 2, centerPos.y - textSize.y / 2);
        drawList->AddText(textPos, IM_COL32(255, 255, 255, 150), emptyText.c_str());
        ImGui::PopFont();
        return;
    }

    //Draw text
    float posY = 0.331395*winHeight;
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

        if (mousePos.x >= rowTopLeft.x && mousePos.x <= rowBottomRight.x && mousePos.y >= rowTopLeft.y && mousePos.y <= rowBottomRight.y && !confirmRemoveState) {
            if(currentLoadState != gameState)drawList->AddRectFilled(rowTopLeft, rowBottomRight, IM_COL32(255, 255, 255, 25)); // Highlight color
            alpha = 50;
            shouldDrawSelected = true;
            gameStateToDraw = gameState;
            //If mouseClicked, set currentLoadState to gameState
            if (ImGui::IsMouseClicked(0)) {
                currentLoadState = gameState;
                alpha = 100;
            }
            if(ImGui::IsMouseDoubleClicked(0)){
                currentLoadState = gameState;
                LoadSimulationsButton();
            }
            float textSizeY = ImGui::CalcTextSize(idString.c_str()).y/2;
            removeStateButton->UpdatePosition(0.723437 ,(posY +textSizeY +diffY * (actualIndex + 1 - idOffset))/winHeight);
            removeStateButton->Draw();
            
        }
        if(currentLoadState == gameState){
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
        ImVec2 centerPosScrollBar =  ImVec2(0.743229*winWidth, 0.55*winHeight);
        // Calcul de la hauteur de la barre de défilement en fonction du nombre d'éléments visibles
        float totalItemsHeight = hauteur/2.5; // La hauteur totale disponible pour les éléments
        ImVec2 topLeftScrollBar = ImVec2(centerPosScrollBar.x - scrollbarWidth/2, centerPosScrollBar.y - totalItemsHeight * 1.5);
        ImVec2 bottomRightScrollBar = ImVec2(centerPosScrollBar.x + scrollbarWidth/2, centerPosScrollBar.y + totalItemsHeight * 1.5);
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
    topLeft = ImVec2(winWidth*0.543229, winHeight*0.343992);
    bottomRight = ImVec2(winWidth*0.74375, winHeight*0.732558);
    if(mousePos.x > topLeft.x && mousePos.x < bottomRight.x && mousePos.y > topLeft.y && mousePos.y < bottomRight.y){
        if(ImGui::GetIO().MouseWheel > 0){
            changeScrollOffset(-1);
        }else if(ImGui::GetIO().MouseWheel < 0){
            changeScrollOffset(1);
        }
    }
    ImGui::PopFont();
}




















