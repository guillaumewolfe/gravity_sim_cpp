#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/MusicPlayerTool.h"
#include <SDL_mixer.h>
#include "engine/RenderTools/soundTool.h"
#include <iostream>
#include <sstream> // For std::ostringstream
#include <iomanip> // For std::setw and std::setfill
#include "path_util.h"
#include <random> // Pour std::random_device et std::mt19937
#include "opencv2/opencv.hpp"


MusicPlayerTool::MusicPlayerTool(RenderContext* renderContext) : RenderComponent(renderContext){
    initSongs();
    initUI();


    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    centerPos = ImVec2(winWidth*0.5, 0.05 * winHeight);
    longueur = winWidth*0.2;
    hauteur = winHeight*0.1;
}

MusicPlayerTool::~MusicPlayerTool() {
    delete playButton;
    delete pauseButton;
    delete nextButton;
    delete previousButton;
}

void MusicPlayerTool::initSDL_Mixer(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

}   

void MusicPlayerTool::initSongs() {
    // Exemple d'ajout de chansons
    songs.push_back({"../assets/music/genesis.mp3", "Genesis","Movediz", 134, "../assets/music/albumCover/genesis.jpg", ImVec4(206,185,205,255)});
    songs.push_back({"../assets/music/deep.mp3", "Deep","Custesy Chamber Ensemble", 242, "../assets/music/albumCover/deep.jpg", ImVec4(221,200,210,255)}); // 180 secondes comme exemple
    songs.push_back({"../assets/music/frontiers.mp3", "Frontiers","Revo", 157, "../assets/music/albumCover/revo.jpg", ImVec4(137,157,161,255)});
    songs.push_back({"../assets/music/otherworld.mp3", "Otherworldly","All Good Folks", 357, "../assets/music/albumCover/allGoodFolks.jpg", ImVec4(250,82,50,255)});
    songs.push_back({"../assets/music/looking-forward.mp3", "Looking Forward","Caleb Fincher", 284, "../assets/music/albumCover/lookingForward.jpg", ImVec4(124,96,118,255)});
    songs.push_back({"../assets/music/universal.mp3", "Universal","Revo", 204, "../assets/music/albumCover/revo.jpg", ImVec4(137,157,161,255)});
    songs.push_back({"../assets/music/peacefulness.mp3", "Peacefulness","Simon Folwar", 151, "../assets/music/albumCover/peacefulness.jpg", ImVec4(122,164,154,255)});
    songs.push_back({"../assets/music/violet-haze.mp3", "Violet Haze","Ambient Boy", 597, "../assets/music/albumCover/VioletHaze.jpg", ImVec4(203,142,150,255)});
    songs.push_back({"../assets/music/oblivious.mp3", "Oblivious","Jonathan Doherty", 154, "../assets/music/albumCover/oblivious.jpg", ImVec4(154,137,127,255)});
    songs.push_back({"../assets/music/oneMomentInTime.mp3", "On Moment In Time","All Good Folks", 171, "../assets/music/albumCover/allGoodFolks.jpg", ImVec4(250,82,50,255)});


    // Charger les musiques
    for (auto& song : songs) {
        song.music = Mix_LoadMUS(getFullPath(song.path).c_str());
        if (!song.music) {
            std::cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
        song.albumTexture = loadTexture(getFullPath(song.albumCoverPath).c_str());
        song.backgroundColor = song.albumColor;
    }
    if (!songs.empty()) {
    currentSong = &songs[0]; 
    songLength = currentSong->length;
}
}


void MusicPlayerTool::Draw() {
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    drawUI();
    update();
    updateProgress();
    checkVolumeChange();
    //Print mouse pos in % of screen if clicked

}

void MusicPlayerTool::StartCounting(){
    shouldRestartSong = true;
}

void MusicPlayerTool::changeMusicVolume(float volume){
    Mix_VolumeMusic(volume*MIX_MAX_VOLUME);
}

void MusicPlayerTool::update() {
    // Si une musique est en train de jouer, ne rien faire
    if (Mix_PlayingMusic()) {
        return;
    }
    // Si la musique a fini de jouer, passer à la chanson suivante
    nextSong();
    play(); // Joue la chanson suivante
    songProgression = 0;
}
void MusicPlayerTool::nextSong() {
    size_t currentIndex = std::distance(songs.begin(), std::find(songs.begin(), songs.end(), *currentSong));

    if (isShuffling) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, songs.size() - 1);

        size_t nextIndex;
        do {
            nextIndex = distrib(gen);
        } while (songs.size() > 1 && nextIndex == currentIndex);

        currentSong = &songs[nextIndex];
    } else {
        size_t nextIndex = (currentIndex + 1) % songs.size();
        currentSong = &songs[nextIndex];
    }
    Mix_HaltMusic();

    // Charger et jouer la nouvelle chanson
    if (Mix_PlayMusic(currentSong->music, 1) == -1) { // 1 pour jouer une fois
        std::cerr << "Mix_PlayMusic error: " << Mix_GetError() << std::endl;
    }

    // Réinitialiser le temps de progression et mettre à jour les labels
    songProgression = 0;
    songLength = currentSong->length;
    songName->UpdateText(currentSong->name);
    songArtist->UpdateText(currentSong->artist);
    // Réinitialiser l'état de lecture
    playbackState = PlaybackState::Playing;
}

