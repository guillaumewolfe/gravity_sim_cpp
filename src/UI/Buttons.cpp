#include "glad/glad.h"
#include "UI/Buttons.h"
#include "opencv2/opencv.hpp"
#include "path_util.h"

// Constructor
Button::Button(std::function<void(std::string)> playSoundFunc,float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 hoverColor, const std::string& label,  float alpha,float fontsize, std::function<void()> onClickAction,float cornerRadius,std::string clickSound, bool isRound, ImVec4 labelColor, bool isContinuedClick)
    :playSoundFunc(playSoundFunc), position(xPercent, yPercent), sizePercent(sizePercent), color(color), hoverColor(hoverColor), onClick(onClickAction), label(label),  alpha(alpha),fontSize(fontsize),cornerRadius(cornerRadius), isRound(isRound), labelColor(labelColor), isContinuedClick(isContinuedClick), clickSoundName(clickSound)  {
        
    if(playSoundFunc){
        soundFunctionValid = true;
    }
    if(fontSize==0){font = ImGui::GetFont(); }
    else{
        int winWidth, winHeight;
        glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
        float fontSizeScaled = fontSize * winWidth / 1920;
        font = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/RobotoB.ttf").c_str(), fontSizeScaled);}
    if (!font) {
        // Gérer le cas où le chargement de la police échoue
        std::cerr << "Erreur lors du chargement de la police." << std::endl;
        font = ImGui::GetFont(); // Utilisez la police par défaut comme solution de repli
    }
    }
Button::~Button() {
}
void Button::Draw() {
if(hidden){return;}
    // Get window dimensions
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Calculate actual size based on window dimensions and percentage values
    ImVec2 actualSize = ImVec2(sizePercent.x * winWidth, sizePercent.y * winHeight);

    // Calculate the position based on the logical window size
    ImVec2 actualPos = ImVec2((position.x * winWidth) - (actualSize.x * 0.5),
                                (position.y * winHeight) - (actualSize.y * 0.5));

    // Set the cursor position for ImGui
    ImGui::SetCursorPos(actualPos);
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();

    // Get the draw list from ImGui
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Check if the button is hovered
    ImVec2 center = ImVec2(cursorPos.x + actualSize.x * 0.5, cursorPos.y + actualSize.y * 0.5);
    float radius = (actualSize.x < actualSize.y ? actualSize.x : actualSize.y) * 0.5;

    bool isHovered;
    if (isRound) {
        // Check hover for a circle
        ImVec2 mousePos = ImGui::GetMousePos();
        isHovered = (mousePos.x - center.x) * (mousePos.x - center.x) +
                    (mousePos.y - center.y) * (mousePos.y - center.y) <= radius * radius;
    } else {
        // Check hover for a rectangle
        isHovered = ImGui::IsMouseHoveringRect(cursorPos, ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y));
    }

    // Hover sound logic
    if (isHovered && !hoverSoundPlayed&&enabled) {
        if(soundFunctionValid){playSoundFunc("hover");}
        hoverSoundPlayed = true;
    } else if (!isHovered && hoverSoundPlayed) {
        hoverSoundPlayed = false;
    }

    // Determine button shape
    if (isRound) {
        // Draw a circle if isRound is true
        ImVec2 center = ImVec2(cursorPos.x + actualSize.x * 0.5, cursorPos.y + actualSize.y * 0.5);
        float radius = (actualSize.x < actualSize.y ? actualSize.x : actualSize.y) * 0.5;
        drawList->AddCircleFilled(center, radius,
                                    IM_COL32(color.x * 255, color.y * 255, color.z * 255, alpha * 255));
    } else {
        // Draw a rectangle if isRound is false
        drawList->AddRectFilled(cursorPos,
                                ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y),
                                IM_COL32(color.x * 255, color.y * 255, color.z * 255, alpha * 255), cornerRadius);

    }

    // Draw the label
ImGui::PushFont(font);
    ImVec2 textSize = ImGui::CalcTextSize(label.c_str());   
ImGui::PopFont();
    ImVec2 textPos = ImVec2(cursorPos.x + (actualSize.x - textSize.x) * 0.5,
                            cursorPos.y + (actualSize.y - textSize.y) * 0.5);

