#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/PathTool.h"


PathTool::PathTool(RenderContext* renderContext) : RenderComponent(renderContext){
}

// Desctructeur
PathTool::~PathTool() {
}



void PathTool::Draw() {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    if(!m_renderContext->showAllPaths || *(m_renderContext->isCreating)){
        return;
    }
    // Assurez-vous que le mode de rendu OpenGL est configuré pour dessiner des lignes
    glLineWidth(2.0f);  // Définissez l'épaisseur de la ligne selon vos besoins
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        if(object == m_renderContext->currentCamera->followedObject && !m_renderContext->currentCamera->isGlobalFollowing){
            continue;
        }
        if(!object->showPath){
            continue;
        }
        const auto& positionHistory = object->getPositionHistory();
        ImVec4 circleColor = m_renderContext->colorByTypeDict[object->type].second;
        if (positionHistory.size() < 2) {
            continue;
        }

        glBegin(GL_LINE_STRIP);
        // Itérez sur l'historique des positions
        for (size_t i = 0; i < positionHistory.size(); ++i) {
            // Calculer l'alpha pour chaque point
            float alpha = 0.7f * (static_cast<float>(i) / static_cast<float>(positionHistory.size() - 1));
            //glColor4f(circleColor.x/255,circleColor.y/255,circleColor.z/255, alpha); // Ici, la couleur de base est blanche, ajustez selon vos besoins
            glColor4f(1.0f, 1.0f, 1.0f, alpha);
            // Définir la couleur de la ligne avec l'alpha calculé
            const auto& point = positionHistory[i];
            glVertex3f(point.x, point.y, point.z);
        }

        glEnd();
    }
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Restaurez les paramètres OpenGL par défaut si nécessaire
}