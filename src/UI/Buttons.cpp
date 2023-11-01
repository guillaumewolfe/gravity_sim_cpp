#include "UI/Buttons.h"



Button::Button(float xPercent, float yPercent, ImVec2 size, ImVec4 color, ImVec4 hoverColor, const std::string& label, ImFont* font, float alpha, std::function<void()> onClickAction)
    : position(xPercent, yPercent), size(size), color(color), hoverColor(hoverColor), onClick(onClickAction), label(label), font(font), alpha(alpha) {}


void Button::Draw() {
    // Calculate actual position and size based on window dimensions
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Calculate the position based on the logical window size
    ImVec2 actualPos = ImVec2((position.x * winWidth) - (size.x * 0.5), 
                            (position.y * winHeight) - (size.y * 0.5));

    // Set the cursor position
    ImGui::SetCursorPos(actualPos);
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    bool isHovered = ImGui::IsMouseHoveringRect(cursorPos, ImVec2(cursorPos.x + size.x, cursorPos.y + size.y));

    if (isHovered) {
        drawList->AddRectFilled(cursorPos, 
                                ImVec2(cursorPos.x + size.x, cursorPos.y + size.y), 
                                IM_COL32(hoverColor.x * 255, hoverColor.y * 255, hoverColor.z * 255, alpha * 255));
    } else {
        drawList->AddRectFilled(cursorPos, 
                                ImVec2(cursorPos.x + size.x, cursorPos.y + size.y), 
                                IM_COL32(color.x * 255, color.y * 255, color.z * 255, alpha * 255));
    }

    // Drawing the label
    ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
    ImVec2 textPos = ImVec2(cursorPos.x + (size.x - textSize.x) * 0.5, 
                            cursorPos.y + (size.y - textSize.y) * 0.5); // Centering the text

    if (font) {
        drawList->AddText(font, font->FontSize, textPos, 
                          IM_COL32(255, 255, 255,255), 
                          label.c_str());
    } else {
        drawList->AddText(textPos, 
                          IM_COL32(255, 255, 255,255), 
                          label.c_str());}


    if (isHovered && ImGui::IsMouseClicked(0)) {
        if (onClick) {
            onClick(); // Invoke the action associated with the button
        }
    }
}