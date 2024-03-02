#define GL_SILENCE_DEPRECATION
#include "engine/RenderTools/saveSimulation.h"
#include "path_util.h"
#include "engine/RenderTools/soundTool.h"
#include "save/saveTool.h"

SaveSimulationTool::SaveSimulationTool(RenderContext* renderContext) : RenderComponent(renderContext){
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    longueur = winWidth * 0.20;
    hauteur = winHeight * 0.2;
    topLeft = ImVec2(winWidth * 0.5 - longueur * 0.5, winHeight * 0.5 - hauteur * 0.5);
    center = ImVec2(winWidth * 0.5, winHeight * 0.5);
    initUI();

    saveTool = new SaveTool();
}

SaveSimulationTool::~SaveSimulationTool() {
//Remove memory used

}





void SaveSimulationTool::Draw() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    drawModes();
    //print mouse click % of planetScreenPos
    ImVec2 mousePos = ImGui::GetMousePos();
    //if mouseClicked, print position %
    if (ImGui::IsMouseClicked(0)) {
        std::cout << "Mouse clicked at: " << mousePos.x / winWidth << ", " << mousePos.y / winHeight << std::endl;
    }

    ImGui::End(); 
}

void SaveSimulationTool::initUI() {
    //Labels
    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);

    float taille_x = 0.35;
    float taille_y = 0.15;
    title = new Labbel(0.5f,0.47f,ImVec4(255,255,255,255),
                            "Do you wish to save or load a state?",22.0f,0.9f);

    saveTitleLabbel = new Labbel(0.5f,0.2525,ImVec4(255,255,255,255),
                            "Save current state",30.0f,0.80f);
    loadTitleLabbel = new Labbel(0.5f,0.2525,ImVec4(255,255,255,255),
                            "Load a game state",30.0f,0.80f);
    simulationTimeLabel = new Labbel(0.5f,0.2525,ImVec4(255,255,255,255),
                            "2024-02-02 22:33:44",22.0f,0.80f);
    saveHistorySave = new Labbel(0.631771, 0.290698,ImVec4(255,255,255,255),
                            "Save history",25.0f,0.80f);
    loadingLabbel = new Labbel(0.5,0.5,ImVec4(255,255,255,255),
                            "Loading...",22.0f,0.75f);
    removeLabbel = new Labbel(0.5f,0.475,ImVec4(255,255,255,255),
                            "Do you wish to restart",20.0f,0.80f);
