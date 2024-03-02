#include <iostream>
#include <game/game.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <filesystem>
#include "opencv2/opencv.hpp"
#include <states/LoadingState.h>
#include <states/MenuState.h>
#include <states/SimulationState.h>
#include <cmath> // Include the cmath library for sin function
#include "save/saveTool.h"


Game::Game() : currentState(nullptr), window(nullptr), 
               requestedState(nullptr), changeStateRequested(false) {
    InitSettings();
    loadSettings();
               }

Game::~Game()
{
    Close();
}
bool Game::ShouldClose() const { return shouldClose; }
void Game::setShouldClose(bool value) { shouldClose = value; }

void Game::Init()
{ 
states["loading"] = new LoadingState(this);
std::thread loadingThread(&Game::InitLoadingWindow, this);
loadingThread.detach();
states["menu"] = new MenuState(this);
states["simulation"] = new SimulationState(this);
loadingThreadShouldExit = true;
ChangeState("menu");
showMainWindow();

}

void Game::Close()
{
    if (currentState)
        for (auto& state : states) {
            state.second->Exit();
            delete state.second;
        }

    CleanupOpenGL();
    if (settings.soundTool) {
        delete settings.soundTool;
    }
    delete currentState;
    delete requestedState;
    ImGui::DestroyContext(loadingIMGUIContext);
    ImGui::DestroyContext(mainIMGUIContext);
    delete loadingIMGUIContext;
    delete mainIMGUIContext;

}

void Game::Update()
{
    if (currentState)
    {
        currentState->Update();
    }
    ApplyStateChange();
}

void Game::UpdatePhysics(double dt)
{
    if (currentState)
    {
        currentState->UpdatePhysics(dt);
    }
}

void Game::Draw()
{
    if (currentState)
    {
        currentState->Draw();
    }
}

void Game::ChangeState(const std::string& stateKey) {
    auto it = states.find(stateKey);
        currentState = it->second;
        currentState->RestartState();
        }

void Game::ApplyStateChange() {
    if (changeStateRequested && requestedState != nullptr) {
        if (currentState) {
            currentState->Exit();
            delete currentState;
        }
        currentState = requestedState;
        currentState->Enter();
        requestedState = nullptr;
        changeStateRequested = false;
    }
}
void Game::getState()
{
    if (currentState)
    {
        std::cout << currentState->getDescription() << std::endl;
    }
}

bool Game::InitOpenGL()
{
    if (!glfwInit())
    {
        return false;
    }

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    // Obtenez les dimensions de l'espace de travail de l'écran
    int workAreaX, workAreaY, workAreaWidth, workAreaHeight;
    glfwGetMonitorWorkarea(primaryMonitor, &workAreaX, &workAreaY, &workAreaWidth, &workAreaHeight);

    // Configurer la fenêtre pour être "windowed borderless"
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    // Création de la fenêtre en utilisant les dimensions de l'espace de travail
    window = glfwCreateWindow(workAreaWidth, workAreaHeight, "Space Query", NULL, NULL);
    glfwHideWindow(window);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    // Placer la fenêtre pour s'adapter à l'espace de travail
    glfwSetWindowPos(window, workAreaX, workAreaY);

    settings.resolutionX = workAreaWidth;
    settings.resolutionY = workAreaHeight;
    settings.fullscreen = false;

    glfwMakeContextCurrent(window);
    gladLoadGL();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120");

    ImGui::StyleColorsDark();
    return true;
}


GLFWwindow *Game::getWindow()
{
    return window;
}

void Game::CleanupOpenGL()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
}

GameSettings* Game::getSettings() { return &settings; }
GameSettings* Game::getDefaultSettings() { return &defaultSettings; }

void Game::InitSettings() {
    settings.textureQuality = 1;
    settings.antiAliasing = true;
    settings.movementSpeed = 0.5f;
    settings.rotationSpeed = 0.5f;
    settings.musicVolume = 0.6f;
    settings.sfxVolume = 0.6f;
    settings.mainVolume = 0.6f;
    settings.volumeChanged = false;
    settings.soundTool = new SoundTool(&settings);
    defaultSettings = settings;
}

//getSoundTool
SoundTool* Game::getSoundTool() { return settings.soundTool; }

void Game::InitLoadingWindow() {
    // Initialize GLFW for this thread
    glfwInit();
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    // Obtenez les dimensions de l'espace de travail de l'écran
    int workAreaX, workAreaY, workAreaWidth, workAreaHeight;
    glfwGetMonitorWorkarea(primaryMonitor, &workAreaX, &workAreaY, &workAreaWidth, &workAreaHeight);
    int windowWidth = workAreaWidth * 0.2;
    int windowHeight = workAreaWidth * 0.2; // Utilisez workAreaHeight si vous voulez baser la hauteur proportionnellement

    // Configurer la fenêtre pour être "windowed borderless"
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    // Création de la fenêtre en utilisant les dimensions de l'espace de travail
    loadingWindow = glfwCreateWindow(windowWidth, windowHeight, "Space Query", NULL, NULL);
    if (!loadingWindow) {
        glfwTerminate();
        return;
    }

    dynamic_cast<LoadingState*>(states["loading"])->setWindow(loadingWindow);
    int centerX = workAreaX + (workAreaWidth - windowWidth) / 2;
    int centerY = workAreaY + (workAreaHeight - windowHeight) / 2;

    // Positionner la fenêtre au centre
    glfwSetWindowPos(loadingWindow, centerX, centerY);
    // Make the context of the loading window current on this thread
    glfwMakeContextCurrent(loadingWindow);
    // Initialize GLAD for this thread/context
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }


    //Check font atlas
    // Simple loading loop
    while (!glfwWindowShouldClose(loadingWindow) && !loadingThreadShouldExit) {
        glfwPollEvents();
        dynamic_cast<LoadingState*>(states["loading"])->Draw();
        // Swap buffers
        glfwSwapBuffers(loadingWindow);
    }
    glfwDestroyWindow(loadingWindow);
}

void Game::showMainWindow() {
    if (window != nullptr) {
        glfwShowWindow(window);
    }
}

void Game::loadSettings(){
    SaveTool saveTool;
    bool alreadyWritten = saveTool.checkIfSettingsAlreadyWritten(false);
    if (!alreadyWritten) {
        saveTool.saveGameSettings(settings);
    }else{
        saveTool.loadSettings(&settings);
    }
}