#include "states/SimulationState.h"
#include "path_util.h"
#include "save/saveTool.h"
#include "engine/RenderTools/successTool.h"
#include "engine/RenderTools/steamTool.h"

SimulationState::SimulationState(Game* gameObj) : BaseState(gameObj){
    Enter();
}
void SimulationState::Enter() {
    //Constructions des éléments
    soundTool = gameObj->getSoundTool();
    labbels = generateLabbels();
    buttons = generateButtons();
    changeSimulationSpeed(true);
    imageButtons = generateImageButtons();

    //Construction des outils pour le render
    systemeSolaire = new SystemeSolaire();
    float maxSize = systemeSolaire->maxSize;
    currentCamera = new Camera(Vec3(0.0, 0.0, 10.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0));
    renderContext = new RenderContext(&simulation_time, &time_multiplier, currentCamera, labbels, buttons,imageButtons, &maxSize, &showAxes, systemeSolaire, &currentSpeedIndex, speedSettings, &isCreating, &showInfo, &showCameraOptions, &isLive, &showOptions, &showSettings, &isOrbiting, &isPaused, soundTool);
    render = new Render(renderContext);
    physics = new Physics(renderContext);
    currentCamera->setContext(renderContext);
    //Bind function to toolss
    render->Options_Tool->setMenuButtonFunction(std::bind(&SimulationState::MenuButton, this));
    render->Options_Tool->setSettingsButtonFunction(std::bind(&SimulationState::SettingsButton, this));
    render->Options_Tool->setResumeButtonFunction(std::bind(&SimulationState::OptionsButton, this));
    render->Options_Tool->setAboutButtonFunction(std::bind(&SimulationState::OpenWelcomeTool, this));

    render->Settings_Tool->setCloseButtonFunction(std::bind(&SimulationState::SettingsButton, this));
    render->Settings_Tool->setSaveButtonFunction(std::bind(&SimulationState::SettingsButton, this));
    render->Settings_Tool->setGameSettings(gameObj->getSettings());
    render->Settings_Tool->setDefaultSettings(gameObj->getDefaultSettings());
    render->Settings_Tool->init();

    currentCamera->zoomSensitiviy = &(gameObj->getSettings()->movementSpeed);
    currentCamera->rotationSensitivity = &(gameObj->getSettings()->rotationSpeed);
    render->Zoom_Tool->zoomCamera->zoomSensitiviy = &(gameObj->getSettings()->movementSpeed);
    render->Zoom_Tool->zoomCamera->rotationSensitivity = &(gameObj->getSettings()->rotationSpeed);
    render->Zoom_Tool->escapeClicked = &isShowZoomClose;



    render->Zoom_Tool->setCloseButtonFonction(std::bind(&SimulationState::TelescopeButton, this));

    render->CameraOptions_Tool->setCloseButtonFunction(std::bind(&SimulationState::ShowCameraOptionsButton, this));

    render->Minimap_Tool->setCloseButtonFunction(std::bind(&SimulationState::MinimapButton, this));


    render->Creator_Manager->setUpdateNamesFunction(std::bind(&NameTool::initLabbels, render->Name_Tool));

    physics->setCollisionFunction(std::bind(&CollisionTool::newCollision, render->Collision_Tool, std::placeholders::_1, std::placeholders::_2));
    physics->setEndCollisionFunction(std::bind(&CollisionTool::endCollision, render->Collision_Tool, std::placeholders::_1, std::placeholders::_2));
    systemeSolaire->setContext(renderContext);
    render->PlaneteInfo_Tool->changeParametersTool->setupdateNamesFunction(std::bind(&NameTool::synchronizeLabels, render->Name_Tool));
    render->PlaneteInfo_Tool->changeParametersTool->setCloseMinimapFunction(std::bind(&SimulationState::MinimapButton, this));
    render->PlaneteInfo_Tool->changeParametersTool->setEditPositionFunction(std::bind(&CreatorManager::editObject, render->Creator_Manager, std::placeholders::_1));
    render->PlaneteInfo_Tool->setConfirmBoxFunction(std::bind(&SimulationState::generateDialogBox, this, std::placeholders::_1, std::placeholders::_2));
    
    render->Quiz_Tool->setConfirmBoxFunction(std::bind(&SimulationState::generateDialogBox, this, std::placeholders::_1, std::placeholders::_2));

    render->UI_Tool->musicPlayerTool->setVolumeButtonFunction(std::bind(&SimulationState::SettingsButton, this));
    render->UI_Tool->showFPS = &(gameObj->getSettings()->showFPS);
    render->SaveSimulation_Tool->updateNamesFunction = std::bind(&NameTool::initLabbels, render->Name_Tool);

    render->Restart_Tool->setRestartFunction(std::bind(&SimulationState::Restart, this, std::placeholders::_1));

    render->Welcome_Tool->shouldDraw = &(renderContext->showWelcomeTool);
    renderContext->successTool->setNotificationTool(render->Notification_Tool);

}

