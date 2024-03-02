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

    // Dessinez le rectangle de progression
    drawList->AddRectFilled(actualPos, ImVec2(cursorPosX, actualPos.y + actualSize.y), IM_COL32(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w), cornerRadius);

    // Curseur
    float cursorRadius = actualSize.y * 0.5; // Rayon du cercle basé sur la hauteur du slider
    float cursorPosY = actualPos.y + actualSize.y / 2.0f; // Centre Y du cercle

    // Calculez la position X du curseur de manière à ce qu'il ne dépasse pas les limites du slider
    float minCursorPosX = actualPos.x; // Position minimale
    float maxCursorPosX = actualPos.x + actualSize.x; // Position maximale
    cursorPosX = std::min(std::max(cursorPosX, minCursorPosX), maxCursorPosX);

    // Dessinez le cercle du curseur
    float cursorRadiusCircle = 3*cursorRadius;
    drawList->AddCircleFilled(ImVec2(cursorPosX, cursorPosY), cursorRadiusCircle, IM_COL32(hoverColor.x, hoverColor.y, hoverColor.z, 255)); // Curseur blanc

    float detectionHeight = cursorRadiusCircle * 3; // Hauteur basée sur le rayon du curseur
    ImVec2 detectionPos = ImVec2(actualPos.x, actualPos.y + actualSize.y / 2 - detectionHeight / 2);
    ImVec2 detectionSize = ImVec2(actualSize.x, detectionHeight);

    isHovered = ImGui::IsMouseHoveringRect(detectionPos, ImVec2(detectionPos.x + detectionSize.x, detectionPos.y + detectionSize.y));
    if(isHovered)ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    if (isHovered && ImGui::IsMouseDown(0)) {
        isDragging = true;
    } else if (!ImGui::IsMouseDown(0)) {
        isDragging = false;
    }
    ImVec2 mousePos = ImGui::GetMousePos();
    if (isDragging && isHovered) {
        // Mettez à jour la valeur du slider en fonction de la position de la souris
        float newValue = (mousePos.x - actualPos.x) / actualSize.x;
        newValue = newValue < 0 ? 0 : (newValue > 1 ? 1 : newValue); // Clampez entre 0 et 1
        *value = range.x + newValue * (range.y - range.x); // Convertissez en valeur de la plage

        // Mettez à jour la barre de progression et le curseur
        cursorPosX = std::min(std::max(actualPos.x + newValue * actualSize.x, minCursorPosX), maxCursorPosX);
        drawList->AddRectFilled(actualPos, ImVec2(cursorPosX, actualPos.y + actualSize.y), IM_COL32(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w), cornerRadius);
        //drawList->AddCircleFilled(ImVec2(cursorPosX, cursorPosY), cursorRadiusCircle, IM_COL32(220, 220, 220, 220));
    }

    if (isHovered && !isDragging) {
        //drawList->AddCircleFilled(ImVec2(cursorPosX, cursorPosY), cursorRadiusCircle, IM_COL32(220, 220, 220, 220));
    }
}
