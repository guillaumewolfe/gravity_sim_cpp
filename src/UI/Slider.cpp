#include "UI/Slider.h"

Slider::Slider(float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 hoverColor, float* value, ImVec2 range, float cornerRadius)
    : position(xPercent, yPercent), sizePercent(sizePercent), color(color), hoverColor(hoverColor), value(value), range(range), cornerRadius(cornerRadius) {
}


void Slider::Draw() {

    // Obtenez les dimensions de la fenêtre
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
 
    // Calculez la taille et la position réelles
    ImVec2 actualSize = ImVec2(sizePercent.x * winWidth, sizePercent.y * winHeight);
    ImVec2 actualPos = ImVec2((position.x * winWidth) - (actualSize.x * 0.5), (position.y * winHeight) - (actualSize.y * 0.5));

    // Normalisez la valeur actuelle du slider en fonction de la plage
    float normalizedValue = (*value - range.x) / (range.y - range.x);
    normalizedValue = normalizedValue < 0 ? 0 : (normalizedValue > 1 ? 1 : normalizedValue); // Clampez entre 0 et 1
    float cursorPosX = actualPos.x + normalizedValue * actualSize.x;



    // Dessinez le rectangle principal du slider
    drawList->AddRectFilled(actualPos, ImVec2(actualPos.x + actualSize.x, actualPos.y + actualSize.y), IM_COL32(color.x, color.y, color.z, color.w), cornerRadius);

    float rectWidth = winWidth*0.0020; // Largeur des rectangles aux extrémités
    float extraHeightFactor = 0.75; // Augmente la hauteur de 50%
    float extraHeight = actualSize.y * extraHeightFactor;

    // Dessinez le rectangle de gauche
    drawList->AddRectFilled(ImVec2(actualPos.x - rectWidth, actualPos.y - extraHeight / 2), 
                            ImVec2(actualPos.x, actualPos.y + actualSize.y + extraHeight / 2), 
                            IM_COL32(color.x, color.y, color.z, color.w));


    // Dessinez le rectangle de droite
    drawList->AddRectFilled(ImVec2(actualPos.x + actualSize.x, actualPos.y - extraHeight / 2), 
                            ImVec2(actualPos.x + actualSize.x + rectWidth, actualPos.y + actualSize.y + extraHeight / 2), 
                            IM_COL32(color.x, color.y, color.z, color.w));
    
    //Curseur
    float cursorWidth = winWidth*0.0025; // Largeur du curseur
    float cursorExtraHeight = actualSize.y * extraHeightFactor*1.75;
    float cursorPosY = actualPos.y - cursorExtraHeight / 2;  // Ajustez la position Y du curseur

    // Calculez la position X du curseur de manière à ce qu'il ne dépasse pas les rectangles d'extrémité
    float minCursorPosX = actualPos.x + cursorWidth / 2.0f; // Position minimale
    float maxCursorPosX = actualPos.x + actualSize.x - cursorWidth / 2.0f; // Position maximale
    cursorPosX = std::min(std::max(cursorPosX, minCursorPosX), maxCursorPosX);

    ImVec2 cursorPos = ImVec2(cursorPosX - cursorWidth / 2.0f, cursorPosY);
    drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + cursorWidth, cursorPos.y + actualSize.y + cursorExtraHeight), IM_COL32(220, 220, 220, 220)); // Curseur blanc

    // Gérez l'interaction avec la souris
    ImVec2 mousePos = ImGui::GetMousePos();
    isHovered = ImGui::IsMouseHoveringRect(actualPos, ImVec2(actualPos.x + actualSize.x, actualPos.y + actualSize.y));
    
    if (isHovered && ImGui::IsMouseDown(0)) {
        isDragging = true;
    } else if (!ImGui::IsMouseDown(0)) {
        isDragging = false;
    }

    if (isDragging) {
        // Mettez à jour la valeur du slider en fonction de la position de la souris
        float newValue = (mousePos.x - actualPos.x) / actualSize.x;
        newValue = newValue < 0 ? 0 : (newValue > 1 ? 1 : newValue); // Clampez entre 0 et 1
        *value = range.x + newValue * (range.y - range.x); // Convertissez en valeur de la plage
        drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + cursorWidth, cursorPos.y + actualSize.y + cursorExtraHeight), IM_COL32(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w));
    }

    // Dessinez l'effet de survol si nécessaire
    if (isHovered && !isDragging) {
        drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + cursorWidth, cursorPos.y + actualSize.y + cursorExtraHeight), IM_COL32(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w)); // Curseur blanc
    }
}
