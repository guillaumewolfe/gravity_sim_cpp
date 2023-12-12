#pragma once

#include <iostream>
#include <functional>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>

class Slider {
public:
    Slider(float xPercent, float yPercent, ImVec2 sizePercent, ImVec4 color, ImVec4 hoverColor, float* value, ImVec2 range, float cornerRadius = 0.0f);
    void Draw();

private:
    ImVec2 position;
    ImVec2 sizePercent;
    ImVec4 color;
    ImVec4 hoverColor;
    float* value;
    ImVec2 range; // Nouveau membre pour la plage de valeurs
    float cornerRadius;
    bool isHovered = false;
    bool isDragging = false;
};


