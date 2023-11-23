#include "states/SimulationState.h"


SimulationState::SimulationState(Game* gameObj) : BaseState(gameObj){
    Enter();
}

void SimulationState::Enter() {

    //Constructions des éléments
    labbels = generateLabbels();
    buttons = generateButtons();

    changeSimulationSpeed(true);

    //Construction des outils pour le render
    systemeSolaire = new SystemeSolaire();
    float maxSize = systemeSolaire->maxSize;
    currentCamera = new Camera(Vec3(0.0, 0.0, 10.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0));
    renderContext = new RenderContext(&simulation_time, &time_multiplier, currentCamera, labbels, buttons, &maxSize, &showAxes, systemeSolaire, &currentSpeedIndex, speedSettings, &isCreating);
    render = new Render(renderContext);
    physics = new Physics(renderContext);

}

//Labels
std::vector<Labbel*> SimulationState::generateLabbels(){
    std::vector<Labbel*> labbel_list;

    Labbel *TimeLabel = new Labbel(0.5f,0.90f,ImVec4(255,255,255,255),
                                "Simulation time : ",30.0f,1.0f);
    Labbel *TimeMultiplier = new Labbel(0.5f,0.94f,ImVec4(255,255,255,255),
                                "Time speed: x ",30.0f,1.0f);

    labbel_list.push_back(TimeLabel);
    labbel_list.push_back(TimeMultiplier);
    return labbel_list;
}
//Butons
std::vector<Button*> SimulationState::generateButtons(){
    std::vector<Button*> buttons_list;
    float diff = 0.050;
    float position_x = 0.046f;
    float taille_x = 0.075f;
    float taille_y = 0.045f;
    ImVec4 button_color = ImVec4(0.5f, 0.5f, 0.7f, 1.0f);

   Button *MenuButton = new Button(position_x, 1-(0.045f), ImVec2(taille_x, taille_y),
                               button_color,
                               ImVec4(1.0f, 0.1f, 0.1f, 1.0f),
                               "Menu", gameObj->getFont(), 0.25f,
                               [this]() {
    auto boundFunction = std::bind(&SimulationState::MenuButton, this);
    this->generateDialogBox(boundFunction, "Do you want to return to the main menu?");});

   Button *RestartButton = new Button(position_x, 1-(0.045f+diff), ImVec2(taille_x, taille_y),
                               button_color,
                               ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                               "Restart", gameObj->getFont(), 0.25f,
                               std::bind(&SimulationState::Restart, this));   

   Button *PauseButton = new Button(position_x, 1-(0.045f+2*diff), ImVec2(taille_x, taille_y),
                               button_color,
                               ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                               "Pause", gameObj->getFont(), 0.25f,
                               std::bind(&SimulationState::Pause, this));  

    Button *ShowAxes = new Button(position_x, 1-(0.045f+3*diff), ImVec2(taille_x, taille_y),
                            button_color,
                            ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                            "Show Axes", gameObj->getFont(), 0.25f,
                            std::bind(&SimulationState::ShowAxesButton, this));  
    Button *CreateObject = new Button(position_x, 1-(0.045f+4*diff), ImVec2(taille_x, taille_y),
                        button_color,
                        ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                        "Add object", gameObj->getFont(), 0.25f,
                        std::bind(&SimulationState::CreateObjectButton, this));  
    Button *increaseSpeed = new Button(0.61f, 1-(0.060f), ImVec2(0.02, 0.03),
                            button_color,
                            ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                            "+", gameObj->getFont(), 0.25f,
                            std::bind(&SimulationState::changeSimulationSpeed, this,true)); 
    Button *decreaseSpeed = new Button(0.39f, 1-(0.060f), ImVec2(0.02, 0.03),
                            button_color,
                            ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                            "-", gameObj->getFont(), 0.25f,
                            std::bind(&SimulationState::changeSimulationSpeed, this,false)); 


    buttons_list.push_back(MenuButton);
    buttons_list.push_back(RestartButton);
    buttons_list.push_back(PauseButton);
    buttons_list.push_back(ShowAxes);
    buttons_list.push_back(CreateObject);
    buttons_list.push_back(increaseSpeed);
    buttons_list.push_back(decreaseSpeed);
    return buttons_list;
}