void MusicPlayerTool::previousSong() {
    size_t currentIndex = std::distance(songs.begin(), std::find(songs.begin(), songs.end(), *currentSong));
    currentIndex = (currentIndex == 0) ? songs.size() - 1 : currentIndex - 1;
    currentSong = &songs[currentIndex];

    Mix_HaltMusic();

    // Charger et jouer la nouvelle chanson
    if (Mix_PlayMusic(currentSong->music, 1) == -1) {
        std::cerr << "Mix_PlayMusic error: " << Mix_GetError() << std::endl;
    }

    // Réinitialiser le temps de progression et mettre à jour les labels
    songProgression = 0;
    songLength = currentSong->length;
    songName->UpdateText(currentSong->name);
    songArtist->UpdateText(currentSong->artist);
    // Réinitialiser l'état de lecture
    playbackState = PlaybackState::Playing;
}

void MusicPlayerTool::play() {
    if (currentSong != nullptr) {
        if (playbackState == PlaybackState::Paused) {
            // Calculer la durée de la pause
            auto pauseDuration = std::chrono::steady_clock::now() - pauseTimePoint;
            totalPauseDuration += std::chrono::duration_cast<std::chrono::seconds>(pauseDuration);
        }
        Mix_ResumeMusic();
        playbackState = PlaybackState::Playing;
    }
}

void MusicPlayerTool::restartSong(){
    Mix_HaltMusic();
    Mix_PlayMusic(currentSong->music, 1);
    songProgression = 0;
    playbackState = PlaybackState::Playing;
}

void MusicPlayerTool::pause() {
    if (playbackState == PlaybackState::Playing) {
        Mix_PauseMusic();
        playbackState = PlaybackState::Paused;
        // Marquer le moment de la pause
        pauseTimePoint = std::chrono::steady_clock::now();
    }
}

void MusicPlayerTool::repeatButton(){
    isRepeating = !isRepeating;
    if(isRepeating){
        repeatButtonOn->isOn = true;
    }else{
        repeatButtonOn->isOn = false;
    }
}

void MusicPlayerTool::shuffleButton(){
    isShuffling = !isShuffling;
    if(isShuffling){
        shuffleButtonOn->isOn = true;
    }else{
        shuffleButtonOn->isOn = false;
    }
}

void MusicPlayerTool::setSongTime(float time){
    songProgression = time;
    Mix_RewindMusic();
    //Mix_PlayMusic(currentSong->music, 1);
    Mix_SetMusicPosition(time);
}

void MusicPlayerTool::updateProgress() {
    if (playbackState != PlaybackState::Playing) return;

    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(now - lastUpdateTime);

    if (timeSinceLastUpdate.count() >= 1) {
        // Soustraire totalPauseDuration du temps écoulé avant d'incrémenter songProgression
        auto effectiveTimeElapsed = timeSinceLastUpdate.count() - totalPauseDuration.count();
        songProgression += effectiveTimeElapsed;
        // Réinitialiser totalPauseDuration après l'avoir pris en compte
        totalPauseDuration = std::chrono::seconds(0);
        lastUpdateTime = now;
    }

    if (songProgression >= songLength) {
        if (isRepeating) {
            songProgression = 0;
            Mix_HaltMusic();
            Mix_PlayMusic(currentSong->music, 1);
        } else {
            nextSong();
        }
    }
}



