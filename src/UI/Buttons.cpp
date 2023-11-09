#include "UI/Buttons.h"

// Constructor
Button::Button(float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 hoverColor, const std::string& label, ImFont* font, float alpha, std::function<void()> onClickAction)
    : position(xPercent, yPercent), sizePercent(sizePercent), color(color), hoverColor(hoverColor), onClick(onClickAction), label(label), font(font), alpha(alpha) {
        if (!InitSoundEffects()) {
            std::cout<<"Failed to load sound at init"<<std::endl;
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
// Draw method
void Button::Draw() {
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
    bool isHovered = ImGui::IsMouseHoveringRect(cursorPos, ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y));
    if(!hoverSound){
        std::cout<<"ERROR"<<std::endl;
    }
    if (isHovered && !hoverSoundPlayed) {
        Mix_PlayChannel(-1, hoverSound, 0); // Play the hover sound effect
        hoverSoundPlayed = true; // Set the flag to true to indicate sound has been played
    }else if (!isHovered && hoverSoundPlayed)
    {
        hoverSoundPlayed=false;
    }
    
    // Draw the button rectangle
    drawList->AddRectFilled(cursorPos, 
                            ImVec2(cursorPos.x + actualSize.x, cursorPos.y + actualSize.y), 
                            isHovered ? IM_COL32(hoverColor.x * 255, hoverColor.y * 255, hoverColor.z * 255, alpha * 255)
                                      : IM_COL32(color.x * 255, color.y * 255, color.z * 255, alpha * 255));

    // Draw the label
    ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
    ImVec2 textPos = ImVec2(cursorPos.x + (actualSize.x - textSize.x) * 0.5, 
                            cursorPos.y + (actualSize.y - textSize.y) * 0.5); // Centering the text

    if (font) {
        drawList->AddText(font, font->FontSize, textPos, 
                          IM_COL32(255, 255, 255,255), 
                          label.c_str());
    } else {
        drawList->AddText(textPos, 
                          IM_COL32(255, 255, 255,255), 
                          label.c_str());
    }

    // Check for button press
    if (isHovered && ImGui::IsMouseClicked(0)) {
        if (onClick) {
            onClick(); // Invoke the action associated with the button
        }
    }

}

// Additional methods for Button class could be implemented here (e.g., setters/getters, interaction logic, etc.)
void Button::updateLabel(const std::string& newLabel){
    label = newLabel;
}


// Initialize sound effects
bool Button::InitSoundEffects() {
    // Open audio with desired parameters
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cerr << "SDL_mixer could not initialize audio: " << Mix_GetError() << std::endl;
        return false;
    }

    // Load sound effects (replace with your sound file paths)
    hoverSound = Mix_LoadWAV("../assets/sounds/select.wav");
    //clickSound = Mix_LoadWAV("../assets/sounds/start.wav");

    if (!hoverSound /* || !clickSound */) {
        std::cerr << "Failed to load sound effects: " << Mix_GetError() << std::endl;
        Mix_CloseAudio(); // Close audio before returning
        return false;
    }

    return true; // Sound effects initialized successfully
}


