#include "states/MenuState.h"
#include "states/SimulationState.h"
#include "engine/RenderTools/soundTool.h"
#include "path_util.h"
#include "engine/RenderTools/welcomeTool.h"

MenuState::MenuState(Game* gameObj) : BaseState(gameObj),elapsedTime(0.0){
   /*Constructeur :
       1- Construire les bouttons
       2- Initialiser le video*/
    Enter();
    messageBox = nullptr;
    drawWelcomeMessage = gameObj->getSettings()->firstLaunch;
}


void MenuState::Enter() {
   //std::cout << "Entering Menu State" << std::endl;
   /* Enter menu state:
       1- Restart video
       2- Restart button states (If needed)*/
    soundTool = gameObj->getSoundTool();
   //Buttons
   buttons = generateButtons();
   labbels = generateLabbels();
   
   //Videos & Music

   generateVideo();
   generateMusic();

    welcomeTool = new WelcomeTool();
    welcomeTool->shouldDraw = &drawWelcomeMessage;
    welcomeTool->Open();

    settingsTool = new SettingsTool(nullptr);
    settingsTool->setCloseButtonFunction(std::bind(&MenuState::settingsButton, this));
    settingsTool->setSaveButtonFunction(std::bind(&MenuState::settingsButton, this));
    settingsTool->setGameSettings(gameObj->getSettings());
    settingsTool->setDefaultSettings(gameObj->getDefaultSettings());
    settingsTool->init();
}


std::vector<Labbel*> MenuState::generateLabbels(){
    std::vector<Labbel*> labbel_list;

    Labbel *GameTitle = new Labbel(0.5f,0.2f,ImVec4(255,255,255,255),
                                "Space Query",120.0f,0.95f);
    Labbel *GameTitle2 = new Labbel(0.5015f,0.2f,ImVec4(255,102,255,255),
                                "Space Query",120.0f,0.6f);
    Labbel *GameTitle3 = new Labbel(0.4985f,0.2f,ImVec4(0,255,255,255),
                                "Space Query",120.0f,0.6f);
    /*Labbel *GameTitle2 = new Labbel(0.501f,0.201f,ImVec4(0,0,0,255),
                            "Space Querry",gameObj->getFont("Title"),0.4f);*/
    Labbel *MainMenu= new Labbel(0.5f,0.75f,ImVec4(255,255,255,255),
                                    "Main Menu",25.0f,0.9f);

    float badgeFontScale = 110.0f;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    menuFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/zing.otf").c_str(), badgeFontScale*winWidth / 1920);

    //labbel_list.push_back(GameTitle3);
    //labbel_list.push_back(GameTitle2);
    //labbel_list.push_back(GameTitle);
    labbel_list.push_back(MainMenu);
    return labbel_list;
}


std::vector<Button*> MenuState::generateButtons(){
   std::vector<Button*> buttons_list;
    ImVec4 color = ImVec4(53/255, 88/255, 87/255, 150/255);
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, soundTool, std::placeholders::_1);

   Button *exitButton = new Button(playSoundFunc,0.5f, 0.92f, ImVec2(0.08, 0.05),
                                   ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                                   ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                                   "Exit", 0.2f,24.0f,
                                   [this]() {
    auto boundFunction = std::bind(&MenuState::closeButton, this);
    this->generateDialogBox(boundFunction, "Do you want to leave?");},5);


   Button *StartButton = new Button(playSoundFunc,0.5f, 0.8f, ImVec2(0.08, 0.05),
                               ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                               ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                               "Start", 0.2f,24.0f,
                               [this]() {startButton();},5,"start");

   Button *OptionButton = new Button(playSoundFunc,0.5f, 0.86f, ImVec2(0.08, 0.05),
                               ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                               ImVec4(1.0f, 1.0f, 0.5f, 1.0f),
                               "Options", 0.2f,24.0f,
                               std::bind(&MenuState::settingsButton, this),5,"restart");

   Button *ControlsButton = new Button(playSoundFunc,0.035f, 0.975f, ImVec2(0.055, 0.035),
                               ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                               ImVec4(1.0f, 1.0f, 0.5f, 1.0f),
                               "About", 0.2f,18.0f,
                               std::bind(&MenuState::showWelcome, this),5,"restart");

   buttons_list.push_back(exitButton);
   buttons_list.push_back(OptionButton);
   buttons_list.push_back(StartButton);
    buttons_list.push_back(ControlsButton);
   return buttons_list;
}






void MenuState::showWelcome(){
    drawWelcomeMessage = true;
    welcomeTool->Open();
}

void MenuState::Update() {
    if (fadeOpacity > 0.0f) {
        fadeOpacity -= 0.01f; // Ajustez ce taux selon la vitesse de fondu souhaitée
    }    
    if (!musicStarted) {
        Mix_PlayMusic(bgMusic, -1);
        musicStarted = true;
    }



}