//destructeur

SimulationState::~SimulationState() {
}
//Labels
std::vector<Labbel*> SimulationState::generateLabbels(){
    std::vector<Labbel*> labbel_list;
    float taille_y = 0.06f;
    float topY = 1 - taille_y;
    float buttonCenterY = topY + taille_y / 2; 
    Labbel *TimeLabel = new Labbel(0.33f,buttonCenterY,ImVec4(255,255,255,255),
                                "Simulation time : ",18.0f,0.8f);
    Labbel *TimeMultiplier = new Labbel(0.585f,buttonCenterY,ImVec4(200,200,200,200),
                                "Time speed: x ",16.0f,0.8f);
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
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, soundTool, std::placeholders::_1);

   Button *MenuButton = new Button(playSoundFunc,0.02, 0.02, ImVec2(0.03, 0.03),
                               ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               ImVec4(0.0f, 0.0f, 0.0f, 1.0f),
                               "Menu", 40.0/255.0,22.0f,
                               [this]() {
    auto boundFunction = std::bind(&SimulationState::MenuButton, this);
    this->generateDialogBox(boundFunction, "Do you want to return to the main menu?");},1);
   
   Button *PauseButton = new Button(playSoundFunc,0.35f, 0.95, ImVec2(0.0275, 0.0275),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                               "P", 0.9f,22.0f,
                               std::bind(&SimulationState::Pause, this),5);  

    Button *ShowCamera = new Button(playSoundFunc,0.30, 0.95, ImVec2(0.030, 0.025),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                            "Camera", 0.9f,16.0f,
                            std::bind(&SimulationState::ShowCameraOptionsButton, this),5);  

    Button *CreateObject = new Button(playSoundFunc,0.36, 0.95, ImVec2(0.025, 0.025),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "Add", 0.9f,16.0f,
                        std::bind(&SimulationState::CreateObjectButton, this),5); 

    Button *increaseSpeed = new Button(playSoundFunc,0.645f, 0.95, ImVec2(0.025, 0.025),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                            "+", 0.9f,22.0f,
                            std::bind(&SimulationState::changeSimulationSpeed, this,true),0); 
    Button *decreaseSpeed = new Button(playSoundFunc,0.575f, 0.95, ImVec2(0.025, 0.025),
                        ImVec4(0.075f, 0.075f, 0.12f, 0.0f),
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                            "-", 0.9f,22.0f,
                            std::bind(&SimulationState::changeSimulationSpeed, this,false),0); 


    return buttons_list;
}
std::vector<ImageButton*> SimulationState::generateImageButtons(){
    std::function<void(std::string)> playSoundFunc=
    std::bind(&SoundTool::playSound, soundTool, std::placeholders::_1);


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
    ImageButton *cameraButton = new ImageButton(playSoundFunc,position_x_left-diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/camera.png", 0,
                            std::bind(&SimulationState::ShowCameraOptionsButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);


    ImageButton *addButton = new ImageButton(playSoundFunc,position_x_left-2*diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/planet.png", 0,
                            std::bind(&SimulationState::CreateObjectButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    
    ImageButton *SuccessTool = new ImageButton(playSoundFunc,position_x_left+diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/trophy.png", 0,
                            std::bind(&SimulationState::SuccessToolButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *quizButton = new ImageButton(playSoundFunc,position_x_left, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/quiz.png", 0,
                            std::bind(&SimulationState::QuizButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *minimap = new ImageButton(playSoundFunc,position_x_left-4*diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/solarsystem.png", 0,
                            std::bind(&SimulationState::MinimapButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *telescopeButton = new ImageButton(playSoundFunc,position_x_left-3*diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/compare.png", 0,
                            std::bind(&SimulationState::TelescopeButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *menuButton = new ImageButton(playSoundFunc,0.015f, 0.025, ImVec2(0.05, 0.05),0.50,
                        button_color,button_color,
                        "../assets/button/menu.png", 0,
                        std::bind(&SimulationState::OptionsButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *saveButton = new ImageButton(playSoundFunc,position_x_right-diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.50,
                        button_color,button_color,
                        "../assets/button/save.png", 0,
                        std::bind(&SimulationState::SaveButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    ImageButton *pauseButton = new ImageButton(playSoundFunc,0.5, buttonCenterY, ImVec2(taille_x, taille_y),0.325,
                        button_color,button_color,
                        "../assets/button/pause.png", 0,
                            std::bind(&SimulationState::Pause, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *playButton = new ImageButton(playSoundFunc,0.5, buttonCenterY, ImVec2(taille_x, taille_y),0.325,
                        button_color,button_color,
                        "../assets/button/play.png", 0,
                            std::bind(&SimulationState::Pause, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    playButton->hidden=true; //Au début, seul le boutton pause est affiché

    ImageButton *forwardButton = new ImageButton(playSoundFunc,0.5+diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.4,
                        button_color,button_color,
                        "../assets/button/forward2.png", 0,
                            std::bind(&SimulationState::changeSimulationSpeed, this,true),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *backwardButton = new ImageButton(playSoundFunc,0.5-diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.45,
                        button_color,button_color,
                        "../assets/button/backward2.png", 0,
                            std::bind(&SimulationState::changeSimulationSpeed, this,false),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *restartButton = new ImageButton(playSoundFunc,position_x_right+diffx, buttonCenterY, ImVec2(taille_x, taille_y),0.45,
                        button_color,button_color,
                        "../assets/button/restart.png", 0,
                        std::bind(&SimulationState::RestartButton, this),
                            3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *controls = new ImageButton(playSoundFunc,0.055f, 0.025, ImVec2(0.05, 0.05),0.50,
                        button_color,button_color,
                        "../assets/button/controls.png", 0,
                        std::bind(&SimulationState::showControlsButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    ImageButton *resetView = new ImageButton(playSoundFunc,position_x_right, buttonCenterY, ImVec2(taille_x, taille_y),0.4,
                        button_color,button_color,
                        "../assets/button/resetPosition.png", 0,
                        std::bind(&SimulationState::resetView, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    imageButtons_list.push_back(cameraButton);//0
    imageButtons_list.push_back(addButton);//1
    imageButtons_list.push_back(SuccessTool);//2
    imageButtons_list.push_back(menuButton); //3
    imageButtons_list.push_back(pauseButton);   //4
    imageButtons_list.push_back(playButton);   //5
    imageButtons_list.push_back(forwardButton); //6
    imageButtons_list.push_back(backwardButton); //7
    imageButtons_list.push_back(restartButton); //8
    imageButtons_list.push_back(telescopeButton); //9
    imageButtons_list.push_back(minimap); //10
    imageButtons_list.push_back(controls); //11
    imageButtons_list.push_back(resetView); //12
    imageButtons_list.push_back(quizButton); //13
    imageButtons_list.push_back(saveButton); //14
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

void SimulationState::setButtonActive(bool active, ImageButton* buttonExeption){
    for(auto imageButton : imageButtons){
        if(imageButton != buttonExeption){
        imageButton->enabled = active;
        }
    }
    if(!active){
        buttonsAreDeactivated = true;
    }else{
        buttonsAreDeactivated = false;
        buttonExeption = nullptr;
    }

}

void SimulationState::Update() {
    
    checkButtonState();
    if (isCreating || renderContext->showZoom || render->PlaneteInfo_Tool->changeParametersTool->getMode()!=0 || renderContext->showQuiz || renderContext->showSaveSimulation ||  renderContext->showRestartTool || showSettings || render->UI_Tool->isSearching || renderContext->showWelcomeTool || renderContext->showSuccessTool) {
        if(!buttonsAreDeactivated)setButtonActive(false, buttonExeption);
        return;}
    else if(buttonsAreDeactivated){
        setButtonActive(true);
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {Pause();}
    if (ImGui::IsKeyReleased(ImGuiKey_Escape)) {
        if(!isShowZoomClose){isShowZoomClose = true;}
        else{OptionsButton();}}
    

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
    if (ImGui::IsKeyDown(ImGuiKey_E)) {
        bool in = true;
        currentCamera->zoomByDistance(in);}
    if (ImGui::IsKeyDown(ImGuiKey_Q)) {
        bool in = false;
        currentCamera->zoomByDistance(in);}


    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {changeSimulationSpeed(false);} 
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {changeSimulationSpeed(true);}
    if (ImGui::IsKeyPressed(ImGuiKey_V)) {ShowAxesButton();}
    //Show object info
    if (ImGui::IsKeyReleased(ImGuiKey_I)) {showInfos();}

    if (ImGui::IsKeyDown(ImGuiKey_Z)) {bool in = true;currentCamera->changeValue(in);} 
    if (ImGui::IsKeyDown(ImGuiKey_C)) {bool in = false;currentCamera->changeValue(in);}

    if (ImGui::IsKeyDown(ImGuiKey_W) && currentCamera->followedObject) {currentCamera->orbitAroundObject(0,0.01);}
    if (ImGui::IsKeyDown(ImGuiKey_S)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(0,-0.01);}
    if (ImGui::IsKeyDown(ImGuiKey_D)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(0.01,0);isOrbiting=false;}
    if (ImGui::IsKeyDown(ImGuiKey_A)&& currentCamera->followedObject) {currentCamera->orbitAroundObject(-0.01,0);isOrbiting=false;}
    if (ImGui::IsKeyPressed(ImGuiKey_LeftShift)) {render->UI_Tool->isSearching = true;}
    /*if ((!ImGui::IsKeyDown(ImGuiKey_W) && !ImGui::IsKeyDown(ImGuiKey_A) &&
            !ImGui::IsKeyDown(ImGuiKey_S) && !ImGui::IsKeyDown(ImGuiKey_D) &&
            !ImGui::IsKeyDown(ImGuiKey_R)) && currentCamera->followedObject) {currentCamera->orbitAroundObject(0.001,0);}*/

        //ImGuiRightClick
    if (ImGui::IsKeyReleased(ImGuiKey_F) or ImGui::IsMouseClicked(1)) {changeGlobalFollowing();isOrbiting=true;}


    if (ImGui::IsKeyPressed(ImGuiKey_R)) {resetView();isOrbiting=true;}
    if (ImGui::IsKeyPressed(ImGuiKey_Tab)) {changeFollowedObject();}
    if (ImGui::IsKeyPressed(ImGuiKey_C)) {ShowCameraOptionsButton();}
    if (ImGui::IsKeyPressed(ImGuiKey_M)){MinimapButton();}
    if (ImGui::IsKeyPressed(ImGuiKey_L)){showDialogBox();}
    if (ImGui::IsKeyPressed(ImGuiKey_Z)){
        if(renderContext->showQuiz == false){
            renderContext->showQuiz = true;
            render->Quiz_Tool->Open();
            if(renderContext->showNotificationTool){
                showDialogBox();}
        }else{
            renderContext->showQuiz = false;
            render->Quiz_Tool->Close();

        }}
    //Mouse dragging for mooving the camera
    if(render->PlaneteInfo_Tool->changeParametersTool->getMode()!=0){return;}
    if(render->PlaneteInfo_Tool->isHovering){return;}
    if(showCameraOptions && render->CameraOptions_Tool->mouseOnCameraOptions()){return;}
    if(showOptions){return;}
    if(showSettings){return;}
    if(isCreating){return;}
    if(renderContext->showZoom){return;}
    if(renderContext->showSaveSimulation){return;}
    if(render->UI_Tool->musicPlayerTool->mouseHoveringPlayer()){return;}
    if(render->UI_Tool->mouseHoveringUI()){return;}
    if (ImGui::IsMouseDragging(0, 0.0f)) {
        //Set cursor to resize
        ImVec2 delta = ImGui::GetMouseDragDelta(0, 0.0f);
        currentCamera->orbitAroundObject(-delta.x*0.005, delta.y*0.005);
        if(delta.x!=0){
            isOrbiting=false;
        }
        ImGui::ResetMouseDragDelta(0);
    }
}
void SimulationState::UpdatePhysics(double dt){
    if (!isPaused){
        simulation_time += dt * time_multiplier;
        physics->Update(dt* time_multiplier);
    }
};


void SimulationState::Draw() {
    ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiKey_Tab, ImGuiKeyOwner_None);
ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Tab, ImGuiKeyOwner_None);
    render->Draw();
    renderContext->steamTool->Draw();

    if (gameObj->getSettings() && gameObj->getSettings()->volumeChanged) {
        int sdlVolume = static_cast<int>(gameObj->getSettings()->musicVolume * gameObj->getSettings()->mainVolume * 128);
        Mix_VolumeMusic(sdlVolume);
        gameObj->getSettings()->volumeChanged = false; // Réinitialisez le drapeau
    }
        if (!musicStarted) {
        Mix_PlayMusic(bgMusic, -1);
        musicStarted = true;
    }

    if(simulation_time>1 && showFirstNotification){
        showDialogBox();
        showFirstNotification=false;
    }
}


void SimulationState::generateMusic(){
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return;
    }

    // Load music
    bgMusic = Mix_LoadMUS(getFullPath("../assets/sounds/simulationMusic.mp3").c_str());
    if (!bgMusic) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return;
    }

    int sdlVolume = static_cast<int>(gameObj->getSettings()->musicVolume*gameObj->getSettings()->mainVolume * 128);
    Mix_VolumeMusic(sdlVolume);

}



void SimulationState::Exit() {

    imageButtons.clear(); // Clear the vector after deleting the buttons
    for (Button* button : buttons) {
        delete button;
    }
    buttons.clear(); // Clear the vector after deleting the buttons

    for (Labbel* label : labbels) {
        delete label;
    }
    labbels.clear(); // Clear the vector after deleting the labels


    //detruit les outils    
    delete render;
    delete physics;
    delete systemeSolaire;
    delete currentCamera;
    delete renderContext;
    Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
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

void SimulationState::Restart(bool defaultState){
    simulation_time = 0;
    showAxes=false;
    isCreating = false;
    showOptions = false;
    renderContext->showZoom = false;
    isPaused = false;
    showMinimap = false;
    showSettings = false;
    showCameraOptions = false;
    renderContext->showControls = true;
    renderContext->showNotificationTool = false;
    systemeSolaire->reset(defaultState);
    currentCamera->resetPosition();
    render->Name_Tool->initLabbels();
    render->Collision_Tool->reset();
    isLive = defaultState;
    render->UI_Tool->update_time();
    //Time multiplier
    isPaused = false;
    currentSpeedIndex = 0;
    followedObjectIndex = 0;
    renderContext->showZoom = false;
    time_multiplier = speedSettings[currentSpeedIndex].first;
    physics->Update(0.001);
    buttonExeption = nullptr;
}

void SimulationState::MenuButton(){
    render->UI_Tool->musicPlayerTool->firstUpdate = true;
    OptionsButton();
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
        button->turnOff();
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

    if(isCreating){imageButtons[1]->turnOn();}
    else{imageButtons[1]->turnOff();}



    if(showCameraOptions){imageButtons[0]->turnOn();}
    else{imageButtons[0]->turnOff();}

    if(renderContext->showSuccessTool){imageButtons[2]->turnOn();}
    else{imageButtons[2]->turnOff();}

    //Pause for live
    if(isPaused){isLive=false;}

    //Show Zoom
    if(showOptions || showSettings || isCreating){renderContext->showZoom = false;}
    
    if(renderContext->showZoom){imageButtons[9]->turnOn();}
    else{imageButtons[9]->turnOff();}

    if(renderContext->showMinimap){imageButtons[10]->turnOn();}
    else{imageButtons[10]->turnOff();}

    if(renderContext->showControls){imageButtons[11]->hidden=true;}
    else{imageButtons[11]->hidden=false;}

    if(isCreating && renderContext->showControls){
        showControlsButton();
    }

    if(renderContext->showQuiz){imageButtons[13]->turnOn();}
    else{imageButtons[13]->turnOff();}

    if(renderContext->showSaveSimulation){imageButtons[14]->turnOn();}
    else{imageButtons[14]->turnOff();}

    //Si moins que 2 objects, deactivate button
    if(systemeSolaire->objects.size()<2){
        imageButtons[9]->enabled=false;
    }

    if(renderContext->showSuccessTool){
        buttonExeption = imageButtons[2];
    }
}


void SimulationState::ShowAxesButton(){
    if(showAxes){
        showAxes = false;
    }else{showAxes = true;}
}

void SimulationState::CreateObjectButton(){
    if(isCreating){
        render->Creator_Manager->Exit();
        Pause();
        if(wasMiniMapShown){MinimapButton();}
        if(wasControlsShown){showControlsButton();}
    }else{
        wasMiniMapShown = renderContext->showMinimap;
        wasControlsShown = renderContext->showControls;
        if(showOptions){OptionsButton();}
        if(renderContext->showMinimap){MinimapButton();}
        if(renderContext->showControls){showControlsButton();}
        if(renderContext->showZoom){TelescopeButton();}
        if(showInfo){showInfos();}
        if(renderContext->showNotificationTool){render->Notification_Tool->Close();}
        if(showCameraOptions){ShowCameraOptionsButton();}
        resetView();
        isCreating = true;
        forcePause = true;
        Pause();
        buttonExeption = imageButtons[1];
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
        if(currentCamera->followedObject == nullptr){return;}
        if(systemeSolaire->objects.empty()){return;}
        if(currentCamera->firstPersonModeEnabled){currentCamera->firstPersonModeEnabled=false;}
        if(currentCamera->isGlobalFollowing){
            currentCamera->newFollowObjectGlobal(systemeSolaire->objects[followedObjectIndex]); 
        }
        else{currentCamera->newFollowObject(systemeSolaire->objects[followedObjectIndex]);}
        currentCamera->selectedObject = systemeSolaire->objects[followedObjectIndex];
        showInfo = true;
        isOrbiting=true;
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
    showOptions = false;
    renderContext->showZoom = false;
    isPaused = false;
    showMinimap = false;
    showSettings = false;
    showCameraOptions = false;
    renderContext->showNotificationTool = false;

    resetButtons();
    Restart();
    //render->UI_Tool->musicPlayerTool->play();

}

void SimulationState::showInfos(){
    if(showInfo){showInfo=false;}
    else{showInfo=true;}
}

void SimulationState::ShowCameraOptionsButton(){
    if(showCameraOptions){
        if(!render->CameraOptions_Tool->isClosed){render->CameraOptions_Tool->Close();return;}
        else{showCameraOptions = false;}
        imageButtons[0]->isOn=false;
    }
    else{
        showCameraOptions = true;
        imageButtons[0]->isOn=true;
        render->CameraOptions_Tool->Open();
        }
}

void SimulationState::SettingsButton(){
    if(showSettings){
        showSettings = false;
        imageButtons[2]->isOn=false;
        render->Settings_Tool->setPlaneteInfoToolOpen();
    }
    else{
        if(showOptions){OptionsButton();}
        if(renderContext->showZoom){TelescopeButton();}
        if(isCreating){CreateObjectButton();}
        render->Settings_Tool->Open();
        imageButtons[2]->isOn=true;
        showSettings = true;
        buttonExeption = imageButtons[2];
        }
}

void SimulationState::MinimapButton(){
    if(renderContext->showMinimap){
        renderContext->showMinimap = false;
        imageButtons[10]->isOn=false;
        renderContext->mouseIsOnMinimap = false;
    }
    else{
        if(isCreating){CreateObjectButton();}
        //if(showInfo){showInfos();}
        if(render->PlaneteInfo_Tool->changeParametersTool->getMode()!=0){
            render->PlaneteInfo_Tool->changeParametersTool->setMode(0,nullptr);
        }
        imageButtons[10]->isOn=true;
        renderContext->showMinimap = true;
        render->Minimap_Tool->Open();
        }
}

void SimulationState::OptionsButton(){
    if(showOptions){
        showOptions = false;
        Pause();
    }
    else{
        if(showSettings){render->Settings_Tool->CloseButton();}
        if(renderContext->showZoom){OptionsButton();}
        if(isCreating){CreateObjectButton();}
        if(showInfo){showInfos();}
        showOptions = true;
        forcePause = true;
        Pause();
        }
}

void SimulationState::TelescopeButton(){
    //Comparing Tool
   if(renderContext->showZoom){
       renderContext->showZoom = false;
       imageButtons[9]->isOn=false;
   }
   else{
    //Si moins que 2 objets, return
    if(systemeSolaire->objects.size()<2){return;}
       if(showCameraOptions){ShowCameraOptionsButton();}
       if(showOptions){OptionsButton();}
       if(isCreating){CreateObjectButton();}
       if(showInfo){showInfos();}
       if(renderContext->showControls){showControlsButton();}
       if(renderContext->showNotificationTool){render->Notification_Tool->Close();}
       if(renderContext->showMinimap){MinimapButton();}
       render->Zoom_Tool->Open();
       renderContext->showZoom = true;
       imageButtons[9]->isOn=true;
       isShowZoomClose = false;
       forcePause = true;
       Pause();
       }
}



void SimulationState::changeGlobalFollowing(){
    //Change between follow object and global isGlobalFollowing
    if(currentCamera->followedObject == nullptr){return;}
    currentCamera->resetOrbits();
    if(currentCamera->isGlobalFollowing){
        currentCamera->newFollowObject(currentCamera->followedObject);
        showInfo = true;
    }else{
        currentCamera->newFollowObjectGlobal(currentCamera->followedObject);
        showInfo = false;
    }
}

void SimulationState::showControlsButton(){
    if(!renderContext->showControls){
        renderContext->showControls = true;
    }else{
        renderContext->showControls = false;
    }
}

void SimulationState::showDialogBox(){
    if(renderContext->showNotificationTool){
        render->Notification_Tool->Close();
    }else{
        //render->Notification_Tool->Open("Notification","I have created a minimap for you!","Open","Close",std::bind(&SimulationState::MinimapButton, this));
    }
}

void SimulationState::QuizButton(){
    if(renderContext->showQuiz){
        renderContext->showQuiz = false;
        render->Quiz_Tool->Close();
    }else{
        forcePause = true;
        Pause();
        renderContext->showQuiz = true;
        render->Quiz_Tool->Open();
        if(renderContext->showNotificationTool){
            showDialogBox();
        }
        if(renderContext->showZoom){TelescopeButton();}
        if(isCreating){CreateObjectButton();}
        if(showCameraOptions){ShowCameraOptionsButton();}
        if(renderContext->showMinimap){MinimapButton();}
        buttonExeption = imageButtons[13];
    }
}

void SimulationState::SaveButton(){
    //SaveTool saveTool;
    //saveTool.saveGameState(renderContext);
    if(renderContext->showSaveSimulation == true){
        renderContext->showSaveSimulation = false;
        render->SaveSimulation_Tool->Close();
    }else{
        forcePause = true;
        Pause();
        renderContext->showSaveSimulation = true;
        render->SaveSimulation_Tool->Open();
        buttonExeption = imageButtons[14];
    }
}

void SimulationState::RestartButton(){
    if(renderContext->showRestartTool){
        renderContext->showRestartTool = false;
        render->Restart_Tool->Close();
    }else{
        forcePause = true;
        Pause();
        renderContext->showRestartTool = true;
        render->Restart_Tool->Open();
        buttonExeption = imageButtons[8];
    }
}

void SimulationState::OpenWelcomeTool(){
    if(renderContext->showWelcomeTool){
        renderContext->showWelcomeTool = false;
        render->Welcome_Tool->Close();
    }else{
        forcePause = true;
        Pause();
        renderContext->showWelcomeTool = true;
        render->Welcome_Tool->Open();
    }
}

void SimulationState::SuccessToolButton(){
    if(renderContext->showSuccessTool){
        renderContext->showSuccessTool = false;
        renderContext->successTool->Close();
    }else{
        buttonExeption = imageButtons[2];
        forcePause = true;
        Pause();
        renderContext->showSuccessTool = true;
        renderContext->successTool->Open();
    }

}