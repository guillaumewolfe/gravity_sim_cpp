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
    ImVec2 position = ImVec2(0, 0); 
    ImVec2 sizePercent = ImVec2(0, 0); 
    ImVec4 color = ImVec4(0,0,0,0);   
    ImVec4 hoverColor = ImVec4(0,0,0,0); 
    ImVec4 labelColor = ImVec4(0,0,0,0);
    GLuint textureNormal = 0;
    GLuint textureHover = 0;
    GLuint textureClicked = 0;
    std::function<void()> onClick = nullptr; 
    std::string label="";
    ImFont* font = nullptr;
    ImFont* fontHover = nullptr;
    float alpha = 1.0f;
    float cornerRadius = 10.0f;
    bool isRound = false;
    float fontSize = 20.0f;
    bool isContinuedClick = false;
    //Sound
    Mix_Chunk* hoverSound = nullptr;
    Mix_Chunk* clickSound = nullptr;
    bool hoverSoundPlayed = false; 
    std::string clickSoundName = "normal";

    bool mouseButtonPressed = false;
    float imageScale = 1.0f; 
    std::function<void(std::string)> playSoundFunc = nullptr;
    bool soundFunctionValid = false;


public:

    ImageButton(std::function<void(std::string)> playSoundFunc,float xPercent, float yPercent, ImVec2 sizePercent,float imageScalePercent, ImVec4 color, ImVec4 hoverColor, const std::string& imagePath, float alpha, std::function<void()> onClickAction, float cornerRadius, bool isRound, ImVec4 borderColor, bool isContinuedClick,std::string clickSoundName = "normal");
    ~ImageButton(); 
    void Draw();
    void updateLabel(const std::string& newLabel);
    bool enabled = true;
    bool hidden = false;
    bool isOn = false;
    void changeColor(ImVec4 newColor);
    void UpdatePosition(float xPerc, float yPerc);
    void loadTexture(const char* filename);
    GLuint createTextureWithTint(const cv::Mat& originalImage, float tint);
    void turnOff();
    void turnOn();
    ImVec2 getPosition();
};
