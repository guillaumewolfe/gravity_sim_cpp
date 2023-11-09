#ifndef GLEW_STATIC
#define GLEW_STATIC // Si vous utilisez GLEW en mode statique
#endif

// Incluez glad.h après les directives de préprocesseur
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/Render.h"


Render::Render(RenderContext* Context): Context(Context){}

void Render::initTools(){
    UI_Tool = new UITool(Context);
}

void Render::Draw(){
    //Clear Buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //New frame pour les éléments GUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //On dessiner les éléments
    UI_Tool->Draw();
    ImGui::Render();

    




    //On dessine les éléments du GUI à la fin
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


