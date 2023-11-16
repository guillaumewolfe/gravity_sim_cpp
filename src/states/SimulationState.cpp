#include "states/SimulationState.h"


SimulationState::SimulationState(Game* gameObj) : BaseState(gameObj){}

void SimulationState::Enter() {
    //std::cout << "Entering Simulation State" << std::endl;

    //Constructions des éléments
    labbels = generateLabbels();
    buttons = generateButtons();

    changeSimulationSpeed(true);

    float maxSize = 200;
    //Construction des outils pour le render
    systemeSolaire = new SystemeSolaire();
    currentCamera = new Camera(Vec3(0.0, 0.0, 10.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0));
    renderContext = new RenderContext(&simulation_time, &time_multiplier, currentCamera, labbels, buttons, &maxSize, &showAxes, systemeSolaire, &currentSpeedIndex, speedSettings);
    render = new Render(renderContext);
    physics = new Physics(renderContext);
    //currentCamera->setPerspective(40.0, 1, 0.5, 300.0);

}

//Labels
std::vector<Labbel*> SimulationState::generateLabbels(){
    std::vector<Labbel*> labbel_list;

    Labbel *TimeLabel = new Labbel(0.5f,0.90f,ImVec4(255,255,255,255),
                                "Simulation time : ",gameObj->getFont("Main Menu"),1.0f);
    Labbel *TimeMultiplier = new Labbel(0.5f,0.94f,ImVec4(255,255,255,255),
                                "Time speed: x ",gameObj->getFont("Main Menu"),1.0f);

    labbel_list.push_back(TimeLabel);
    labbel_list.push_back(TimeMultiplier);
    return labbel_list;
}
//Butons
std::vector<Button*> SimulationState::generateButtons(){
    std::vector<Button*> buttons_list;
    float diff = 0.057;
    ImVec4 button_color = ImVec4(0.5f, 0.5f, 0.7f, 1.0f);

   Button *MenuButton = new Button(0.08f, 1-(0.045f), ImVec2(0.12, 0.05),
                               button_color,
                               ImVec4(1.0f, 0.1f, 0.1f, 1.0f),
                               "Menu", gameObj->getFont(), 0.25f,
                               [this]() {
    auto boundFunction = std::bind(&SimulationState::MenuButton, this);
    this->generateDialogBox(boundFunction, "Do you want to return to the main menu?");});

   Button *RestartButton = new Button(0.08f, 1-(0.045f+diff), ImVec2(0.12, 0.05),
                               button_color,
                               ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                               "Restart", gameObj->getFont(), 0.25f,
                               std::bind(&SimulationState::Restart, this));   

   Button *PauseButton = new Button(0.08f, 1-(0.045f+2*diff), ImVec2(0.12, 0.05),
                               button_color,
                               ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                               "Pause", gameObj->getFont(), 0.25f,
                               std::bind(&SimulationState::Pause, this));  

    Button *ShowAxes = new Button(0.08f, 1-(0.045f+3*diff), ImVec2(0.12, 0.05),
                            button_color,
                            ImVec4(0.5f, 0.7f, 1.0f, 1.0f),
                            "Show Axes", gameObj->getFont(), 0.25f,
                            std::bind(&SimulationState::ShowAxesButton, this));  
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
    buttons_list.push_back(increaseSpeed);
    buttons_list.push_back(decreaseSpeed);
    return buttons_list;
}





void SimulationState::Update() {
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {Pause();}
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {auto boundFunction = std::bind(&SimulationState::MenuButton, this);
    this->generateDialogBox(boundFunction, "Do you want to return to the main menu?");}
    

    if (ImGui::IsKeyDown(ImGuiKey_E)) {bool in = true;currentCamera->zoom(in);} 
    if (ImGui::IsKeyDown(ImGuiKey_Q)) {bool in = false;currentCamera->zoom(in);}

    if (ImGui::IsKeyDown(ImGuiKey_A)) {currentCamera->rotateHorizontal(0.5);}
    if (ImGui::IsKeyDown(ImGuiKey_D)) {currentCamera->rotateHorizontal(-0.5);}
    if (ImGui::IsKeyDown(ImGuiKey_W)) {currentCamera->rotateVertical(0.5);}
    if (ImGui::IsKeyDown(ImGuiKey_S)) {currentCamera->rotateVertical(-0.5);}

    if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) {currentCamera->moveForward(1);}
    if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) {currentCamera->moveForward(-1);}
    if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) {currentCamera->moveRight(1);}
    if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) {currentCamera->moveRight(-1);}

    if (ImGui::IsKeyDown(ImGuiKey_I)) {currentCamera->orbitAroundObject(0,0.01);}
    if (ImGui::IsKeyDown(ImGuiKey_K)) {currentCamera->orbitAroundObject(0,-0.01);}
    if (ImGui::IsKeyDown(ImGuiKey_L)) {currentCamera->orbitAroundObject(0.01,0);}
    if (ImGui::IsKeyDown(ImGuiKey_J)) {currentCamera->orbitAroundObject(-0.01,0);}




    if (ImGui::IsKeyPressed(ImGuiKey_F)) {changeFollowedObject();}

    if (ImGui::IsKeyDown(ImGuiKey_R)) {resetView();}

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
    render->initCamera();

    //Time multiplier
    currentSpeedIndex = 0;
    followedObjectIndex = 0;
    time_multiplier = speedSettings[currentSpeedIndex].first;
    systemeSolaire->resetPosition();
}

void SimulationState::MenuButton(){
    gameObj->ChangeState(new MenuState(gameObj));
}

void SimulationState::generateDialogBox(std::function<void()> func, const std::string& message){
    forcePause = true;
    Pause();
    render->Message_Tool = new MessageTool(message, func);
}

void SimulationState::activateButtons(){
       for (Button *btn : buttons) {
       btn->enabled = true;
   }}

void SimulationState::deactivateButtons(){
       for (Button *btn : buttons) {
       btn->enabled = false;
   }
}

void SimulationState::ShowAxesButton(){
    if(showAxes){
        showAxes = false;
    }else{showAxes = true;}
}


void SimulationState::changeSimulationSpeed(bool increase) {
    if (increase && currentSpeedIndex < speedSettings.size() - 1) {
        currentSpeedIndex++;
    } else if (!increase && currentSpeedIndex > 0) {
        currentSpeedIndex--;
    }

    time_multiplier = speedSettings[currentSpeedIndex].first;
    std::string speedLabel = speedSettings[currentSpeedIndex].second;

    // Vous pouvez utiliser speedLabel ici pour afficher ou logger
}


void SimulationState::changeFollowedObject(){
        currentCamera->followObject(systemeSolaire->objects[followedObjectIndex]);
        followedObjectIndex+=1;
        if (followedObjectIndex > systemeSolaire->objects.size()-1){followedObjectIndex=0;}
}


void SimulationState::resetView(){
    followedObjectIndex = 0;
    currentCamera->resetPosition();
}

