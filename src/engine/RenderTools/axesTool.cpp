#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/AxesTool.h"

AxesTool::AxesTool(RenderContext* renderContext) : RenderComponent(renderContext){
    lineLength = m_renderContext->systemeSolaire->maxSize;
}



void AxesTool::Draw() {
    glUseProgram(0); // Utiliser le shader par défaut
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Axe X en rouge
    glBegin(GL_LINES);
    glVertex3f(-lineLength, 0.0f, 0.0f);
    glVertex3f(lineLength, 0.0f, 0.0f);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f); // Axe X en rouge
    glBegin(GL_LINES);
    glVertex3f(0.0f, -lineLength, 0.0f);
    glVertex3f( 0.0f,lineLength, 0.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f); // Axe Z en bleu
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -lineLength);
    glVertex3f(0.0f, 0.0f, lineLength);
    glEnd();
    glDisable(GL_LINE_SMOOTH);

// Calcul de l'espacement de la grille
    float gridSpacing = lineLength / 15.0f;

    // Couleur de la grille (peut être modifiée selon vos préférences)
    glColor3f(0.3f, 0.3f, 0.3f);

    // Dessiner la grille dans le plan XZ
    for (int i = -15; i <= 15; ++i) {
        float linePos = i * gridSpacing;

        // Lignes parallèles à l'axe X
        glBegin(GL_LINES);
        glVertex3f(-lineLength, 0.0f, linePos);
        glVertex3f(lineLength, 0.0f, linePos);
        glEnd();

        // Lignes parallèles à l'axe Z
        glBegin(GL_LINES);
        glVertex3f(linePos, 0.0f, -lineLength);
        glVertex3f(linePos, 0.0f, lineLength);
        glEnd();
}
}



