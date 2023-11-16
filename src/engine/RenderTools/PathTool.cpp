#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/PathTool.h"


PathTool::PathTool(RenderContext* renderContext) : RenderComponent(renderContext){
}

void PathTool::Draw() {
    // Assurez-vous que le mode de rendu OpenGL est configuré pour dessiner des lignes
    glLineWidth(2.0f);  // Définissez l'épaisseur de la ligne selon vos besoins
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        const auto& positionHistory = object->getPositionHistory();

        if (positionHistory.size() < 2) {
            continue;
        }

        glBegin(GL_LINE_STRIP);

        // Itérez sur l'historique des positions
        for (size_t i = 0; i < positionHistory.size(); ++i) {
            // Calculer l'alpha pour chaque point
            float alpha = 0.7f * (static_cast<float>(i) / static_cast<float>(positionHistory.size() - 1));

            // Définir la couleur de la ligne avec l'alpha calculé
            glColor4f(1.0f, 1.0f, 1.0f, alpha); // Ici, la couleur de base est blanche, ajustez selon vos besoins

            const auto& point = positionHistory[i];
            glVertex3f(point.x, point.y, point.z);
        }

        glEnd();
    }
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    // Restaurez les paramètres OpenGL par défaut si nécessaire
}