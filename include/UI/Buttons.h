#pragma once

#include <iostream>
#include <functional>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>
#include <SDL_mixer.h> 

class Button {
private:
    ImVec2 position; 
    ImVec2 sizePercent; 
    ImVec4 color;   
    ImVec4 hoverColor; 
    ImVec4 labelColor;
    std::function<void()> onClick; 
    std::string label;
    ImFont* font;
    float alpha;
    float cornerRadius;
    bool isRound;
    float fontSize;
    //Sound
    Mix_Chunk* hoverSound = nullptr;
    Mix_Chunk* clickSound = nullptr;
    bool hoverSoundPlayed = false; 

    bool mouseButtonPressed = false;


public:
    Button(float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 hoverColor, const std::string& label, float alpha,float fontsize, std::function<void()> onClickAction, float cornerRadius = 10.0f, bool isRound = false, ImVec4 labelColor = ImVec4(255,255,255,240));
    ~Button(); 
    void Draw();
    void updateLabel(const std::string& newLabel);
    bool InitSoundEffects();
    bool enabled = true;
    bool hidden = false;
    void changeColor(ImVec4 newColor);
    void UpdatePosition(float xPerc, float yPerc);
    void UpdateLabelColor(float x,float y,float z,float w);
};
