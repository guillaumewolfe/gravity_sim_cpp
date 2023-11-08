#include "states/SimulationState.h"
#include <sstream>
#include <iomanip>


SimulationState::SimulationState(Game* gameObj) : BaseState(gameObj) {}

void SimulationState::Enter() {
    std::cout << "Entering Simulation State" << std::endl;
    labbels = generateLabbels();

}


std::vector<Labbel*> SimulationState::generateLabbels(){
    std::vector<Labbel*> labbel_list;

    Labbel *TimeLabel = new Labbel(0.5f,0.8f,ImVec4(255,255,255,255),
                                "Simulation time : ",gameObj->getFont("Main Menu"),1.0f);


    labbel_list.push_back(TimeLabel);
    return labbel_list;
}





void SimulationState::Update() {
}
void SimulationState::UpdatePhysics(double dt){
    simulation_time += dt * time_multiplier;
};

void SimulationState::Draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

}


void SimulationState::drawUiElements(){
std::ostringstream stream;
stream << std::fixed << std::setprecision(2) << simulation_time;
std::string rounded_simulation_time_str = stream.str();
std::string newLabelText = "Simulation time : " + rounded_simulation_time_str;
 if (labbels.size() > 0) {
        labbels[0]->UpdateText(newLabelText);  // Update the text of the second label
    }
   for (Labbel *label : labbels) {
       label->Draw();
   }
}



void SimulationState::Exit() {
    std::cout << "Exiting Simulation State" << std::endl;
}

std::string SimulationState::getDescription() {
    return "Simulation State";
}
