#include "states/SimulationState.h"


SimulationState::SimulationState(Game* gameObj) : BaseState(gameObj){
    Enter();
}

void SimulationState::Enter() {
    
    //Constructions des éléments
    labbels = generateLabbels();
    buttons = generateButtons();
    changeSimulationSpeed(true);
    imageButtons = generateImageButtons();

    //Construction des outils pour le render
    systemeSolaire = new SystemeSolaire();
    float maxSize = systemeSolaire->maxSize;
    currentCamera = new Camera(Vec3(0.0, 0.0, 10.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0));
    renderContext = new RenderContext(&simulation_time, &time_multiplier, currentCamera, labbels, buttons,imageButtons, &maxSize, &showAxes, systemeSolaire, &currentSpeedIndex, speedSettings, &isCreating, &showInfo, &showCameraOptions, &isLive);
    render = new Render(renderContext);
    physics = new Physics(renderContext);
    currentCamera->setContext(renderContext);
}


//Labels
std::vector<Labbel*> SimulationState::generateLabbels(){
    std::vector<Labbel*> labbel_list;
    float taille_y = 0.06f;
    float topY = 1 - taille_y;
    float buttonCenterY = topY + taille_y / 2; 
    Labbel *TimeLabel = new Labbel(0.29f,buttonCenterY,ImVec4(255,255,255,255),
                                "Simulation time : ",18.0f,0.8f);
    Labbel *TimeMultiplier = new Labbel(0.4f,buttonCenterY,ImVec4(200,200,200,200),
                                "Time speed: x ",14.0f,0.8f);
    Labbel *Speed = new Labbel(0.61f,0.94f,ImVec4(255,255,255,255),
                                "Speed",18.0f,0.8f);
    labbel_list.push_back(TimeLabel);
    labbel_list.push_back(TimeMultiplier);
    //labbel_list.push_back(Speed);
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

   Button *MenuButton = new Button(0.02, 0.02, ImVec2(0.03, 0.03),
                               ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               "Menu", 40.0/255.0,22.0f,
                               [this]() {
    auto boundFunction = std::bind(&SimulationState::MenuButton, this);
    this->generateDialogBox(boundFunction, "Do you want to return to the main menu?");},1);

   Button *RestartButton = new Button(0.40f, 0.95, ImVec2(0.034, 0.0275),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                               "Restart", 0.9f,16.0f,
                            std::bind(&SimulationState::Restart, this),5);   
   Button *PauseButton = new Button(0.35f, 0.95, ImVec2(0.0275, 0.0275),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                               "P", 0.9f,22.0f,
                               std::bind(&SimulationState::Pause, this),5,true);  

    Button *ShowCamera = new Button(0.30, 0.95, ImVec2(0.030, 0.025),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                            "Camera", 0.9f,16.0f,
                            std::bind(&SimulationState::ShowCameraOptionsButton, this),5);  

    Button *CreateObject = new Button(0.36, 0.95, ImVec2(0.025, 0.025),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "Add", 0.9f,16.0f,
                        std::bind(&SimulationState::CreateObjectButton, this),5); 

    Button *increaseSpeed = new Button(0.645f, 0.95, ImVec2(0.025, 0.025),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                            "+", 0.9f,22.0f,
                            std::bind(&SimulationState::changeSimulationSpeed, this,true),0,true); 
    Button *decreaseSpeed = new Button(0.575f, 0.95, ImVec2(0.025, 0.025),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                            "-", 0.9f,22.0f,
                            std::bind(&SimulationState::changeSimulationSpeed, this,false),0,true); 


    return buttons_list;
}
std::vector<ImageButton*> SimulationState::generateImageButtons(){
    std::vector<ImageButton*> imageButtons_list;
    float taille_x = 0.035f;
    float taille_y = 0.06f;
    float position_x_left = (1.0+0.6)/2-taille_x/2;
    float position_x_right = (1.0-0.6)/2+taille_x/2;
    float diffx = taille_x;
    float topY = 1 - taille_y;
    float buttonCenterY = topY + taille_y / 2; 
    ImVec4 button_color = ImVec4(20.0f/255.0f, 25.0f/255.0f, 30.0f/255.0f, 200);
    button_color = ImVec4(1,1,1,1);
    float alpha = 200.0f/255.0f;
    ImageButton *cameraButton = new ImageButton(position_x_left-diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.40,
                        button_color,button_color,
                        "../assets/button/camera.png", 0,
                            std::bind(&SimulationState::ShowCameraOptionsButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);


    ImageButton *addButton = new ImageButton(position_x_left-2*diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/planet.png", 0,
                            std::bind(&SimulationState::CreateObjectButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton *optionButton = new ImageButton(position_x_left, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/settings.png", 0,
                            std::bind(&SimulationState::SettingsButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *minimapButton = new ImageButton(position_x_left-3*diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/minimap.png", 0,
                            std::bind(&SimulationState::MinimapButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton *menuButton = new ImageButton(0.015f, 0.025, ImVec2(0.05, 0.05),0.50,
                        button_color,button_color,
                        "../assets/button/menu.png", 0,
                             [this]() {
    auto boundFunction = std::bind(&SimulationState::MenuButton, this);
    this->generateDialogBox(boundFunction, "Do you want to return to the main menu?");},3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton *pauseButton = new ImageButton(0.5, buttonCenterY, ImVec2(taille_x, taille_y),0.325,
                        button_color,button_color,
                        "../assets/button/pause.png", 0,
                            std::bind(&SimulationState::Pause, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *playButton = new ImageButton(0.5, buttonCenterY, ImVec2(taille_x, taille_y),0.325,
                        button_color,button_color,
                        "../assets/button/play.png", 0,
                            std::bind(&SimulationState::Pause, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    playButton->hidden=true; //Au début, seul le boutton pause est affiché

    ImageButton *forwardButton = new ImageButton(0.5+diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.4,
                        button_color,button_color,
                        "../assets/button/forward.png", 0,
                            std::bind(&SimulationState::changeSimulationSpeed, this,true),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *backwardButton = new ImageButton(0.5-diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.45,
                        button_color,button_color,
                        "../assets/button/backward.png", 0,
                            std::bind(&SimulationState::changeSimulationSpeed, this,false),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *restartButton = new ImageButton(position_x_right, buttonCenterY, ImVec2(taille_x, taille_y),0.45,
                        button_color,button_color,
                        "../assets/button/restart.png", 0,
                            [this]() {
                            auto boundFunction = std::bind(&SimulationState::Restart, this);
                            this->generateDialogBox(boundFunction, "Do you want to restart the simulation?");},
                            3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);


    imageButtons_list.push_back(cameraButton);
    imageButtons_list.push_back(addButton);
    imageButtons_list.push_back(optionButton);
    imageButtons_list.push_back(menuButton);
    imageButtons_list.push_back(pauseButton);
    imageButtons_list.push_back(playButton);
    imageButtons_list.push_back(forwardButton);
    imageButtons_list.push_back(backwardButton);
    imageButtons_list.push_back(restartButton);
    imageButtons_list.push_back(minimapButton);
    return imageButtons_list;

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
    checkButtonState();
    if (isCreating){
        return;}
    else{if(!buttonsActivated)activateButtons();}
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
            //glfwGetWindowSize(window, &width, &height);
            //glfwSetCursorPos(window, width / 2, height / 2);
            }
    }

    if (ImGui::IsKeyDown(ImGuiKey_W)& !currentCamera->followedObject) { currentCamera->moveUp(0.20); }
    if (ImGui::IsKeyDown(ImGuiKey_S)& !currentCamera->followedObject) { currentCamera->moveUp(-0.20); }
    if (ImGui::IsKeyDown(ImGuiKey_D)& !currentCamera->followedObject) { currentCamera->moveRight(0.20); }
    if (ImGui::IsKeyDown(ImGuiKey_A)& !currentCamera->followedObject) { currentCamera->moveRight(-0.20); }
    if (ImGui::IsKeyDown(ImGuiKey_LeftShift)& !currentCamera->followedObject) { currentCamera->moveForward(0.20); }

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

    //Show object info
    if (ImGui::IsKeyReleased(ImGuiKey_I)) {showInfos();}

    if (ImGui::IsKeyDown(ImGuiKey_Z)) {bool in = true;currentCamera->changeValue(in);} 
    if (ImGui::IsKeyDown(ImGuiKey_C)) {bool in = false;currentCamera->changeValue(in);}

    if(isOrbiting){currentCamera->orbitAroundObject(0.0020,0);}
    if (ImGui::IsKeyDown(ImGuiKey_W) && currentCamera->followedObject) {currentCamera->orbitAroundObject(0,0.01);}
    if (ImGui::IsKeyDown(ImGuiKey_S)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(0,-0.01);}
    if (ImGui::IsKeyDown(ImGuiKey_D)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(0.01,0);isOrbiting=false;}
    if (ImGui::IsKeyDown(ImGuiKey_A)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(-0.01,0);isOrbiting=false;}

    /*if ((!ImGui::IsKeyDown(ImGuiKey_W) && !ImGui::IsKeyDown(ImGuiKey_A) &&
            !ImGui::IsKeyDown(ImGuiKey_S) && !ImGui::IsKeyDown(ImGuiKey_D) &&
            !ImGui::IsKeyDown(ImGuiKey_R)) && currentCamera->followedObject) {currentCamera->orbitAroundObject(0.001,0);}*/


    if (ImGui::IsKeyReleased(ImGuiKey_F)) {changeFollowedObject();isOrbiting=true;}

    if (ImGui::IsKeyPressed(ImGuiKey_T)) {resetView();isOrbiting=true;}

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
        forcePause = false;
        imageButtons[4]->hidden=true; //Pause
        imageButtons[5]->hidden=false; //Play
        return;        
    }
    if(isPaused){
        isPaused=false;
        imageButtons[4]->hidden=false; //Pause
        imageButtons[5]->hidden=true; //Play
    }else{
        isPaused=true;
        imageButtons[4]->hidden=true; //Pause
        imageButtons[5]->hidden=false; //Play
    }
}

void SimulationState::Restart(){
    simulation_time = 0;
    currentCamera->resetPosition();
    isLive = true;
    render->UI_Tool->update_time();
    //Time multiplier
    isPaused = false;
    currentSpeedIndex = 0;
    followedObjectIndex = 0;
    time_multiplier = speedSettings[currentSpeedIndex].first;
    systemeSolaire->resetPosition();
    physics->Update(0.001);
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
       buttonsActivated = true;
   }

void SimulationState::deactivateButtons(){
       for (Button *btn : buttons) {
       btn->enabled = false;}
       buttonsActivated = false;
}

void SimulationState::resetButtons(){
    for(ImageButton* button : imageButtons){
        button->isOn=false;
    }
    imageButtons[4]->hidden=false; //Pause
    imageButtons[5]->hidden=true; //Play
}

void SimulationState::checkButtonState(){
    if(imageButtons.size()==0){return;}
    if(isPaused){
        imageButtons[4]->hidden=true; //Pause
        imageButtons[5]->hidden=false; //Play
    }else{
        imageButtons[4]->hidden=false; //Pause
        imageButtons[5]->hidden=true; //Play
    }

    if(isCreating){imageButtons[1]->isOn=true;}
    else{imageButtons[1]->isOn=false;}

    if(showCameraOptions){imageButtons[0]->isOn=true;}
    else{imageButtons[0]->isOn=false;}

    if(showSettings){imageButtons[2]->isOn=true;}
    else{imageButtons[2]->isOn=false;}

    if(showMinimap){imageButtons[9]->isOn=true;}
    else{imageButtons[9]->isOn=false;}
}


void SimulationState::ShowAxesButton(){
    if(showAxes){
        showAxes = false;
    }else{showAxes = true;}
}

void SimulationState::CreateObjectButton(){
    if(isCreating){
        isCreating = false;
        activateButtons();
        Pause();
        imageButtons[1]->isOn=false;
    }else{
        resetView();
        showCameraOptions = false;
        showInfo = false;
        isCreating = true;
        deactivateButtons();
        forcePause = true;
        Pause();
        imageButtons[1]->isOn=true;
    }
}


void SimulationState::changeSimulationSpeed(bool increase) {
    if (increase && currentSpeedIndex < speedSettings.size() - 1) {
        currentSpeedIndex++;
    } else if (!increase && currentSpeedIndex > 0) {
        currentSpeedIndex--;
    }
    isLive=false;

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
    showInfo = true;
    showAxes=false;
    isCreating = false;
    isPaused = false;
    showMinimap = false;
    showSettings = false;
    showCameraOptions = false;
    resetButtons();
    Restart();

}

void SimulationState::showInfos(){
    if(showInfo){showInfo=false;}
    else{showInfo=true;}
}

void SimulationState::ShowCameraOptionsButton(){
    if(showCameraOptions){
        showCameraOptions = false;
        imageButtons[0]->isOn=false;
    }
    else{
        showCameraOptions = true;
        imageButtons[0]->isOn=true;}
}

void SimulationState::SettingsButton(){
    if(showSettings){
        showSettings = false;
        imageButtons[2]->isOn=false;
    }
    else{
        imageButtons[2]->isOn=true;
        showSettings = true;}
}

void SimulationState::MinimapButton(){
    if(showMinimap){
        showMinimap = false;
        imageButtons[9]->isOn=false;
    }
    else{
        imageButtons[9]->isOn=true;
        showMinimap = true;}
}