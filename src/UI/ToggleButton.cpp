#include "UI/ToggleButton.h"

ToggleButton::ToggleButton(float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color,  ImVec4 activeColor, bool* value, float cornerRadius)
    : position(xPercent, yPercent), sizePercent(sizePercent), color(color), activeColor(activeColor), value(value), cornerRadius(cornerRadius) {
}

void ToggleButton::Draw() {
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 actualSize = ImVec2(sizePercent.x * winWidth, sizePercent.y * winHeight);
    ImVec2 actualPos = ImVec2((position.x * winWidth) - (actualSize.x * 0.5), (position.y * winHeight) - (actualSize.y * 0.5));

    ImVec4 currentColor = *value ? activeColor : color;

    // Dessiner le rectangle du bouton
    drawList->AddRectFilled(actualPos, ImVec2(actualPos.x + actualSize.x, actualPos.y + actualSize.y), IM_COL32(currentColor.x, currentColor.y, currentColor.z, currentColor.w), cornerRadius);

    // Calculez la position du cercle
    float circleMargin = actualSize.y * 0.05; // Marge de 5% de la hauteur
    float circlePosX;
    if (*value) {
        circlePosX = actualPos.x + actualSize.x - actualSize.y / 2 - circleMargin; // Décaler de 5% pour l'état "on"
    } else {
        circlePosX = actualPos.x + actualSize.y / 2 + circleMargin; // Décaler de 5% pour l'état "off"
    }
    float circlePosY = actualPos.y + actualSize.y / 2;

    // Dessinez le cercle du bouton
    drawList->AddCircleFilled(ImVec2(circlePosX, circlePosY), actualSize.y / 2, IM_COL32(245,245,245, 255));

    // Détecter les interactions
    ImVec2 detectionPos = actualPos;
    ImVec2 detectionSize = actualSize;

    bool isHovered = ImGui::IsMouseHoveringRect(detectionPos, ImVec2(detectionPos.x + detectionSize.x, detectionPos.y + detectionSize.y));
    if (isHovered && ImGui::IsMouseClicked(0)) {
        *value = !*value; // Basculer la valeur
    }
}
