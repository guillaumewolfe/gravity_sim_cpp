#include "states/SimulationState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

SimulationState::SimulationState(GLFWwindow* window) : BaseState(window) {}

void SimulationState::Enter() {
    std::cout << "Entering Simulation State" << std::endl;
    font = new FTPixmapFont("/Library/Fonts/Arial Unicode.ttf");
    if(font->Error()) {
    // Gérer l'erreur
    std::cerr << "Erreur lors du chargement de la police!" << std::endl;
}
    font->FaceSize(50); 
}

void SimulationState::Update() {
}


void SimulationState::Draw() {
    // Effacer la fenêtre avec une couleur de fond (par exemple, noir)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Définir la couleur de fond en noir
    glClear(GL_COLOR_BUFFER_BIT);

    // Configurer la projection orthogonale pour le rendu 2D
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    FTBBox bbox = font->BBox("Simulation en cours");
    float textWidth = bbox.Upper().Xf() - bbox.Lower().Xf();
    float textHeight = bbox.Upper().Yf() - bbox.Lower().Yf();

    float x_position = (windowWidth - textWidth) / 2.0f;
    float y_position = (windowHeight - textHeight) / 2.0f;

    glColor3f(1.0f, 1.0f, 1.0f); // Couleur blanche
    glRasterPos2f(x_position, y_position); // Position où le texte doit être affiché
    font->Render("Simulation en cours");
}



void SimulationState::Exit() {
    delete font;
    font = nullptr;
    std::cout << "Exiting Simulation State" << std::endl;
}

std::string SimulationState::getDescription() {
    return "Simulation State";
}
