#ifndef GLEW_STATIC
#define GLEW_STATIC // Si vous utilisez GLEW en mode statique
#endif

// Incluez glad.h après les directives de préprocesseur
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/Render.h"
#include "engine/RenderTools/successTool.h"

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
    Notification_Tool = new NotificationTool(Context);
    Context->NotificationTool = Notification_Tool;
    BackgroundImage_Tool = new BackgroundImageTool(Context);
    //ISS_Tool = new ISSTool(Context);
    Quiz_Tool = new QuizTool(Context);
    SaveSimulation_Tool = new SaveSimulationTool(Context);
    Restart_Tool = new RestartTool(Context);
    Welcome_Tool = new WelcomeTool();
    //Success_Tool = new SuccessTool(Context);
    //Success_Tool->setNotificationTool(Notification_Tool);

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
    updateShouldClick();
    if(!Context->showZoom && (!*(Context->isCreating) && Creator_Manager->getStateName()!="PositionCreator")){Name_Tool->Draw();}
    if(*(Context->showCameraOptions)){CameraOptions_Tool->Draw();}
    if(*(Context->isCreating)){Creator_Manager->Draw();}
    if(Context->showMinimap){Minimap_Tool->Draw();}
    if((Context->currentCamera->followedObject!=nullptr ||Context->currentCamera->selectedObject!=nullptr ) && *(Context->showInfo) && !Context->showZoom){PlaneteInfo_Tool->Draw();}
    if(Context->showZoom){Zoom_Tool->drawImGui();}
    if(Context->showControls){Keys_UI->Draw();}
    if(*(Context->showSettings)){Settings_Tool->Draw();}
    if(*(Context->showOptions)){Options_Tool->Draw();}
    if (Message_Tool != nullptr) {Message_Tool->Draw();}
    if (Message_Tool != nullptr && Message_Tool->shouldClose){delete Message_Tool;Message_Tool = nullptr;}
    if(Context->showQuiz){Quiz_Tool->Draw();}
    if(Context->showSaveSimulation){SaveSimulation_Tool->Draw();}
    if(Context->showSuccessTool){Context->successTool->Draw();}
    Context->successTool->checkSuccess();
    if(Context->showWelcomeTool){Welcome_Tool->Draw();}
    if(!Context->showZoom){UI_Tool->Draw();}
    if(Context->showRestartTool){Restart_Tool->Draw();}
    if(Context->showNotificationTool){Notification_Tool->Draw();}
    //Success_Tool->Draw();

    ImGui::Render();

    updateCamera();
    BackgroundImage_Tool->Draw();
    Background_Tool->Draw();
    if(!*(Context->isCreating) or Creator_Manager->getStateName()!="PositionCreator"){Objects_Tool->Draw();}
    if(Context->showZoom){Zoom_Tool->Draw();}
    if(*(Context->showAxes)){Axes_Tool->Draw();}
    if(!Context->showZoom){Path_Tool->Draw();}
    if(!*(Context->isCreating) && !Context->showZoom){Orbit_Tool->Draw();}
    if(Context->showCollision){Collision_Tool->Draw();}
    //ISS_Tool->Draw();


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if((Context->currentCamera->followedObject!=nullptr||Context->currentCamera->selectedObject!=nullptr) && *(Context->showInfo) && !Context->showZoom){PlaneteInfo_Tool->drawTexturedSphere();}
    if(*(Context->isCreating)){Creator_Manager->DrawOpenGL();}
    if(UI_Tool->isSearching){UI_Tool->drawPlanet();}

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

void Render::updateShouldClick(){
    if(Context->showZoom){
        Context->shouldClickOnNames = false;

    }else if(*(Context->isCreating)){
        Context->shouldClickOnNames = false;}

    else if (PlaneteInfo_Tool->changeParametersTool->getMode() != 0){
        Context->shouldClickOnNames = false;
    }
    else if(*(Context->showOptions)){
        Context->shouldClickOnNames = false;
    }
    else if(*(Context->showSettings)){
        Context->shouldClickOnNames = false;
    }
    else if(Context->mouseIsOnMinimap){
        Context->shouldClickOnNames = false;}
        
    else if(PlaneteInfo_Tool->isHovering){
        Context->shouldClickOnNames = false;
    }
    else if(Message_Tool != nullptr){
        Context->shouldClickOnNames = false;
    }
    else if(Context->showQuiz){
        Context->shouldClickOnNames = false;
    }
    else if(UI_Tool->musicPlayerTool->mouseHoveringPlayer()){
        Context->shouldClickOnNames = false;
    }
    else if(Context->showSaveSimulation){
        Context->shouldClickOnNames = false;
    }
    else if(Context->showWelcomeTool){
        Context->shouldClickOnNames = false;
    }
    else if(Context->showSuccessTool){
        Context->shouldClickOnNames = false;
    }
    else{
        Context->shouldClickOnNames = true;
    }





    if(PlaneteInfo_Tool->changeParametersTool->getMode() != 0)
    {Context->isChangingParameters = true;}
    else{Context->isChangingParameters = false;}

    if(!*(Context->showInfo)){
        PlaneteInfo_Tool->isHovering = false;
    }
}
//destructeur
Render::~Render(){  


    if(Background_Tool != nullptr){
        delete Background_Tool;
    }

    if(Axes_Tool != nullptr){
        delete Axes_Tool;
    }

    if(Path_Tool != nullptr){
        delete Path_Tool;
    }

    if(Creator_Manager != nullptr){
        delete Creator_Manager;
    }

    if(Name_Tool != nullptr){
        delete Name_Tool;
    }

    if(PlaneteInfo_Tool != nullptr){
        delete PlaneteInfo_Tool;
    }

    if(CameraOptions_Tool != nullptr){
        delete CameraOptions_Tool;
    }

    if(Options_Tool != nullptr){
        delete Options_Tool;
    }

    if(Settings_Tool != nullptr){
        delete Settings_Tool;
    }

    if(Zoom_Tool != nullptr){
        delete Zoom_Tool;
    }

    if(Orbit_Tool != nullptr){
        delete Orbit_Tool;
    }

    if(Minimap_Tool != nullptr){
        delete Minimap_Tool;
    }

    if(Keys_UI != nullptr){
        delete Keys_UI;
    }

    if(Collision_Tool != nullptr){
        delete Collision_Tool;
    }

    if(Notification_Tool != nullptr){
        delete Notification_Tool;
    }

    if(BackgroundImage_Tool != nullptr){
        delete BackgroundImage_Tool;
    }

    if(Quiz_Tool != nullptr){
        delete Quiz_Tool;
    }

    if(SaveSimulation_Tool != nullptr){
        delete SaveSimulation_Tool;
    }

    if(Restart_Tool != nullptr){
        delete Restart_Tool;
    }

    if(Welcome_Tool != nullptr){
        delete Welcome_Tool;
    }


}