//Buttons
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);

    closeBtn_PositionInitiale = ImVec2(0.45f, 0.55);
    nextBtn_PositionInitiale = ImVec2(0.55f, 0.55);
    float hauteur = 0.54f;
    saveIntroBtn = new Button(playSoundFunc,0.465f, hauteur, ImVec2(0.07, 0.035),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                            "Save current state", 0.1f,16.0f,
                            std::bind(&SaveSimulationTool::changeState, this,1),5); 

    loadIntroBtn = new Button(playSoundFunc,0.535f, hauteur, ImVec2(0.055, 0.035),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                            "Load a state", 0.1f,16.0f,
                            std::bind(&SaveSimulationTool::changeState, this,2),5); 
 
    quitBtn = new ImageButton(playSoundFunc,0.619271, 0.437016, ImVec2(taille_x*0.15, taille_y*0.15),0.60,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&SaveSimulationTool::Close, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    quitSaveBtn = new ImageButton(playSoundFunc,0.757292, 0.225775, ImVec2(taille_x*0.15, taille_y*0.15),0.65,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&SaveSimulationTool::Close, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    arrowUp = new ImageButton(playSoundFunc,0.743229, 0.391473, ImVec2(taille_x*0.25, taille_y*0.25),0.85,
                        button_color,button_color,
                        "../assets/button/ArrowUpNEW.png", 0,
                            std::bind(&SaveSimulationTool::changeScrollOffset, this,-1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    arrowDown = new ImageButton(playSoundFunc, 0.743229, 0.530039, ImVec2(taille_x*0.25, taille_y*0.25),0.85,
                        button_color,button_color,
                        "../assets/button/ArrowDownNEW.png", 0,
                            std::bind(&SaveSimulationTool::changeScrollOffset, this,1),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    removeStateButton = new ImageButton(playSoundFunc,0.743229, 0.225775, ImVec2(taille_x*0.15, taille_y*0.15),0.85,
                        button_color,button_color,
                        "../assets/button/delete.png", 0,
                            std::bind(&SaveSimulationTool::confirmRemove, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);


    saveIcon = new Icon(0.240625, 0.225775,ImVec2(taille_x*0.20, taille_y*0.20),0.65,"../assets/button/save.png",0.7);
    iconIntro = new Icon(0.376563,0.437016,ImVec2(taille_x*0.15, taille_y*0.15),0.60,"../assets/button/warning.png",0.6);
    verticalArrow = new Icon(0.5,0.5,ImVec2(0.04, 0.02),0.60,"../assets/button/verticalArrow.png",0.85);
    weightIcon = new Icon(0.5,0.5,ImVec2(0.04, 0.02),0.65,"../assets/button/weight.png",0.85);
    diameterIcon = new Icon(0.5,0.5,ImVec2(0.04, 0.02),0.60,"../assets/button/diameter.png",1.0);
    notificationIcon = new Icon(0.415104, 0.458333,ImVec2(0.02f,0.02f),0.35f,"../assets/button/warning.png",0.7);


    button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    taille_x = 0.035f*1.0;
    taille_y = 0.06f*1.0;

    float diffx = 0.5-0.365;
    saveSimulationBtn = new Button(playSoundFunc,0.5+diffx, 0.685, ImVec2(taille_x*1.25, taille_y*0.50),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               "Save", 0.2f,18.0f,
                            std::bind(&SaveSimulationTool::saveSimulationButton, this),5);   
    ExitSaveSimulationBtn = new Button(playSoundFunc,0.5, 0.765, ImVec2(taille_x*1.5, taille_y*0.65),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Close", 0.05f,19.0f,
                        std::bind(&SaveSimulationTool::Close, this),5); 
    loadBtn = new Button(playSoundFunc,0.5, 0.765, ImVec2(taille_x*1.5, taille_y*0.65),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                               ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Load", 0.1f,19.0f,
                        std::bind(&SaveSimulationTool::LoadSimulationsButton, this),5); 
    switchStateBtn = new Button(playSoundFunc,0.725521, 0.765, ImVec2(taille_x*1.8, taille_y*0.6),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            "Switch state", 0.1f,14.0f,
                        std::bind(&SaveSimulationTool::SwitchState, this),5); 


    confirmRemoveButton = new Button(playSoundFunc,0.535f, hauteur, ImVec2(0.045, 0.035),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(1.0f, 0.5f, 0.5f, 1.0f),
                            "Remove", 0.1f,16.0f,
                            std::bind(&SaveSimulationTool::removeState, this),5); 
    cancelRemoveButton = new Button(playSoundFunc,0.465f, hauteur, ImVec2(0.045, 0.035),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                            "Cancel", 0.05f,16.0f,
                            std::bind(&SaveSimulationTool::cancelRemove, this),5); 
//Fonts
    float fontSize = 17.0f; // Taille de police
    float selectedFontSize = 19.0f; // Taille de police
    float wrongInputNameFontSize = 13.0f; // Taille de police
    float saveTalbeFontSize = 14.0f; // Taille de police
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    float fontSizeScaled = fontSize * winWidth / 1920;
    float selectedFontSizeScaled = selectedFontSize * winWidth / 1920;
    float wrongInputNameFontSizeScaled = wrongInputNameFontSize * winWidth / 1920;
    float saveTalbeFontSizeScaled = saveTalbeFontSize * winWidth / 1920;
    planetNamesFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaled);
    SelectedplanetNameInSolarSystemFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), selectedFontSizeScaled);
    wrongInputNameFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), wrongInputNameFontSizeScaled);
    saveTableFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), saveTalbeFontSizeScaled);
}



