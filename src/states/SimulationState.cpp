#include "states/SimulationState.h"


SimulationState::SimulationState(Game* gameObj) : BaseState(gameObj){}

void SimulationState::Enter() {
    std::cout << "Entering Simulation State" << std::endl;

    //Constructions des éléments
    labbels = generateLabbels();
    buttons = generateButtons();


    //Construction des outils pour le render
    renderContext = new RenderContext(&simulation_time, labbels, buttons);
    render = new Render(renderContext);
    render->initTools();
}

//Labels
std::vector<Labbel*> SimulationState::generateLabbels(){
    std::vector<Labbel*> labbel_list;

    Labbel *TimeLabel = new Labbel(0.5f,0.8f,ImVec4(255,255,255,255),
                                "Simulation time : ",gameObj->getFont("Main Menu"),1.0f);


    labbel_list.push_back(TimeLabel);
    return labbel_list;
}
//Butons
std::vector<Button*> SimulationState::generateButtons(){
    std::vector<Button*> buttons_list;
    float diff = 0.057;
   Button *MenuButton = new Button(0.92f, 1-(0.045f), ImVec2(0.12, 0.05),
                               ImVec4(0.5f, 0.5f, 0.7f, 1.0f),
                               ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                               "Menu", gameObj->getFont(), 0.2f,
                               std::bind(&SimulationState::MenuButton, this));    

   Button *RestartButton = new Button(0.92f, 1-(0.045f+diff), ImVec2(0.12, 0.05),
                               ImVec4(0.5f, 0.5f, 0.7f, 1.0f),
                               ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                               "Restart", gameObj->getFont(), 0.2f,
                               std::bind(&SimulationState::Restart, this));   

   Button *PauseButton = new Button(0.92f, 1-(0.045f+2*diff), ImVec2(0.12, 0.05),
                               ImVec4(0.5f, 0.5f, 0.7f, 1.0f),
                               ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                               "Pause", gameObj->getFont(), 0.2f,
                               std::bind(&SimulationState::Pause, this));  


    buttons_list.push_back(MenuButton);
    buttons_list.push_back(RestartButton);
    buttons_list.push_back(PauseButton);
    return buttons_list;
}





void SimulationState::Update() {
}
void SimulationState::UpdatePhysics(double dt){
    if (!isPaused){
        simulation_time += dt * time_multiplier;
    }
};


void SimulationState::Draw() {
    /*glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White background
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    ImGui::PopStyleColor();
    //UI

    drawUiElements();
    ImGui::End(); 
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
*/
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
std::cout << "Exiting Simulation State" << std::endl;
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
}

void SimulationState::MenuButton(){
gameObj->ChangeState(new MenuState(gameObj));
}