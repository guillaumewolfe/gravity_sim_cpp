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
    ImVec2 position = ImVec2(0, 0);
    ImVec2 sizePercent = ImVec2(0, 0);
    ImVec4 color = ImVec4(0, 0, 0, 0);
    ImVec4 hoverColor = ImVec4(0, 0, 0, 0);
    float* value = nullptr;
    ImVec2 range = ImVec2(0,1); // Nouveau membre pour la plage de valeurs
    float cornerRadius = 0.0f;
    bool isHovered = false;
    bool isDragging = false;
};