void SaveSimulationTool::Open() {
    currentState = State::Intro;
    setupDimension();
    confirmRemoveState = false;
}

void SaveSimulationTool::Close() {
    m_renderContext->showSaveSimulation = false;
}

void SaveSimulationTool::enterSaveSimulationState(){
    currentState = State::SaveSimulation;
    saveSimulationBtn->isDisabled = true;
    stateIDName = "";
    
    selectedObject = nullptr;
    hoverObject = nullptr;
    //Check if the objects are valids
    gameStates = saveTool->readAllStates();
    gameStatesIds = saveTool->getGameStatesIds();
    currentGameStateSave = nullptr;
    idOffset = 0;
    maxGameStateShown = 5;
    switchStateBtn->updateLabel("Load a state");
}

void SaveSimulationTool::enterLoadSimulationState(){
    currentState = State::LoadSimulation;
    switchStateBtn->updateLabel("Save current state");
    gameStates = saveTool->readAllStates();
    selectedObjectData = nullptr;
    hoverObjectData = nullptr;
    isLoading = false;
    shouldLoad = false;

    if(gameStates.empty()){
        return;
    }
    currentLoadState = gameStates.back();
    gameStatesIds = saveTool->getGameStatesIds();
    currentGameStateSave = nullptr;
    maxGameStateShown = 9;
    //IdOffset should permit to see the last 10 states
    idOffset = gameStates.size() - maxGameStateShown;
    if (idOffset < 0 or gameStates.size() < maxGameStateShown) {
        idOffset = 0;
    }

}

void SaveSimulationTool::saveCurrentState(){
    saveTool->saveGameState(m_renderContext, stateIDName);
    gameStates = saveTool->readAllStates();
    stateIDName = "";
    timeSaveConfirmed = std::chrono::system_clock::now();
}

void SaveSimulationTool::LoadSimulationsButton(){
    if(!isLoading){
        isLoading = true;
    }
}

void SaveSimulationTool::load(){
    if (currentLoadState != nullptr) {
        saveTool->loadGameState(m_renderContext, currentLoadState);
        std::vector<CelestialObject*> objects = currentLoadState->getCelestialObjects();
        m_renderContext->systemeSolaire->isLoadedState = true;
        m_renderContext->systemeSolaire->loadState(objects);
        updateNamesFunction();
        m_renderContext->currentCamera->resetPosition();
        Close();
    }    
}

void SaveSimulationTool::drawModes() {
    switch (currentState) {
        case State::Intro:
            drawIntro();
            break;
        case State::SaveSimulation:
            drawSaveSimulation();
            break;
        case State::LoadSimulation:
            drawLoadSimulation();
            break;
        case State::SaveConfirmed:
            drawSaveConfirmed();
            break;
    }
}

void SaveSimulationTool::changeState(int newState) {
    if (newState == 0) {
        currentState = State::Intro;
    } else if (newState == 1) {
        enterSaveSimulationState();
    } else if (newState == 2) {
        enterLoadSimulationState();
    }
}


void SaveSimulationTool::SwitchState() {
    if (currentState == State::SaveSimulation) {
        enterLoadSimulationState();
    } else if (currentState == State::LoadSimulation) {
        enterSaveSimulationState();
    }
}





//Draw Intro
void SaveSimulationTool::drawIntro() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float longueur = winWidth * 0.275; // Exemple de taille
    float hauteur = winHeight * 0.175; // Exemple de taille
    float cornerRadius = winWidth*0.01;
    topLeft = ImVec2(winWidth * 0.5 - longueur * 0.5, winHeight * 0.5 - hauteur * 0.5);
    ImVec2 bottomRight = ImVec2(winWidth * 0.5 + longueur * 0.5, winHeight * 0.5 + hauteur * 0.5);

    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winHeight*2,winHeight*2), 
                            IM_COL32(0, 0, 0, 200), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0, 0, 0, 250), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30, 200), // Color
                            cornerRadius);
    title->Draw();
    loadIntroBtn->Draw();
    saveIntroBtn->Draw();
    quitBtn->Draw();
    iconIntro->Draw();
}