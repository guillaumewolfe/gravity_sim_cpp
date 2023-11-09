#include <iostream>
#include <game/game.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <filesystem>
#include "opencv2/opencv.hpp"

Game::Game() : currentState(nullptr), window(nullptr), myFont(nullptr), 
               requestedState(nullptr), changeStateRequested(false) {}

Game::~Game()
{
    Close();
}
bool Game::ShouldClose() const { return shouldClose; }
void Game::setShouldClose(bool value) { shouldClose = value; }

void Game::Init()
{ 
LoadFonts(); 
}

void Game::LoadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    fonts["Regular"] = io.Fonts->AddFontFromFileTTF("../assets/fonts/TiltNeon-Regular.ttf", 22.0f);
    fonts["Title"] = io.Fonts->AddFontFromFileTTF("../assets/fonts/TiltNeon-Regular.ttf", 100.0f);
    fonts["Main Menu"] = io.Fonts->AddFontFromFileTTF("../assets/fonts/TiltNeon-Regular.ttf", 30.0f);

    // Assurez-vous que toutes les polices ont été chargées correctement
    for (auto& font : fonts) {
        if (!font.second) {
            std::cerr << "Failed to load font: " << font.first << std::endl;
        }
    }
}

ImFont* Game::getFont(const std::string& fontKey) {
    auto it = fonts.find(fontKey);
    if (it != fonts.end()) {
        return it->second;
    }
    return nullptr;
}
ImFont* Game::getFont() {
    // Utilisez une clé par défaut ou un chemin de fichier pour la police "Regular"
    return getFont("TiltNeon-Regular.ttf"); // ou le chemin relatif selon l'organisation de vos fichiers
}
void Game::Close()
{
    if (currentState)
    {
        currentState->Exit();
        delete currentState;
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

void Game::ChangeState(BaseState* newState) {
    if (requestedState != nullptr) {
        delete requestedState;
    }
    requestedState = newState;
    changeStateRequested = true;
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

    window = glfwCreateWindow(1920, 1080, "Space Query", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

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
