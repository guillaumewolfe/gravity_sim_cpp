#include "states/MenuState.h"
#include "states/SimulationState.h"

MenuState::MenuState(Game* gameObj) : BaseState(gameObj),elapsedTime(0.0){
   /*Constructeur :
       1- Construire les bouttons
       2- Initialiser le video*/
    Enter();
  
}
void MenuState::Enter() {
   //std::cout << "Entering Menu State" << std::endl;
   /* Enter menu state:
       1- Restart video
       2- Restart button states (If needed)*/
  
   //Buttons
   buttons = generateButtons();
   labbels = generateLabbels();
   //Videos & Music

   generateVideo();
   generateMusic();

    settingsTool = new SettingsTool(nullptr);
    settingsTool->setCloseButtonFunction(std::bind(&MenuState::settingsButton, this));
    settingsTool->setSaveButtonFunction(std::bind(&MenuState::settingsButton, this));
    settingsTool->setGameSettings(gameObj->getSettings());
    settingsTool->init();
}


std::vector<Labbel*> MenuState::generateLabbels(){
    std::vector<Labbel*> labbel_list;

    Labbel *GameTitle = new Labbel(0.5f,0.2f,ImVec4(255,255,255,255),
                                "Space Querry",100.0f,0.7f);
    /*Labbel *GameTitle2 = new Labbel(0.501f,0.201f,ImVec4(0,0,0,255),
                            "Space Querry",gameObj->getFont("Title"),0.4f);*/
    Labbel *MainMenu= new Labbel(0.5f,0.75f,ImVec4(255,255,255,255),
                                    "Main Menu",30.0f,0.9f);

    labbel_list.push_back(GameTitle);
    labbel_list.push_back(MainMenu);
    return labbel_list;
}


std::vector<Button*> MenuState::generateButtons(){
   std::vector<Button*> buttons_list;
    ImVec4 color = ImVec4(53/255, 88/255, 87/255, 150/255);
   Button *exitButton = new Button(0.5f, 0.92f, ImVec2(0.08, 0.05),
                                   ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                                   ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                                   "Exit", 0.2f,24.0f,
                                   [this]() {
    auto boundFunction = std::bind(&MenuState::closeButton, this);
    this->generateDialogBox(boundFunction, "Do you want to leave?");},5);


   Button *StartButton = new Button(0.5f, 0.8f, ImVec2(0.08, 0.05),
                               ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                               ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                               "Start", 0.2f,24.0f,
                               [this]() {startButton();},5,"start");

   Button *OptionButton = new Button(0.5f, 0.86f, ImVec2(0.08, 0.05),
                               ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                               ImVec4(1.0f, 1.0f, 0.5f, 1.0f),
                               "Options", 0.2f,24.0f,
                               std::bind(&MenuState::settingsButton, this),5,"restart");


   buttons_list.push_back(exitButton);
   buttons_list.push_back(OptionButton);
   buttons_list.push_back(StartButton);
   return buttons_list;
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
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
   ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);


    ImGui::SetNextWindowFocus();
    drawUiElements();


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
   // Draw your custom buttons
   for (Button *btn : buttons) {
       btn->Draw();
   }
   for (Labbel *label : labbels) {
       label->Draw();
   }
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







////VIDEO////


void MenuState::generateVideo() {
    
    std::string videoPath = "../assets/animations/intro.mp4";
    
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
    bgMusic = Mix_LoadMUS("../assets/sounds/music_background.mp3");
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