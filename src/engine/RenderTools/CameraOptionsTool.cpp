#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/CameraOptionsTool.h"


CameraOptionsTool::CameraOptionsTool(RenderContext* renderContext) : RenderComponent(renderContext){
    generate_buttons();
    generate_labels();
    initcameraModeList();
}

void CameraOptionsTool::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
    ImGui::Begin("MessageBox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);


    drawBackground();
    draw_labels();
    draw_buttons();
    update_label();
    ImGui::End(); 

}


void CameraOptionsTool::initcameraModeList(){
    cameraMode[0]="Free Camera";
    cameraMode[1]="Follow Object";
    cameraMode[2] = "Axis focus";
}

void CameraOptionsTool::drawBackground(){

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 2.0f;


    float longueur = winWidth* 0.14; // Exemple de taille
    float hauteur = winHeight * 0.325; // Exemple de taille

    centerPos = ImVec2(winWidth * 0.075f, winHeight * 0.825f);
    topLeft = ImVec2(centerPos.x - longueur * 0.5f, centerPos.y - hauteur * 0.5f);   

    drawList->AddRectFilled(topLeft, 
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                        IM_COL32(7.5, 7.5, 7.5, 255), // Couleur
                        cornerRadius);

    drawList->AddRect(topLeft,
                        ImVec2(topLeft.x + longueur, topLeft.y + hauteur),
                        IM_COL32(50, 50, 50, 150), // Couleur de remplissage
                        cornerRadius,0,1.5f); // Ajustez le rayon ici    
}

void CameraOptionsTool::generate_buttons(){
   Button *ZoomInButton = new Button(0.105f, 0.9, ImVec2(0.02, 0.02),
                                ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                                ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               "+", 0.0f,25.0f,
                               std::bind(&CameraOptionsTool::ZoomIn, this),10.0f, false, ImVec4(255, 255, 255, 240),true); 
    Button *ZoomOutButton = new Button(0.045f, 0.9, ImVec2(0.02, 0.02),
                            ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                            ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                            "-", 0.0f,28.0f,
                            std::bind(&CameraOptionsTool::ZoomOut, this),10.0f, false, ImVec4(255, 255, 255, 240),true); 
    Button *ResetPosButton = new Button(0.075f, 0.95, ImVec2(0.07, 0.02),
                        ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                        ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                        "Reset", 0.0f,15.0f,
                        std::bind(&CameraOptionsTool::resetCamPos, this)); 
    Button *PreviousModeButton = new Button(0.03, 0.775f, ImVec2(0.07, 0.02),
                        ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                        ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                        "<<", 0.0f,13.0f,
                        std::bind(&CameraOptionsTool::previousMode, this)); 
    Button *NextModeButton = new Button(0.12, 0.775f, ImVec2(0.07, 0.02),
                        ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                        ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                        ">>", 0.0f,13.0f,
                        std::bind(&CameraOptionsTool::nextMode, this)); 

    buttons.push_back(ZoomInButton);
    buttons.push_back(ZoomOutButton);
    buttons.push_back(ResetPosButton);
    buttons.push_back(PreviousModeButton);
    buttons.push_back(NextModeButton);
    buttons[2]->UpdateLabelColor(255,255,150,200);                        
}
void CameraOptionsTool::generate_labels(){
    Labbel *CameraLabel = new Labbel(0.075f,0.695f,ImVec4(255,255,255,255),
                            "Camera",23.0f,0.9f);
    Labbel *ZoomLabel = new Labbel(0.075f,0.9f,ImVec4(255,255,255,255),
                        "Zoom",18.0f,0.9f);
    Labbel *ModeStaticLabel = new Labbel(0.075f,0.75f,ImVec4(255,255,255,255),
                        "Mode",18.0f,0.9f);
    Labbel *ModeLabel = new Labbel(0.075f,0.775f,ImVec4(255,255,255,255),
                        "Mode",18.0f,0.9f);
    labbels.push_back(CameraLabel);
    labbels.push_back(ZoomLabel);
    labbels.push_back(ModeStaticLabel);
    labbels.push_back(ModeLabel);
}


void CameraOptionsTool::draw_buttons(){
        for (Button* btn : buttons) {
        btn->Draw();
    }
}

void CameraOptionsTool::draw_labels(){
        for (Labbel* label : labbels) {
        label->Draw();
    }
}

void CameraOptionsTool::update_label(){
    if (cameraMode.find(indexMode) != cameraMode.end()){
         std::string selectedValue = cameraMode[indexMode];
         labbels[3]->UpdateText(selectedValue);
    }
}








void CameraOptionsTool::resetCamPos(){
    indexMode = 0;
    m_renderContext->currentCamera->resetPosition();
}
void CameraOptionsTool::ZoomIn(){
    m_renderContext->currentCamera->zoom(true);
}
void CameraOptionsTool::ZoomOut(){
    m_renderContext->currentCamera->zoom(false);
}

void CameraOptionsTool::previousMode(){
    if (indexMode == 0) {
        indexMode = cameraMode.size() - 1;
    } else {
        indexMode--;
    }    
}
void CameraOptionsTool::nextMode(){
    indexMode = (indexMode + 1) % cameraMode.size();
}