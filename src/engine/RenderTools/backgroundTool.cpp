#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/backgroundTool.h"

BackgroundTool::BackgroundTool(RenderContext* renderContext) : RenderComponent(renderContext){
    srand(static_cast<unsigned>(time(nullptr)));
    initStars(30000);
}



void BackgroundTool::Draw() {
    glEnable(GL_DEPTH_TEST);
    glUseProgram(0); // Utiliser le shader par défaut

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float time = *(m_renderContext->simulationTime);

    glBegin(GL_POINTS);
    for (Star& star : stars) {
        // Scintillement avec décalage aléatoire
        float flickerFrequency = 1.0f; // Ralentir le scintillement
        float flicker = (sin(time * flickerFrequency + star.flickerOffset) + 1.0f) * 0.5f;
        star.size = star.originalSize + flicker * 0.1f; // Réduire l'amplitude du scintillement
        star.alpha = star.originalAlpha + flicker * 0.5f;

        glPointSize(star.size);
        glColor4f(star.r, star.g, star.b, star.alpha);
        glVertex3f(star.x, star.y, star.z);
    }
    glEnd();

    glDisable(GL_BLEND);
}



void BackgroundTool::initStars(int numberOfStars) {
    this->numberOfStars = numberOfStars;
    float radius = 100.0f; // Ajuster si nécessaire

    for (int i = 0; i < numberOfStars; i++) {
        Star star;
        star.indice = i;

        // Generate random spherical coordinates
        float u = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float v = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        float theta = 2.0f * M_PI * u;
        float phi = acos(2.0f * v - 1.0f);

        // Convert spherical coordinates to Cartesian coordinates
        star.x = radius * sin(phi) * cos(theta);
        star.y = radius * sin(phi) * sin(theta);
        star.z = radius * cos(phi);

        // Check if the star should move
        if (rand() % 100 < 1) { // Adjust 1 to your desired percentage (1% in this case)
            // Add a small random displacement to the star's position
            float displacement = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;
            star.x += radius * 0.01f * cos(displacement);
            star.y += radius * 0.01f * sin(displacement);
            star.z += radius * 0.01f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - radius * 0.005f; // Small random z adjustment
        }

        // Other random properties
        star.size = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) + 1.5f;
        star.r = 0.8f + 0.2f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        star.g = 0.8f + 0.2f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        star.b = 0.8f + 0.2f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        star.alpha = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        star.originalSize = star.size;
        star.originalAlpha = star.alpha * 0.1;
        star.flickerOffset = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;

        stars.push_back(star);
    }
}


