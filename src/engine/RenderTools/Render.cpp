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
    Objects_Tool = new ObjectsTool(Context);
    Background_Tool = new BackgroundTool(Context);
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
    if (Message_Tool != nullptr) {Message_Tool->Draw();}
    if (Message_Tool != nullptr && Message_Tool->shouldClose){delete Message_Tool;Message_Tool = nullptr;}
    ImGui::Render();
    
    updateCamera();
    Background_Tool->Draw();
    Objects_Tool->Draw();







    //On dessine les éléments du GUI à la fin
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Render::updateCamera(){
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    Context->currentCamera->lookAt();
    glPopMatrix();
}