#include "states/LoadingState.h"
#include "game/game.h"

LoadingState::LoadingState(Game* gameObj) : BaseState(gameObj) {
    Enter();
}

void LoadingState::Enter() {
    // Créez un contexte ImGui distinct pour la fenêtre secondair
}

void LoadingState::Update() {
}

void LoadingState::UpdatePhysics(double dt){};

void LoadingState::Draw() {
   // Clear the buffer
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Enable Alpha for buttons
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // Buttons settings
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

    drawList = ImGui::GetWindowDrawList();

    drawLoadingrectangle();


    ImGui::End();
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


   // Disable blending
   glDisable(GL_BLEND);

    if (progress<1.00){
        progress += 0.001;
        std::cout << progress << std::endl;
    }
    else{
        gameObj->ChangeState("menu");
    }
}

void LoadingState::drawLoadingrectangle(){
    float longueur = 0.225*winWidth;
    float hauteur = 0.01*winHeight;

    ImVec2 centerPos = ImVec2(winWidth * 0.5f, winHeight * 0.85f);
    ImVec2 topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);
    float cornerRadius = 40.0f;

    float longueurProgress = progress*longueur;

    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueurProgress, topLeft.y + hauteur), 
                            IM_COL32(255,255,255,255), // Couleur
                            cornerRadius);

//Draw border rectangle to loading bar, but a bit bigger with a space between
    ImVec2 space = ImVec2(winWidth*0.0075,winWidth*0.0075);
    ImVec2 topLeftBorder = ImVec2(topLeft.x - space.x, topLeft.y - space.y);
    drawList->AddRect(topLeftBorder, 
                        ImVec2(topLeft.x + longueur+space.x, topLeft.y + hauteur+space.y), 
                        IM_COL32(255,255,255,255), // Couleur
                        cornerRadius,0,winWidth*0.0025); 

    
}





void LoadingState::Exit() {
    std::cout << "Exiting Loading State" << std::endl;
}

std::string LoadingState::getDescription() {
    return "Loading State";
}
void LoadingState::RestartState(){}