void SimulationState::rotateCamWithMouse(){
        float speed = 0.025; // Ajustez ce paramètre selon la sensibilité souhaitée
    if (firstMouse) {
        deactivateButtons();
        lastMousePos = ImGui::GetMousePos();
        firstMouse = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        ImVec2 currentMousePos = ImGui::GetMousePos();
        ImVec2 mouseDelta = ImVec2(currentMousePos.x - lastMousePos.x, currentMousePos.y - lastMousePos.y);
        lastMousePos = currentMousePos;

        currentCamera->rotateHorizontal(-mouseDelta.x * speed);
        currentCamera->rotateVertical(-mouseDelta.y * speed);
    }
}

void SimulationState::Update() {
    if (isCreating){return;}
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {Pause();}
    if (ImGui::IsKeyPressed(ImGuiKey_Escape) && render->Message_Tool == nullptr) {auto boundFunction = std::bind(&SimulationState::MenuButton, this);
    this->generateDialogBox(boundFunction, "Do you want to return to the main menu?");}

    

    static ImVec2 lastMousePos = ImVec2(0, 0);

    if (!ImGui::IsKeyDown(ImGuiKey_W) && !ImGui::IsKeyDown(ImGuiKey_A) &&
        !ImGui::IsKeyDown(ImGuiKey_S) && !ImGui::IsKeyDown(ImGuiKey_D) &&
        !ImGui::IsKeyDown(ImGuiKey_R)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if(!firstMouse){
            activateButtons();
            firstMouse = true;
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            glfwSetCursorPos(window, width / 2, height / 2);
            }
    }

    if (ImGui::IsKeyDown(ImGuiKey_W)& !currentCamera->followedObject) { currentCamera->moveForward(0.25); }
    if (ImGui::IsKeyDown(ImGuiKey_S)& !currentCamera->followedObject) { currentCamera->moveForward(-0.25); }
    if (ImGui::IsKeyDown(ImGuiKey_D)& !currentCamera->followedObject) { currentCamera->moveRight(0.20); }
    if (ImGui::IsKeyDown(ImGuiKey_A)& !currentCamera->followedObject) { currentCamera->moveRight(-0.20); }

    // Gestion de la rotation
    if ((ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_S) ||
        ImGui::IsKeyDown(ImGuiKey_D) || ImGui::IsKeyDown(ImGuiKey_A) ||
        ImGui::IsKeyDown(ImGuiKey_R))& !currentCamera->followedObject) {
        isTranslating=true;
        rotateCamWithMouse();
    }
    
    if (ImGui::IsKeyDown(ImGuiKey_E)) {bool in = true;currentCamera->zoom(in);} 
    if (ImGui::IsKeyDown(ImGuiKey_Q)) {bool in = false;currentCamera->zoom(in);}

    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {changeSimulationSpeed(false);} 
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {changeSimulationSpeed(true);}


    if (ImGui::IsKeyDown(ImGuiKey_W) && currentCamera->followedObject) {currentCamera->orbitAroundObject(0,0.01);}
    if (ImGui::IsKeyDown(ImGuiKey_S)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(0,-0.01);}
    if (ImGui::IsKeyDown(ImGuiKey_D)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(0.01,0);}
    if (ImGui::IsKeyDown(ImGuiKey_A)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(-0.01,0);}

    /*if ((!ImGui::IsKeyDown(ImGuiKey_W) && !ImGui::IsKeyDown(ImGuiKey_A) &&
            !ImGui::IsKeyDown(ImGuiKey_S) && !ImGui::IsKeyDown(ImGuiKey_D) &&
            !ImGui::IsKeyDown(ImGuiKey_R)) && currentCamera->followedObject) {currentCamera->orbitAroundObject(0.001,0);}*/


    if (ImGui::IsKeyPressed(ImGuiKey_F)) {changeFollowedObject();}

    if (ImGui::IsKeyDown(ImGuiKey_T)) {resetView();}

}
void SimulationState::UpdatePhysics(double dt){
    if (!isPaused){
        simulation_time += dt * time_multiplier;
        physics->Update(dt* time_multiplier);
    }
};


