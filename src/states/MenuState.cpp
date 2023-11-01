#include "states/MenuState.h"


MenuState::MenuState(Game* gameObj) : BaseState(gameObj) {
    /*Constructeur :
        1- Construire les bouttons
        2- Initialiser le video*/
    
}
void MenuState::Enter() {
    std::cout << "Entering Menu State" << std::endl;
    /* Enter menu state:
        1- Restart video
        2- Restart button states (If needed)*/
    buttons = generateButtons();
}


std::vector<Button*> MenuState::generateButtons(){
    std::vector<Button*> buttons_list;
    Button *exitButton = new Button(0.5f, 0.5f, ImVec2(100, 50),
                                    ImVec4(0.5f, 0.5f, 0.7f, 1.0f),
                                    ImVec4(0.7f, 0.5f, 0.5f, 1.0f),
                                    "Exit", gameObj->getFont(), 0.2f,
                                    [this]()
                                    { gameObj->setShouldClose(true);  });
    buttons_list.push_back(exitButton);
    return buttons_list;
}




void MenuState::Update() {}

void MenuState::UpdatePhysics(int dt){};

void MenuState::Draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // You can set your preferred clear color here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set the contrasting background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White background
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    ImGui::PopStyleColor(); // Reset to original style after ImGui::Begin

    // Draw your custom buttons
    for (Button *btn : buttons)
    {
        btn->Draw();
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glDisable(GL_BLEND);
}


void MenuState::Exit() {
    std::cout << "Exiting Menu State" << std::endl;
}

std::string MenuState::getDescription() {
    return "Menu State";
}
