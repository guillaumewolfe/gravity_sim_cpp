#include "UI/Labbel.h"
#include <filesystem>
#include <iostream>

Labbel::Labbel(float xPercent, float yPercent, ImVec4 color, const std::string& label, ImFont* font, float alpha)
:position(xPercent, yPercent),color(color),label(label), font(font), alpha(alpha){}


void Labbel::Draw(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Calculez la largeur et la hauteur du texte en fonction de la police et de la taille du texte
    ImGui::PushFont(font);
    ImVec2 textSize = ImGui::CalcTextSize(label.c_str(), NULL, false, 0.0f);
    ImGui::PopFont();
    // Calculez la position réelle en tenant compte de la largeur du texte pour le centrer

    if (shouldUpdatePosition(textSize)) {
        prevTextSize = textSize; // Update the stored text size
    }
    ImVec2 actualPos = ImVec2(
        (position.x * winWidth) - (prevTextSize.x * 0.5f), // Center the text horizontally
        (position.y * winHeight) - (textSize.y * 0.5f)     // Center the text vertically
    );
    // Ensuite, utilisez actualPos pour positionner le texte
    drawList->AddText(
        font, font->FontSize,
        actualPos,
        IM_COL32(255, 255, 255, 255 * alpha),  // Utilisez alpha pour la transparence
        label.c_str()
    );
}


bool Labbel::shouldUpdatePosition(const ImVec2& newTextSize) {
    if (prevTextSize.x == 0) { // If this is the first time, we always update
        return true;
    }
    float changeInWidth = std::abs(prevTextSize.x - newTextSize.x);
    return (changeInWidth / prevTextSize.x) > sizeChangeThreshold;
}


void Labbel::UpdateText(const std::string& newText) {
    label = newText;
}