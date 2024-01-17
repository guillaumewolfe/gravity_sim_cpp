#include "glad/glad.h"
#include "UI/Buttons.h"
#include "opencv2/opencv.hpp"

// Constructor
Button::Button(float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 hoverColor, const std::string& label,  float alpha,float fontsize, std::function<void()> onClickAction,float cornerRadius,std::string clickSound, bool isRound, ImVec4 labelColor, bool isContinuedClick)
    : position(xPercent, yPercent), sizePercent(sizePercent), color(color), hoverColor(hoverColor), onClick(onClickAction), label(label),  alpha(alpha),fontSize(fontsize),cornerRadius(cornerRadius), isRound(isRound), labelColor(labelColor), isContinuedClick(isContinuedClick), clickSoundName(clickSound)  {
        if (!InitSoundEffects()) {
            std::cout<<"Failed to load sound at init"<<std::endl;
        }
    if(fontSize==0){font = ImGui::GetFont(); }
    else{
        int winWidth, winHeight;
        glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
        float fontSizeScaled = fontSize * winWidth / 1920;
        font = ImGui::GetIO().Fonts->AddFontFromFileTTF("../assets/fonts/RobotoB.ttf", fontSizeScaled);}
    if (!font) {
        // Gérer le cas où le chargement de la police échoue
        std::cerr << "Erreur lors du chargement de la police." << std::endl;
        font = ImGui::GetFont(); // Utilisez la police par défaut comme solution de repli
    }
    }
Button::~Button() {
    // Free sound effects if they were loaded
    if (hoverSound) {
        Mix_FreeChunk(hoverSound);
        hoverSound = nullptr;
    }
    if (clickSound) {
        Mix_FreeChunk(clickSound);
        clickSound = nullptr;
    }
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
        Mix_PlayChannel(-1, hoverSound, 0);
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
                Mix_PlayChannel(-1, clickSound, 0);
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

// Initialize sound effects
bool Button::InitSoundEffects() {
    // Open audio with desired parameters
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cerr << "SDL_mixer could not initialize audio: " << Mix_GetError() << std::endl;
        return false;
    }

    // Load sound effects
    hoverSound = getSoundPath("hover");
    clickSound = getSoundPath(clickSoundName);


    if (!hoverSound  || !clickSound ) {
        std::cerr << "Failed to load sound effects: " << Mix_GetError() << std::endl;
        Mix_CloseAudio(); // Close audio before returning
        return false;
    }
    SetSoundVolume(0.7f);  // Réglez le volume à la moitié pour l'exemple
    return true; // Sound effects initialized successfully
}

void Button::UpdatePosition(float xPerc, float yPerc){
    position = ImVec2(xPerc,yPerc);
}

void Button::UpdateLabelColor(float x,float y,float z,float w) {
    labelColor = ImVec4(x,y,z,w);
}

Mix_Chunk* Button::getSoundPath(const std::string& name) {
    if (name == "hover") {return Mix_LoadWAV("../assets/sounds/select.wav");} 
    else if (name == "normal") {return Mix_LoadWAV("../assets/sounds/delete.wav");} 
    else if (name == "start"){return Mix_LoadWAV("../assets/sounds/startSim.wav");}
    else if (name == "creation"){return Mix_LoadWAV("../assets/sounds/confirm_creation.wav");}
    else if (name == "restart"){return Mix_LoadWAV("../assets/sounds/restart.mp3");}



    else {return nullptr;}
}


void Button::SetSoundVolume(float volume) {
    int volumeInt = volume * 128;
    Mix_VolumeChunk(hoverSound, volumeInt);
    Mix_VolumeChunk(clickSound, volumeInt);
}

ImVec2 Button::getPosition(){
    return position;
}