void MenuState::UpdatePhysics(double dt){
};


void MenuState::generateDialogBox(std::function<void()> func, const std::string& message){
     messageBox = new MessageTool(message, func);
}



void MenuState::Draw() {
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


    if (fadeOpacity > 0.0f) {
        // Obtenez la taille de la fenêtre
        ImVec2 windowSize = ImGui::GetIO().DisplaySize;
        // Définissez la couleur de fondu avec l'opacité actuelle
        ImVec4 fadeColor = ImVec4(0.0f, 0.0f, 0.0f, fadeOpacity);
        // Dessinez le rectangle de fondu
        ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(0, 0), windowSize, ImGui::GetColorU32(fadeColor));
    }


   // Draw the video texture
   if (videoInitialized) {
       UpdateVideo();
       drawVideo();
   }


   // Set the contrasting background color
glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
ImGui::SetNextWindowPos(ImVec2(0, 0));
ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiKey_Tab, ImGuiKeyOwner_None);
ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Tab, ImGuiKeyOwner_None);
    ImGui::SetNextWindowFocus();
    drawUiElements();
    if(drawWelcomeMessage)drawWelcome();

   ImGui::End();
    //Message Box
    if (messageBox != nullptr) {
        if(showSettings){settingsTool->CloseButton();}
        messageBox->Draw();}
    if (messageBox != nullptr && messageBox->shouldClose){delete messageBox;messageBox = nullptr;}
    if(showSettings){settingsTool->Draw();}
    //Start or exit with buttons
    if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {startButton();}
    if(ImGui::IsKeyPressed(ImGuiKey_Escape) && !showSettings) { auto boundFunction = std::bind(&MenuState::closeButton, this);this->generateDialogBox(boundFunction, "Do you want to leave?");}
    if(ImGui::IsKeyPressed(ImGuiKey_Escape) && showSettings){settingsTool->CloseButton();}
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (gameObj->getSettings() && gameObj->getSettings()->volumeChanged) {
        int sdlVolume = static_cast<int>(gameObj->getSettings()->musicVolume * gameObj->getSettings()->mainVolume * 128);
        Mix_VolumeMusic(sdlVolume);
        gameObj->getSettings()->volumeChanged = false; // Réinitialisez le drapeau
    }
   // Disable blending
   glDisable(GL_BLEND);
}

void MenuState::drawUiElements(){
    if(drawWelcomeMessage)return;
   // Draw your custom buttons
   for (Button *btn : buttons) {
       btn->Draw();
   }
   for (Labbel *label : labbels) {
       label->Draw();
   }

   //Draw Menu text

/*    Labbel *GameTitle = new Labbel(0.5f,0.2f,ImVec4(255,255,255,255),
                                "Space Query",120.0f,0.95f);
    Labbel *GameTitle2 = new Labbel(0.5015f,0.2f,ImVec4(255,102,255,255),
                                "Space Query",120.0f,0.6f);
    Labbel *GameTitle3 = new Labbel(0.4985f,0.2f,ImVec4(0,255,255,255),
                                "Space Query",120.0f,0.6f);*/

    ImVec2 centerPosText = ImVec2(winWidth*0.5, winHeight*0.2);
    ImVec2 centerPosText2 = ImVec2(winWidth*0.5015, winHeight*0.2);
    ImVec2 centerPosText3 = ImVec2(winWidth*0.4985, winHeight*0.2);
    ImGui::PushFont(menuFont);
    ImVec2 textSize = ImGui::CalcTextSize("Space Query", NULL, false, 0.0f);
    ImVec2 textPos = ImVec2(centerPosText.x - textSize.x * 0.5f, centerPosText.y - textSize.y * 0.5f);
    ImVec2 textPos2 = ImVec2(centerPosText2.x - textSize.x * 0.5f, centerPosText2.y - textSize.y * 0.5f);
    ImVec2 textPos3 = ImVec2(centerPosText3.x - textSize.x * 0.5f, centerPosText3.y - textSize.y * 0.5f);


    //Drawlst: 
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    //Text1 = bleu
    drawList->AddText(menuFont, menuFont->FontSize, textPos3, IM_COL32(0, 255, 255, 0.6f*255), "Space Query");
    //Text2 = rose
    drawList->AddText(menuFont, menuFont->FontSize, textPos2, IM_COL32(255, 102, 255, 0.6f*255), "Space Query");
    //Text3 = blanc
    drawList->AddText(menuFont, menuFont->FontSize, textPos, IM_COL32(255, 255, 255, 0.95f*255), "Space Query");

    ImGui::PopFont();
}


