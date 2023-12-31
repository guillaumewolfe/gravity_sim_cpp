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

Game::Game() : currentState(nullptr), window(nullptr), 
               requestedState(nullptr), changeStateRequested(false) {
    InitSettings();
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
states["menu"] = new MenuState(this);
states["simulation"] = new SimulationState(this);
ChangeState("menu");
}

void Game::Close()
{
    if (currentState)
        for (auto& state : states) {
            delete state.second;
        }

    CleanupOpenGL();
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

    // Obtention de la résolution de l'écran principal
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);



    // Définir les dimensions maximales en fonction de la résolution de l'écran
    int windowWidth = mode->width;
    int windowHeight = mode->height;
    
    window = glfwCreateWindow(windowWidth, windowHeight, "Space Query", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return false;
    }
    settings.resolutionX = windowWidth;
    settings.resolutionY = windowHeight;
    settings.fullscreen = (glfwGetWindowMonitor(window) != NULL); // Vérifier si la fenêtre est en plein écran

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
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

GameSettings* Game::getSettings() { return &settings; }

void Game::InitSettings() {
    settings.textureQuality = 1;
    settings.antiAliasing = true;
    settings.movementSpeed = 0.75f;
    settings.rotationSpeed = 0.75f;
    settings.musicVolume = 0.0f;
    settings.sfxVolume = 0.0f;
    settings.mainVolume = 0.0f;
    settings.volumeChanged = false;
}