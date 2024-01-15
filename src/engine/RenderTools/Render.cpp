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
    Objects_Tool = new ObjectsTool(Context, Context->currentCamera);
    Background_Tool = new BackgroundTool(Context);
    Axes_Tool = new AxesTool(Context);
    Path_Tool = new PathTool(Context);
    Creator_Manager = new CreatorManager(Context);
    Name_Tool = new NameTool(Context);
    PlaneteInfo_Tool = new PlaneteInfoTool(Context);
    CameraOptions_Tool = new CameraOptionsTool(Context);
    Options_Tool = new OptionsTool(Context);
    Settings_Tool = new SettingsTool(Context);
    Zoom_Tool = new ZoomTool(Context);
    Orbit_Tool = new OrbitTool(Context);
    Minimap_Tool = new MinimapTool(Context);
    Keys_UI = new KeysUI(Context);
    Collision_Tool = new CollisionTool(Context);

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
    if(*(Context->showCameraOptions)){CameraOptions_Tool->Draw();}
    if(*(Context->isCreating)){Creator_Manager->Draw();}
    if(!Context->showZoom){Name_Tool->Draw();}
    if(Context->showMinimap){Minimap_Tool->Draw();}
    if((Context->currentCamera->followedObject!=nullptr ||Context->currentCamera->selectedObject!=nullptr ) && *(Context->showInfo) && !Context->showZoom){PlaneteInfo_Tool->Draw();}
    if(Context->showZoom){Zoom_Tool->drawImGui();}
    UI_Tool->Draw();
    if(Context->showControls){Keys_UI->Draw();}
    if(*(Context->showSettings)){Settings_Tool->Draw();}
    if(*(Context->showOptions)){Options_Tool->Draw();}
    if (Message_Tool != nullptr) {Message_Tool->Draw();}
    if (Message_Tool != nullptr && Message_Tool->shouldClose){delete Message_Tool;Message_Tool = nullptr;}
    ImGui::Render();

    updateCamera();
    Background_Tool->Draw();
    Objects_Tool->Draw();
    if(Context->showZoom){Zoom_Tool->Draw();}
    if(*(Context->showAxes)){Axes_Tool->Draw();}
    //if(!*(Context->isCreating)){Path_Tool->Draw();}
    if(!Context->showZoom){Path_Tool->Draw();}
    if(Context->showAllOrbits){Orbit_Tool->Draw();}
    if(Context->showCollision){Collision_Tool->Draw();}


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if((Context->currentCamera->followedObject!=nullptr||Context->currentCamera->selectedObject!=nullptr) && *(Context->showInfo) && !Context->showZoom){PlaneteInfo_Tool->drawTexturedSphere();}
    if(*(Context->isCreating)){Creator_Manager->DrawOpenGL();}

}


void Render::updateCamera(){
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    Context->currentCamera->Update();
    glPopMatrix();
}

void Render::initCamera(){
    Vec3 position_initiale = Vec3(-10,0,0);
    Context->currentCamera->setInitPosition(position_initiale);
}


//destructeur
Render::~Render(){
    delete UI_Tool;
    delete Objects_Tool;
    delete Background_Tool;
    delete Axes_Tool;
    delete Path_Tool;
    delete Creator_Manager;
    delete Name_Tool;
    delete PlaneteInfo_Tool;
    delete CameraOptions_Tool;
    delete Options_Tool;
    delete Settings_Tool;
    delete Zoom_Tool;
    delete Orbit_Tool;
    delete Minimap_Tool;
    delete Keys_UI;
}