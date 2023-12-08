#ifndef GLEW_STATIC
#define GLEW_STATIC // Si vous utilisez GLEW en mode statique
#endif

// Incluez glad.h après les directives de préprocesseur
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/Render.h"


Render::Render(RenderContext* Context): Context(Context){
    initTools();
}

void Render::initTools(){
    UI_Tool = new UITool(Context);
    Objects_Tool = new ObjectsTool(Context);
    Background_Tool = new BackgroundTool(Context);
    Axes_Tool = new AxesTool(Context);
    Path_Tool = new PathTool(Context);
    Creator_Manager = new CreatorManager(Context);
    Name_Tool = new NameTool(Context);
    PlaneteInfo_Tool = new PlaneteInfoTool(Context);
    CameraOptions_Tool = new CameraOptionsTool(Context);

    initCamera();
}

void Render::Draw(){
    //Clear Buffer
    glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //New frame pour les éléments GUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //On dessiner les éléments
    if(!*(Context->isCreating)){UI_Tool->Draw();}
    if(*(Context->showCameraOptions)){CameraOptions_Tool->Draw();}
    if (Message_Tool != nullptr) {Message_Tool->Draw();}
    if (Message_Tool != nullptr && Message_Tool->shouldClose){delete Message_Tool;Message_Tool = nullptr;}
    if(*(Context->isCreating)){Creator_Manager->Draw();}
    Name_Tool->Draw();
    if((Context->currentCamera->followedObject!=nullptr ||Context->currentCamera->selectedObject!=nullptr ) && *(Context->showInfo)){PlaneteInfo_Tool->Draw();}
    ImGui::Render();
    updateCamera();
    Background_Tool->Draw();
    Objects_Tool->Draw();
    if(*(Context->showAxes)){Axes_Tool->Draw();}
    //if(!*(Context->isCreating)){Path_Tool->Draw();}
    Path_Tool->Draw();





    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if((Context->currentCamera->followedObject!=nullptr||Context->currentCamera->selectedObject!=nullptr) && *(Context->showInfo)){PlaneteInfo_Tool->drawTexturedSphere(PlaneteInfo_Tool->winWidth*0.027,40,40);}
    if(*(Context->isCreating)){Creator_Manager->DrawOpenGL();}

}


void Render::updateCamera(){
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    Context->currentCamera->Update();
    glPopMatrix();
}

void Render::initCamera(){
    Vec3 position_initiale = Vec3(-100,20.5236,23.6161);
    Context->currentCamera->setInitPosition(position_initiale);
}
