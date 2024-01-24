#pragma once
#include <iostream>
#include <functional>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>


class Labbel {
private:
    ImFont* font = nullptr;
    float fontSize = 20.0f;
    ImVec2 position = ImVec2(0, 0); 
    ImVec4 color = ImVec4(0,0,0,0);   
    float alpha = 1.0f;
    std::string label = "";

    ImVec2 prevTextSize = ImVec2(0, 0);
    const float sizeChangeThreshold = 0.1f; 
    bool shouldUpdatePosition(const ImVec2& newTextSize);

public:
    Labbel(float xPercent, float yPercent, ImVec4 color, const std::string& label, float fontsize, float alpha, ImFont* customFont = nullptr);
    void Draw();
    void UpdateText(const std::string& newText);
    void UpdateAlpha(const float opp);
    void UpdatePosition(float xPercent, float yPercent);
    ImVec2 getPosition();
    void UpdateColor(ImVec4 newColor);
    bool isHidden = false;
    bool alignLeft = false;
};