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
    std::function<void()> onClick; 
    std::string label;
    ImFont* font;
    float alpha;
    float cornerRadius;

    //Sound
    Mix_Chunk* hoverSound = nullptr;
    Mix_Chunk* clickSound = nullptr;
    bool hoverSoundPlayed = false; 

    bool mouseButtonPressed = false;

    bool enabled = true;
    bool hidden = false;

public:
    Button(float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 hoverColor, const std::string& label, ImFont* font, float alpha, std::function<void()> onClickAction);
    ~Button(); 
    void Draw();
    void updateLabel(const std::string& newLabel);
    bool InitSoundEffects();
};