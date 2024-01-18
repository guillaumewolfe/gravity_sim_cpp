#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/AxesTool.h"

AxesTool::AxesTool(RenderContext* renderContext) : RenderComponent(renderContext){
    lineLength = m_renderContext->systemeSolaire->maxSize;
}


void AxesTool::Draw() {
    glUseProgram(0); // Utiliser le shader par défaut
    glLineWidth(2.0f);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //Begin alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(1.0f, 0.3f, 0.3f,0.5f); // Axe X en rouge
    glBegin(GL_LINES);
    glVertex3f(-lineLength, 0.0f, 0.0f);
    glVertex3f(lineLength, 0.0f, 0.0f);
    glEnd();

    glColor4f(0.3f, 1.0f, 0.3f,0.5); // Axe X en rouge
    glBegin(GL_LINES);
    glVertex3f(0.0f, -lineLength, 0.0f);
    glVertex3f( 0.0f,lineLength, 0.0f);
    glEnd();

    glColor4f(0.3f, 0.3f, 1.0f,0.5f); // Axe Z en bleu
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -lineLength);
    glVertex3f(0.0f, 0.0f, lineLength);
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    
// Calcul de l'espacement de la grille
    int number = 15;
    float gridSpacing = lineLength / number;

    // Couleur de la grille (peut être modifiée selon vos préférences)
    glColor4f(0.3f, 0.3f, 0.3f,0.7f);
    glLineWidth(1.0f);
    // Dessiner la grille dans le plan XZ
    for (int i = -number; i <= number; ++i) {
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
    glDisable(GL_BLEND);
}



