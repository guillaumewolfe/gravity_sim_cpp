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
    ImFont* font;
    float fontSize;
    ImVec2 position; 
    ImVec4 color;   
    float alpha;
    std::string label;

    ImVec2 prevTextSize = ImVec2(0, 0);
    const float sizeChangeThreshold = 0.1f; 
    bool shouldUpdatePosition(const ImVec2& newTextSize);

public:
    Labbel(float xPercent, float yPercent, ImVec4 color, const std::string& label, float fontsize, float alpha);
    void Draw();
    void UpdateText(const std::string& newText);
    void UpdateAlpha(const float opp);
    void UpdatePosition(float xPercent, float yPercent);
    ImVec2 getPosition();
    void UpdateColor(ImVec4 newColor);
};