void SimulationState::Draw() {
    render->Draw();
}

void SimulationState::drawUiElements(){
//Update time
std::ostringstream stream;
stream << std::fixed << std::setprecision(2) << simulation_time;
std::string rounded_simulation_time_str = stream.str();
std::string newLabelText = "Simulation time : " + rounded_simulation_time_str;
 if (labbels.size() > 0) {
        labbels[0]->UpdateText(newLabelText);  // Update the text of the second label
    }


//Draw Labels
   for (Labbel *label : labbels) {
       label->Draw();
   }

//Draw Buttons
   for (Button *btn : buttons) {
       btn->Draw();
   }
}



void SimulationState::Exit() {
//std::cout << "Exiting Simulation State" << std::endl;
    for (Button* button : buttons) {
        delete button;
    }
    buttons.clear(); // Clear the vector after deleting the buttons

    for (Labbel* label : labbels) {
        delete label;
    }
    labbels.clear(); // Clear the vector after deleting the labels

}

std::string SimulationState::getDescription() {
    return "Simulation State";
}




//Buttons :

void SimulationState::Pause(){
    if (forcePause){
        isPaused=true;
        buttons[2]->updateLabel("Resume");
        forcePause = false;
        return;        
    }
    if(isPaused){
        isPaused=false;
        buttons[2]->updateLabel("Pause");
    }else{
        isPaused=true;
        buttons[2]->updateLabel("Resume");
    }
}

void SimulationState::Restart(){
    simulation_time = 0;
    currentCamera->resetPosition();

    //Time multiplier
    currentSpeedIndex = 0;
    followedObjectIndex = 0;
    time_multiplier = speedSettings[currentSpeedIndex].first;
    systemeSolaire->resetPosition();
}

void SimulationState::MenuButton(){
    std::string newstate = "menu";
    gameObj->ChangeState(newstate);
}

void SimulationState::generateDialogBox(std::function<void()> func, const std::string& message){
    forcePause = true;
    Pause();
    render->Message_Tool = new MessageTool(message, func);
}

void SimulationState::activateButtons(){
       for (Button *btn : buttons) {
       btn->enabled = true;}
   }

void SimulationState::deactivateButtons(){
       for (Button *btn : buttons) {
       btn->enabled = false;}
}

void SimulationState::ShowAxesButton(){
    if(showAxes){
        showAxes = false;
    }else{showAxes = true;}
}

void SimulationState::CreateObjectButton(){
    if(isCreating){
        isCreating = false;
        render->Creator_Tool->End();
        buttons[4]->updateLabel("Add object");
        buttons[4]->changeColor(ImVec4(0.5f, 0.5f, 0.7f, 1.0f));
    }else{
        isCreating = true;
        buttons[4]->updateLabel("Close");
        buttons[4]->changeColor(ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
    }
}


void SimulationState::changeSimulationSpeed(bool increase) {
    if (increase && currentSpeedIndex < speedSettings.size() - 1) {
        currentSpeedIndex++;
    } else if (!increase && currentSpeedIndex > 0) {
        currentSpeedIndex--;
    }

    time_multiplier = speedSettings[currentSpeedIndex].first;
    std::string speedLabel = speedSettings[currentSpeedIndex].second;

}


void SimulationState::changeFollowedObject(){
        currentCamera->newFollowObject(systemeSolaire->objects[followedObjectIndex]);
        followedObjectIndex+=1;
        if (followedObjectIndex > systemeSolaire->objects.size()-1){followedObjectIndex=0;}
}


void SimulationState::resetView(){
    followedObjectIndex = 0;
    currentCamera->resetPosition();
}

void SimulationState::RestartState(){
    render->Message_Tool = nullptr;
    Restart();

}