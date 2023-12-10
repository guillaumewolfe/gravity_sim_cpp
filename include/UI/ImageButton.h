#pragma once

#include <iostream>
#include <functional>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>
#include <SDL_mixer.h> 

namespace cv {
    class Mat;
}

class ImageButton {
private:
    ImVec2 position; 
    ImVec2 sizePercent; 
    ImVec4 color;   
    ImVec4 hoverColor; 
    ImVec4 labelColor;
    GLuint textureNormal;
    GLuint textureHover;
    GLuint textureClicked;
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
    float imageScale; 


public:

    ImageButton(float xPercent, float yPercent, ImVec2 sizePercent,float imageScalePercent, ImVec4 color, ImVec4 hoverColor, const std::string& imagePath, float alpha, std::function<void()> onClickAction, float cornerRadius, bool isRound, ImVec4 borderColor, bool isContinuedClick);
    ~ImageButton(); 
    void Draw();
    void updateLabel(const std::string& newLabel);
    bool InitSoundEffects();
    bool enabled = true;
    bool hidden = false;
    bool isOn;
    void changeColor(ImVec4 newColor);
    void UpdatePosition(float xPerc, float yPerc);
    void loadTexture(const char* filename);
    GLuint createTextureWithTint(const cv::Mat& originalImage, float tint);
};
