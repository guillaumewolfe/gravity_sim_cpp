#pragma once

#include <iostream>
#include <functional>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>

class Button {
private:
    ImVec2 position; 
    ImVec2 size;    
    ImVec4 color;   
    ImVec4 hoverColor; 
    std::function<void()> onClick; 
    std::string label;
    ImFont* font;
    float alpha;

public:
    Button(float xPercent, float yPercent, ImVec2 size, ImVec4 color, ImVec4 hoverColor, const std::string& label, ImFont* font, float alpha, std::function<void()> onClickAction);
    void Draw();
};