void MusicPlayerTool::checkVolumeChange(){
    if (currentVolume != m_renderContext->soundTool->currentMusicVolume) {
        currentVolume = m_renderContext->soundTool->currentMusicVolume;
        changeMusicVolume(currentVolume);
    }
}






void MusicPlayerTool::setVolumeButtonFunction(std::function<void()> function) {
    volumeButtonFunction = function;
}

void MusicPlayerTool::volumeButtonFunctionCalled() {
    volumeButtonFunction();
}

void MusicPlayerTool::updateProgressionLabbel(){
    int progression = songProgression;
    int minutes = progression / 60;
    int seconds = progression % 60;
    if (minutes < 0) {
        minutes = 0;
    }
    if (seconds < 0) {
        seconds = 0;
    }
    std::ostringstream progressionStream;
    progressionStream << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    songProgressionTime->UpdateText(progressionStream.str());

    int timeLeft = songLength - progression;
    minutes = timeLeft / 60;
    seconds = timeLeft % 60;
    if (minutes < 0) {
        minutes = 0;
    }
    if (seconds < 0) {
        seconds = 0;
    }
    std::ostringstream timeLeftStream;
    timeLeftStream <<"-"<< minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    songTimeLeft->UpdateText(timeLeftStream.str());
}


void MusicPlayerTool::initUI(){
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);

    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);
    float hauteurMain = 0.084f;
    float hauteurBar = hauteurMain-0.023f;

    playButton = new ImageButton(playSoundFunc,0.5, hauteurMain, ImVec2(0.020,0.025),0.60,
                        button_color,button_color,
                        "../assets/button/play.png", 0,
                            std::bind(&MusicPlayerTool::play, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    pauseButton = new ImageButton(playSoundFunc,0.5, hauteurMain, ImVec2(0.020,0.025),0.60,
                        button_color,button_color,
                        "../assets/button/pause.png", 0,
                            std::bind(&MusicPlayerTool::pause, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    nextButton = new ImageButton(playSoundFunc,0.525, hauteurMain, ImVec2(0.015,0.015),0.95,
                        button_color,button_color,
                        "../assets/button/forward.png", 0,
                            std::bind(&MusicPlayerTool::nextSong, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    previousButton = new ImageButton(playSoundFunc,0.475, hauteurMain, ImVec2(0.015,0.015),0.95,
                        button_color,button_color,
                        "../assets/button/backward.png", 0,
                            std::bind(&MusicPlayerTool::previousSong, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    repeatButtonOn = new ImageButton(playSoundFunc,0.435, hauteurMain, ImVec2(0.025,0.025),0.60,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/repeat.png",0.0f,
                            std::bind(&MusicPlayerTool::repeatButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    shuffleButtonOn = new ImageButton(playSoundFunc,0.565, hauteurMain, ImVec2(0.025,0.025),0.60,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/shuffle.png",0.0f,
                            std::bind(&MusicPlayerTool::shuffleButton, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);
    volumeButton = new ImageButton(playSoundFunc,0.415, 0.015, ImVec2(0.02,0.02),0.8,
                        ImVec4(0.17f, 0.27f, 0.17f, 1.0f),ImVec4(0.17f, 0.27f, 0.17f, 1.0f),
                        "../assets/button/volume.png",0.0f,
                            std::bind(&MusicPlayerTool::volumeButtonFunctionCalled, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);


    songName = new Labbel(0.5f,0.02f,ImVec4(255,255,255,255),
                            "Interstellar",15.0f,0.7);
    songArtist = new Labbel(0.5f,0.0375f,ImVec4(255,255,255,255),
                            "Cutesy Chamber Ensemble",13.0f,0.55);
    songProgressionTime = new Labbel(0.415f,hauteurBar,ImVec4(255,255,255,255),
                            "0:00",14.0f,0.6f);
    songTimeLeft = new Labbel(0.585f,hauteurBar,ImVec4(255,255,255,255),
                            "0:00",14.0f,0.6f);
}

void ShadeVertsLinearColorGradientSetAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1) {
    ImVec2 gradient_extent = ImVec2(gradient_p1.x - gradient_p0.x, gradient_p1.y - gradient_p0.y);
    float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
    ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    ImDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    const int col0_r = (col0 >> IM_COL32_R_SHIFT) & 0xFF;
    const int col0_g = (col0 >> IM_COL32_G_SHIFT) & 0xFF;
    const int col0_b = (col0 >> IM_COL32_B_SHIFT) & 0xFF;
    const int col0_a = (col0 >> IM_COL32_A_SHIFT) & 0xFF;
    const int col1_r = (col1 >> IM_COL32_R_SHIFT) & 0xFF;
    const int col1_g = (col1 >> IM_COL32_G_SHIFT) & 0xFF;
    const int col1_b = (col1 >> IM_COL32_B_SHIFT) & 0xFF;
    const int col1_a = (col1 >> IM_COL32_A_SHIFT) & 0xFF;
    for (ImDrawVert* vert = vert_start; vert < vert_end; vert++) {
        float d = ImDot(ImVec2(vert->pos.x - gradient_p0.x,vert->pos.y - gradient_p0.y), gradient_extent);
        float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
        int r = col0_r + (col1_r - col0_r) * t;
        int g = col0_g + (col1_g - col0_g) * t;
        int b = col0_b + (col1_b - col0_b) * t;
        int a = col0_a + (col1_a - col0_a) * t;
        vert->col = IM_COL32(r, g, b, a);
    }
}
void MusicPlayerTool::drawUI(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 topLeft = ImVec2(centerPos.x - longueur / 2, centerPos.y - hauteur / 2);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueur / 2, centerPos.y + hauteur / 2);
    float cornerRadius = winWidth*0.01;

    drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(20, 25, 30, 200),cornerRadius);
    
    //drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(20, 25, 30, 200),0);
    // Dégradé pour la couleur de l'album
    // Couleur de base (RGB constant, A variable)
    int baseR = currentSong->backgroundColor.x;
    int baseG = currentSong->backgroundColor.y;
    int baseB = currentSong->backgroundColor.z;
    drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(baseR, baseG, baseB, 20),cornerRadius);
    // Alpha values
    int alphaTopLeft = 0;     // Complètement transparent
    int alphaTopRight = 70;  // Pic de couleur, plus opaque
    int alphaBottomRight = 0; // Retour à transparent
    int alphaBottomLeft = 0;  // Transparent

    // Conversion des valeurs alpha pour ImGui
    ImU32 colTopLeft = IM_COL32(baseR, baseG, baseB, alphaTopLeft);
    ImU32 colTopRight = IM_COL32(baseR, baseG, baseB, alphaTopRight);
    ImU32 colBottomRight = IM_COL32(baseR, baseG, baseB, alphaBottomRight);
    ImU32 colBottomLeft = IM_COL32(baseR, baseG, baseB, alphaBottomLeft);

    // Dessiner le rectangle avec dégradé d'alpha
    const ImVec2 picker_pos = ImVec2(topLeft.x+longueur*2/3,topLeft.y); // Position de départ du rectangle
    const ImVec2 sv_picker_size = ImVec2(longueur/3, bottomRight.y - topLeft.y); // Taille du rectangle
    ImU32 col_white = IM_COL32(255, 255, 255, 255); // Utilisé pour la commande AddRectFilled

    //picker_pos + sv_picker_size
    ImVec2 diff = ImVec2(sv_picker_size.x + picker_pos.x, sv_picker_size.y + picker_pos.y);
    int vtx_idx_start = drawList->VtxBuffer.Size; // Index de départ avant dessin
    drawList->AddRectFilled(picker_pos, diff, col_white, cornerRadius); // Dessinez avec des coins arrondis
    int vtx_idx_end = drawList->VtxBuffer.Size; // Index de fin après dessin

    // Appliquez le dégradé
    ShadeVertsLinearColorGradientSetAlpha(drawList, vtx_idx_start, vtx_idx_end, picker_pos, ImVec2(picker_pos.x+sv_picker_size.x, picker_pos.y), colTopLeft,colTopRight);
    




    //Buttons

    //drawList->AddRectFilledMultiColor(topLeft, bottomRight, colTopLeft, colTopRight, colBottomRight, colBottomLeft);

    updateProgressionLabbel();
    if(playbackState != PlaybackState::Playing){playButton->Draw();}
    else{pauseButton->Draw();}
    repeatButtonOn->Draw();
    shuffleButtonOn->Draw();
    nextButton->Draw();
    previousButton->Draw();
    songName->Draw();
    songArtist->Draw();
    songProgressionTime->Draw();
    songTimeLeft->Draw();
    volumeButton->Draw();
    drawProgressionBar();

    //Draw current song texture at the top right
    ImVec2 texturePos = ImVec2(winWidth*0.573438, 0.0262248 * winHeight);
    ImVec2 textureSize = ImVec2(hauteur*0.4, hauteur*0.4);
    ImVec2 topLeftTexture = ImVec2(texturePos.x - textureSize.x / 2, texturePos.y - textureSize.y / 2);
    ImVec2 bottomRightTexture = ImVec2(texturePos.x + textureSize.x / 2, texturePos.y + textureSize.y / 2);

    //Background image a bit bigger
    ImVec2 textureSizeBackground = ImVec2(textureSize.x*3, textureSize.y*3);
    ImVec2 topLeftTextureBackground = ImVec2(centerPos.x - textureSizeBackground.x / 2, centerPos.y - textureSizeBackground.y / 2);
    ImVec2 bottomRightTextureBackground = ImVec2(centerPos.x + textureSizeBackground.x / 2, centerPos.y + textureSizeBackground.y / 2);

    drawList->AddImage((void*)(intptr_t)currentSong->albumTexture, topLeftTexture, bottomRightTexture, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 200));
    //drawList->AddImage((void*)(intptr_t)currentSong->albumTexture, topLeftTextureBackground, bottomRightTextureBackground, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 50));
}

void MusicPlayerTool::drawProgressionBar(){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float hauteurMain = 0.084f;
    float hauteurBar = hauteurMain-0.023f;
    ImVec2 centerPos = ImVec2(winWidth*0.5, hauteurBar * winHeight);
    float longueur = winWidth*0.14;
    float hauteur = winHeight*0.005;
    float cornerRadius = winWidth*0.02;

    ImVec2 mousePos = ImGui::GetMousePos();
    float heightPadding = 0.01f * winHeight;
    ImVec2 topLeft = ImVec2(centerPos.x - longueur / 2, centerPos.y - hauteur / 2);

    bool isMouseOverBar = mousePos.x > topLeft.x && mousePos.x < topLeft.x + longueur &&
        mousePos.y > topLeft.y - heightPadding && mousePos.y < topLeft.y + hauteur + heightPadding;
    bool isMouseDown = ImGui::IsMouseDown(0);
    if(isMouseOverBar){ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);}
    //Empty rec for progression
    drawList->AddRectFilled(topLeft, 
                            ImVec2(topLeft.x + longueur, topLeft.y + hauteur), 
                            IM_COL32(255, 255, 255, 50), // Color
                            cornerRadius);

    //Progression
    float longueurProgression = longueur * songProgression / songLength;
    if(!(isMouseDown && isMouseOverBar))
    {
        drawList->AddRectFilled(topLeft, 
                                ImVec2(topLeft.x + longueurProgression, topLeft.y + hauteur), 
                                IM_COL32(255, 255, 255, 100), // Color
                                cornerRadius);
    }


    //Add semi-transparant rectangle with position of mouse if mouse is over progression bar
    float alpha = isMouseDown ? 150 : 75;
    if(isMouseOverBar)
        {
        drawList->AddRectFilled(topLeft, 
                                ImVec2(mousePos.x, topLeft.y + hauteur), 
                                IM_COL32(255, 255, 255, alpha), // Color
                                cornerRadius);
    }
    //if click on progression bar, change song progression
    if(ImGui::IsMouseReleased(0) && mousePos.x > topLeft.x && mousePos.x < topLeft.x + longueur &&
        mousePos.y > topLeft.y - heightPadding && mousePos.y < topLeft.y + hauteur + heightPadding)
        {
        setSongTime((mousePos.x - topLeft.x) / longueur * songLength);
    }
}

bool MusicPlayerTool::mouseHoveringPlayer(){
    ImVec2 topLeft = ImVec2(centerPos.x - longueur / 2, centerPos.y - hauteur / 2);
    ImVec2 bottomRight = ImVec2(centerPos.x + longueur / 2, centerPos.y + hauteur / 2);

    ImVec2 mousePos = ImGui::GetMousePos();
    if(mousePos.x > topLeft.x && mousePos.x < bottomRight.x &&
        mousePos.y > topLeft.y && mousePos.y < bottomRight.y)
        {
        return true;
    }
    return false;
}


GLuint MusicPlayerTool::loadTexture(const char* relativeFilename) {
    std::string fullPath = getFullPath(relativeFilename);
    cv::Mat image = cv::imread(fullPath);
    if (image.empty()) {
        std::cerr << "Erreur: Image non trouvée " << fullPath << std::endl;
        return 0;
    }

    //Change alpha 


    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}