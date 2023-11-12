#include "states/MenuState.h"
#include "states/SimulationState.h"

MenuState::MenuState(Game* gameObj) : BaseState(gameObj){
   /*Constructeur :
       1- Construire les bouttons
       2- Initialiser le video*/
  
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
}
std::vector<Labbel*> MenuState::generateLabbels(){
    std::vector<Labbel*> labbel_list;

    Labbel *GameTitle = new Labbel(0.5f,0.2f,ImVec4(255,255,255,255),
                                "Space Querry",gameObj->getFont("Title"),0.8f);
    Labbel *MainMenu= new Labbel(0.5f,0.75f,ImVec4(255,255,255,255),
                                    "Main Menu",gameObj->getFont("Main Menu"),0.8f);

    labbel_list.push_back(GameTitle);
    labbel_list.push_back(MainMenu);
    return labbel_list;
}


std::vector<Button*> MenuState::generateButtons(){
   std::vector<Button*> buttons_list;
    ImVec4 color = ImVec4(53/255, 88/255, 87/255, 150/255);
   Button *exitButton = new Button(0.5f, 0.92f, ImVec2(0.12, 0.05),
                                   ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                                   ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                                   "Exit", gameObj->getFont(), 0.2f,
                                   [this]() {
    auto boundFunction = std::bind(&MenuState::closeButton, this);
    this->generateDialogBox(boundFunction, "Do you want to leave?");});


   Button *StartButton = new Button(0.5f, 0.8f, ImVec2(0.12, 0.05),
                               ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                               ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                               "Start", gameObj->getFont(), 0.2f,
                               [this]() {startButton();});

   Button *OptionButton = new Button(0.5f, 0.86f, ImVec2(0.12, 0.05),
                               ImVec4(30/255+50, 45/255+50, 45/255+50, 1.0f),
                               ImVec4(1.0f, 1.0f, 0.5f, 1.0f),
                               "Options", gameObj->getFont(), 0.2f,
                               [this]() {});


   buttons_list.push_back(exitButton);
   buttons_list.push_back(OptionButton);
   buttons_list.push_back(StartButton);
   return buttons_list;
}








void MenuState::Update() {}


void MenuState::UpdatePhysics(double dt){};


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


   // Video update should be handled here


   // Draw the video texture
   if (videoInitialized) {
       UpdateVideo();
       drawVideo();
   }


   // Set the contrasting background color
    /*int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));*/
   ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);


    ImGui::SetNextWindowFocus();
    drawUiElements();


   ImGui::End();
    //Message Box
    if (messageBox != nullptr) {messageBox->Draw();}
    if (messageBox != nullptr && messageBox->shouldClose){delete messageBox;messageBox = nullptr;}

    //Start or exit with buttons
    if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {startButton();}
    else if(ImGui::IsKeyPressed(ImGuiKey_Escape)) { auto boundFunction = std::bind(&MenuState::closeButton, this);
    this->generateDialogBox(boundFunction, "Do you want to leave?");}
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


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
    gameObj->ChangeState(new SimulationState(gameObj));
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

   static double lastTime = glfwGetTime();
   static double accumulator = 0.0; // Time accumulator for frame updates


   // Get the video's FPS and calculate the time per frame in seconds
   double fps = cap.get(cv::CAP_PROP_FPS);
   double timePerFrame = 1.0 / fps; // Time per frame in seconds


   double currentTime = glfwGetTime();
   double elapsedTime = currentTime - lastTime; // Time elapsed in seconds


   accumulator += elapsedTime; // Add elapsed time to the accumulator


   if (cap.isOpened() && accumulator >= timePerFrame) {
       // If enough time has passed, update the frame
       cv::Mat frame;
       if (cap.read(frame)) {
           // Convert frame from BGR to RGB
           cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);


           // Update texture with the new frame
           glBindTexture(GL_TEXTURE_2D, videoTexture);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
           glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
       } else {
           // If the video is finished, rewind to start
           cap.set(cv::CAP_PROP_POS_FRAMES, 0);
       }
      
       accumulator -= timePerFrame; // Subtract one frame worth of time from the accumulator
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
    Mix_Music *bgMusic = Mix_LoadMUS("../assets/sounds/music_background.mp3");
    if (!bgMusic) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_Quit();
        return;
    }

    // Play the music
    if (Mix_PlayMusic(bgMusic, -1) == -1) {
        std::cerr << "Failed to play background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(bgMusic);
        Mix_Quit();
        return;
    }
}