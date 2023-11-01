#include <iostream>
#include <game/game.h>
#include <UI/Buttons.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <filesystem>
#include "opencv2/opencv.hpp"

Game::Game() : currentState(nullptr), window(nullptr), myFont(nullptr) {}

Game::~Game()
{
    Close();
}
bool Game::ShouldClose() const { return shouldClose; }
void Game::setShouldClose(bool value) { shouldClose = value; }
ImFont* Game::getFont(){return myFont;};

void Game::Init()
{ ImFont *myFont = Create_font();
/* 
    Button *exitButton = new Button(0.5f, 0.5f, ImVec2(100, 50),
                                    ImVec4(0.5f, 0.5f, 0.7f, 1.0f),
                                    ImVec4(0.7f, 0.5f, 0.5f, 1.0f),
                                    "Exit", myFont, 0.2f,
                                    [this]()
                                    { if (this->currentState) { this->shouldClose = true; } });
    buttons.push_back(exitButton);*/
}

ImFont *Game::Create_font()
{
    ImGuiIO &io = ImGui::GetIO();

    // Load the font
    ImFont *myFont = io.Fonts->AddFontFromFileTTF("../assets/fonts/TiltNeon-Regular.ttf", 16.0f);
    if (!myFont)
    {
        std::cerr << "Failed to load font." << std::endl;
    }
    return myFont;
}

void Game::Close()
{
    if (currentState)
    {
        currentState->Exit();
        delete currentState;
    }
/*
    for (Button *btn : buttons)
    {
        delete btn;
    }*/

    CleanupOpenGL();
}

void Game::Update()
{
    if (currentState)
    {
        currentState->Update();
    }
}

void Game::UpdatePhysics(int dt)
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
    /*
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // You can set your preferred clear color here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set the contrasting background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White background
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    ImGui::PopStyleColor(); // Reset to original style after ImGui::Begin

    // Draw your custom buttons
    for (Button *btn : buttons)
    {
        btn->Draw();
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glDisable(GL_BLEND);*/
}

void Game::ChangeState(BaseState *newState)
{
    if (currentState)
    {
        currentState->Exit();
        delete currentState;
    }
    currentState = newState;
    if (currentState)
    {
        currentState->Enter(); 
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

    window = glfwCreateWindow(500, 500, "Space Query", NULL, NULL);
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
