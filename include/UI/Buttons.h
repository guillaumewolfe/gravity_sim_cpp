#pragma once

#include <iostream>
#include <functional>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>
#include <SDL_mixer.h> 

class Game;

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
    ImFont* fontHover;
    float alpha;
    float cornerRadius;
    bool isRound;
    float fontSize;
    bool isContinuedClick;
    //Sound
    Mix_Chunk* hoverSound = nullptr;
    Mix_Chunk* clickSound = nullptr;
    bool hoverSoundPlayed = false; 

    bool mouseButtonPressed = false;
    std::string clickSoundName;


    std::function<void(std::string)> playSoundFunc;

public:
    Button(std::function<void(std::string)> playSoundFunc,float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 hoverColor, const std::string& label, float alpha,float fontsize, std::function<void()> onClickAction, float cornerRadius = 10.0f,std::string clickSound = "normal", bool isRound = false, ImVec4 labelColor = ImVec4(255,255,255,240), bool isContinuedClick = false);
    ~Button(); 
    void Draw();
    void updateLabel(const std::string& newLabel);
    bool enabled = true;
    bool hidden = false;
    void changeColor(ImVec4 newColor);
    void UpdatePosition(float xPerc, float yPerc);
    void UpdateLabelColor(float x,float y,float z,float w);
    void turnOff();
    void turnOn();
    ImVec2 getPosition();
    bool isOn;
    void UpdateText(std::string newText);
    void UpdateFunction(std::function<void()> newFunction);

    bool soundFunctionValid = false;
};