ImU32 labelColorIM32 = IM_COL32((int)labelColor.x, (int)labelColor.y, (int)labelColor.z, (int)labelColor.w);
if(isOn){ImU32 labelColorIM32 = IM_COL32((int)255, (int)255, (int)255, (int)255);}
if (font) {
    drawList->AddText(font, font->FontSize, textPos,
                      labelColorIM32,
                      label.c_str());
} else {
        drawList->AddText(textPos,
                            labelColorIM32,
                            label.c_str());
    }

    // Button hover effect
    if (isHovered && enabled) {
        if (isRound) {
            // Hover effect for circle
            ImVec2 center = ImVec2(cursorPos.x + actualSize.x * 0.5, cursorPos.y + actualSize.y * 0.5);
            float radius = (actualSize.x < actualSize.y ? actualSize.x : actualSize.y) * 0.5;
            drawList->AddCircleFilled(center, radius,
                                        IM_COL32(hoverColor.x * 255, hoverColor.y * 255, hoverColor.z * 255, alpha * 255));
        } else {
            // Hover effect for rectangle
            drawList->AddRectFilled(cursorPos,
                                    ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y),
                                    IM_COL32(hoverColor.x * 255, hoverColor.y * 255, hoverColor.z * 255, alpha * 255), cornerRadius);
            drawList->AddRect(cursorPos,
                                    ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y),
                                    IM_COL32(hoverColor.x * 255, hoverColor.y * 255, hoverColor.z * 255, alpha * 255*1.3), cornerRadius);
            drawList->AddText(font, font->FontSize, textPos,
                    labelColorIM32,
                    label.c_str());
        }
    }
    if(isOn){
    float rectHeight = actualSize.y * 0.075; // 10% de la hauteur du bouton
    float rectWidth = actualSize.x * 0.70;// 10% de la largeur du bouton
    float offsetX = (actualSize.x - rectWidth) / 2; // Demi-différence entre les largeurs du bouton et du rectangle
    ImVec2 rectTopLeft = ImVec2(actualPos.x + offsetX, actualPos.y + actualSize.y - rectHeight);
    ImVec2 rectBottomRight = ImVec2(rectTopLeft.x + rectWidth, actualPos.y + actualSize.y);
    drawList->AddRectFilled(rectTopLeft, rectBottomRight, IM_COL32(255, 255, 255, 255),cornerRadius);
    drawList->AddRectFilled(cursorPos,
                            ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y),
                            IM_COL32(255, 255, 255, 5), cornerRadius);}

    // Button click detection
    if (isHovered && ImGui::IsMouseClicked(0)) {
        mouseButtonPressed = true;
    }
    if (!isHovered && ImGui::IsMouseReleased(0) && mouseButtonPressed) {
        mouseButtonPressed = false;
    }
    if (mouseButtonPressed && isHovered) {
        if (isRound) {
            // Click effect for circle
            ImVec2 center = ImVec2(cursorPos.x + actualSize.x * 0.5, cursorPos.y + actualSize.y * 0.5);
            float radius = (actualSize.x < actualSize.y ? actualSize.x : actualSize.y) * 0.5;
            drawList->AddCircleFilled(center, radius,
                                        IM_COL32(255, 255, 255, 30), cornerRadius);
        } else {
            // Click effect for rectangle
            drawList->AddRectFilled(cursorPos,
                                    ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y),
                                    IM_COL32(255, 255, 255, 30), cornerRadius);
        }
    }


    if (isContinuedClick) {
        if (isHovered && ImGui::IsMouseDown(0)) {
            // Si le bouton est survolé et que le bouton de la souris est enfoncé
            if (onClick && enabled) {
                onClick(); // Appel répété de la fonction onClick
            }
            // Ajoutez ici la logique d'affichage pour le clic maintenu, si nécessaire
        }
    } else {
        // Logique existante pour le mode non continu
        if (isHovered && ImGui::IsMouseReleased(0) && mouseButtonPressed) {
            if (onClick && enabled) {
                if(soundFunctionValid){playSoundFunc(clickSoundName);}
                onClick(); // Invocation de l'action associée au bouton
            }
            mouseButtonPressed = false; // Réinitialiser le statut du bouton pressé
        }
    }
}




// Additional methods for Button class could be implemented here (e.g., setters/getters, interaction logic, etc.)
void Button::updateLabel(const std::string& newLabel){
    label = newLabel;
}

void Button::changeColor(ImVec4 newColor){
    color = newColor;
}



void Button::UpdatePosition(float xPerc, float yPerc){
    position = ImVec2(xPerc,yPerc);
}

void Button::UpdateLabelColor(float x,float y,float z,float w) {
    labelColor = ImVec4(x,y,z,w);
}





ImVec2 Button::getPosition(){
    return position;
}