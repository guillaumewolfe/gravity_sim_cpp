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
        float flickerFrequency = 2.0f; // Ralentir le scintillement
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
    float radius = 200.0f; // Ajuster si nécessaire

    for (int i = 0; i < numberOfStars; i++) {
        Star star;
        star.indice = i;
        float theta = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/M_PI));
        float phi = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(2*M_PI)));

        // Position aléatoire sur la sphère
        star.x = radius * sin(theta) * cos(phi);
        star.y = radius * sin(theta) * sin(phi);
        star.z = radius * cos(theta);

        // Taille aléatoire plus variée
        star.size = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/2.0f)) + 1.5f;

        // Couleur aléatoire avec une tendance vers le blanc et le jaune
        float colorRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        star.r = 0.8f + 0.2f * colorRand; 
        star.g = 0.8f + 0.2f * colorRand;
        star.b = 0.8f + 0.2f * colorRand;

        // Transparence aléatoire
        star.alpha = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        // Initialisation des champs originaux
        star.originalSize = star.size;
        star.originalAlpha = star.alpha*0.1;

        star.flickerOffset = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI; // Décalage entre 0 et 2π

        stars.push_back(star);
    }
}
