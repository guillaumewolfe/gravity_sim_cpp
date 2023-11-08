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
    ImFont* myFont;
    ImVec2 position; 
    ImVec4 color;   
    float alpha;
    std::string label;

    ImVec2 prevTextSize = ImVec2(0, 0);
    const float sizeChangeThreshold = 0.1f; 
    bool shouldUpdatePosition(const ImVec2& newTextSize);

public:
    Labbel(float xPercent, float yPercent, ImVec4 color, const std::string& label, ImFont* font, float alpha);
    void Draw();
    void UpdateText(const std::string& newText);
};