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

class Icon {
public:
    Icon(float xPercent, float yPercent, ImVec2 sizePercent, float imageScalePercent, const std::string& imagePath, float alpha = 1.0f);
    ~Icon();
    void Draw();
    void loadTexture(const char* filename);
    void UpdatePosition(float xPerc, float yPerc);
    GLuint createTexture(const cv::Mat& originalImage);
    bool enabled = true;
    std::string imagePath;
    ImVec2 getPosition();
    ImVec2 getSize();

private:
    ImVec2 position;
    ImVec2 sizePercent;
    float imageScale;
    GLuint texture;
    float alpha;
};