void MenuState::Exit() {
 
   if (cap.isOpened()) {
       cap.release();
   }

   //std::cout << "Exiting Menu State" << std::endl;
    if (Mix_PlayingMusic() == 1) {
        Mix_HaltMusic();
    }

    // Free the music
    if (bgMusic != nullptr) {
        Mix_FreeMusic(bgMusic);
        bgMusic = nullptr; // Set to nullptr to indicate it's been freed
    }

    // Delete buttons and labels
    for (Button* button : buttons) {
        delete button;
    }
    buttons.clear(); // Clear the vector after deleting the buttons

    for (Labbel* label : labbels) {
        delete label;
    }
    labbels.clear(); // Clear the vector after deleting the labels

    // Close SDL_mixer
    Mix_CloseAudio();
    Mix_Quit();

    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


std::string MenuState::getDescription() {
   return "Menu State";
}

void MenuState::closeButton(){
    gameObj->setShouldClose(true);
}

void MenuState::startButton(){
    std::string newstate = "simulation";
    fadeOpacity = 1.0f;
    gameObj->ChangeState(newstate);
    if (musicStarted) {
        Mix_HaltMusic();
        musicStarted = false;
    }
}


void MenuState::RestartState(){
    accumulator = 0.0;
    lastTime = glfwGetTime();
}

void MenuState::drawWelcome(){
    welcomeTool->Draw();
}






////VIDEO////


void MenuState::generateVideo() {
    //Set counter to 0
    lastTime = glfwGetTime();
    accumulator = 0.0;
    
    std::string videoPath = getFullPath("/assets/animations/intro.mp4");
    
    cap.open(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file." << std::endl;
        return;
    }
    // Only generate the texture if it hasn't been generated before
    if (videoTexture == 0) {
        glGenTextures(1, &videoTexture);
        glBindTexture(GL_TEXTURE_2D, videoTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Create a black dummy image
        unsigned char blackImage[3] = {0, 0, 0};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, blackImage);

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
    }

    videoInitialized = true;
}



void MenuState::UpdateVideo(){
   double currentTime = glfwGetTime();
   double elapsedTime = currentTime - lastTime; // Time elapsed since last update

   accumulator += elapsedTime; // Add elapsed time to the accumulator

   // Get the video's FPS and calculate the time per frame in seconds
   double fps = cap.get(cv::CAP_PROP_FPS);
   double timePerFrame = 1.0 / fps;

   if (cap.isOpened() && accumulator >= timePerFrame) {
       cv::Mat frame;
       if (cap.read(frame)) {
           cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
           glBindTexture(GL_TEXTURE_2D, videoTexture);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
           glBindTexture(GL_TEXTURE_2D, 0);
       } else {
           cap.set(cv::CAP_PROP_POS_FRAMES, 0); // Rewind if the video is finished
       }
       accumulator -= timePerFrame; // Subtract one frame worth of time
   }

   lastTime = currentTime; // Update the last time
}



void MenuState::drawVideo() {

    ImVec2 windowSize = ImGui::GetIO().DisplaySize;
    float windowAspectRatio = windowSize.x / windowSize.y;
    float videoAspectRatio = cap.get(cv::CAP_PROP_FRAME_WIDTH) / (float)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    ImVec2 imageSize;
    // Determine scaling factor based on which dimension needs to be filled
    float scaleWidth = windowSize.x / (float)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    float scaleHeight = windowSize.y / (float)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    float scale = std::max(scaleWidth, scaleHeight); // Choose the larger scale factor
    scale *= 1.05f;
    // Apply scaling factor to both width and height
    imageSize.x = cap.get(cv::CAP_PROP_FRAME_WIDTH) * scale;
    imageSize.y = cap.get(cv::CAP_PROP_FRAME_HEIGHT) * scale;


    // Calculate the position to ensure the video is centered after being cropped
    ImVec2 imagePos = ImVec2((windowSize.x - imageSize.x) * 0.5f, (windowSize.y - imageSize.y) * 0.5f);
    ImGui::SetNextWindowPos(imagePos);
    ImGui::SetNextWindowSize(imageSize);
    ImGui::Begin("VideoBackground", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs);
    ImGui::Image((void*)(intptr_t)videoTexture, imageSize);
    ImGui::End();
}


void MenuState::generateMusic(){
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return;
    }

    // Load music
    
    bgMusic = Mix_LoadMUS(getFullPath("../assets/music/particles.mp3").c_str());
    if (!bgMusic) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return;
    }

    int sdlVolume = static_cast<int>(gameObj->getSettings()->musicVolume*gameObj->getSettings()->mainVolume * 128);
    Mix_VolumeMusic(sdlVolume);

}

void MenuState::settingsButton(){
    if(showSettings){
        showSettings = false;
    }
    else{
        settingsTool->Open();
        showSettings = true;
    }
}