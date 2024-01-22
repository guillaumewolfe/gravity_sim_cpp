#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/dialogBox.h"
#include "engine/RenderTools/soundTool.h"

DialogBox::DialogBox(RenderContext* renderContext) : RenderComponent(renderContext){
    generate_UI();
}


void DialogBox::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    // Obtenir la taille de la fenêtre

    draw_rect();

    if(isTransitioning){
        transition();
    }else{
        draw_UI();
    }

    //Show mouse screen % when clicked
    if (ImGui::IsMouseClicked(0)) {
        ImVec2 mousePos = ImGui::GetMousePos();
        std::cout << "Mouse position: " << mousePos.x/winWidth << ", " << mousePos.y/winHeight << std::endl;
    }



    ImGui::End();

}

void DialogBox::transition(){
    if (shouldClose) {
        // Pour la fermeture
        if (transitionTimer > 0.0f) {
            transitionTimer -= 0.07f;
        } else {
            isTransitioning = false;
            m_renderContext->showDialogBox = false;
        }
    } else {
        // Pour l'ouverture
        if (transitionTimer < 1.0f) {
            transitionTimer += 0.07f;
        } else {
            isTransitioning = false;
        }
    }
}


void DialogBox::generate_UI(){
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);

    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);

    float taille_x = 0.35;
    float taille_y = 0.15;

    TitleText = new Labbel(0.5,0.0750816,ImVec4(255,255,255,150),
                title,25,0.7f);

    PhraseText = new Labbel(0.5,0.115343,ImVec4(255,255,255,125),
                phrase,20,0.60f);

    closeButton = new ImageButton(playSoundFunc,0.640136, 0.0663765, ImVec2(taille_x*0.1, taille_y*0.1),0.60,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&DialogBox::Close, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);



    OkButton = new Button(playSoundFunc,0.525,0.171926, ImVec2(0.04, 0.03),
                               ImVec4(150.0/255.0, 175.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 175.0/255.0, 150.0/255.0, 1.0f),
                            "Ok", 0.1f,16.0,
                            std::bind(&DialogBox::functionCalled, this),5.0); 
    CancelButton = new Button(playSoundFunc,0.475,0.171926, ImVec2(0.04, 0.03),
                               ImVec4(150.0/255.0, 175.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 175.0/255.0, 150.0/255.0, 1.0f),
                            "Cancel", 0.025f,16.0,
                            std::bind(&DialogBox::Close, this),5.0); 


    posButtonCancel = ImVec2(0.475,0.171926);
    posButtonCancelMiddle = ImVec2(0.5,0.171926);
    posButtonOkRight = ImVec2(0.525,0.171926);


}


void DialogBox::draw_rect(){
    // Existing code for sizes and positions
    longueur = winWidth * 0.3; // Example size
    hauteur = winHeight * 0.15; // Example size

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = 15.0f;

    // Le point central sera utilisé comme référence pour le coin inférieur droit
    centerPos = ImVec2(winWidth * 0.5, winHeight * 0.125f);
    
    float progress;
    if (isTransitioning) {
            progress = transitionTimer; // Pour la fermeture
    } else {
        progress = 1.0; // Aucune transition
    }

    // Calcule la taille actuelle en fonction de la progression de la transition
    float currentHeight = hauteur * progress;
    float currentWidth = longueur * progress;
    
    // Calcule les positions actuelles
    ImVec2 currentTopLeft = ImVec2(centerPos.x - currentWidth / 2, centerPos.y - currentHeight / 2);
    ImVec2 currentBottomRight = ImVec2(centerPos.x + currentWidth / 2, centerPos.y + currentHeight / 2);

    drawList->AddRectFilled(currentTopLeft, 
                            currentBottomRight,
                            IM_COL32(20, 25, 30, 100), // Couleur
                            cornerRadius);

    drawList->AddRect(currentTopLeft, 
                      currentBottomRight,
                      IM_COL32(255, 255, 255, 40), // Couleur
                      cornerRadius, 0, 0.2f);
}

void DialogBox::draw_UI(){
    TitleText->Draw();
    PhraseText->Draw();
    //closeButton->Draw();
    OkButton->Draw();
    CancelButton->Draw();

}

void DialogBox::updateUI(){
    TitleText->UpdateText(title);
    PhraseText->UpdateText(phrase);

    if(button2String == ""){
        CancelButton->hidden = true;
        OkButton->UpdatePosition(posButtonCancelMiddle.x,posButtonCancelMiddle.y);
        OkButton->UpdateText(button1String);
        OkButton->UpdateFunction(std::bind(&DialogBox::Close, this));
        }
    else{
        CancelButton->hidden = false;
        OkButton->UpdatePosition(posButtonOkRight.x,posButtonOkRight.y);
        CancelButton->UpdateText(button2String);
        OkButton->UpdateText(button1String);
        if(dialogFunc != nullptr)
            OkButton->UpdateFunction(std::bind(&DialogBox::functionCalled, this));
        else
            OkButton->UpdateFunction(std::bind(&DialogBox::Close, this));
    }

}

void DialogBox::Open(const std::string& title, const std::string& phrase, const std::string& button1Label, const std::string& button2Label, std::function<void()> customDialogFunc){
    m_renderContext->showDialogBox = true;
    isTransitioning = true;
    transitionTimer = 0.0f;
    shouldClose = false;
    dialogFunc = customDialogFunc;
    this->title = title;
    this->phrase = phrase;
    this->button1String = button1Label;
    this->button2String = button2Label;
    m_renderContext->soundTool->playSound("notification");
    updateUI();
}

void DialogBox::Close(){
    if (m_renderContext->showDialogBox) {
        isTransitioning = true;
        transitionTimer = 1.0f; // Commencez à 1.0 pour la fermeture
        shouldClose = true;
    }
}

void DialogBox::functionCalled(){
    dialogFunc();
    Close();
}
