#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/orbitTool.h"


OrbitTool::OrbitTool(RenderContext* renderContext) : RenderComponent(renderContext){   
    init();
}


void OrbitTool::init(){
    typeDict = m_renderContext->colorByTypeDict;
}


void OrbitTool::Draw() {
    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        if(!object->showOrbit){continue;}
        if (object->getName() == "Sun") { continue; } // Ne dessinez pas l'orbite du soleil

        CelestialObject* influentialObject = object->getMostInfluentialObject();
        if (influentialObject && influentialObject->getName() == "Sun") {
            // Calculez la distance actuelle entre l'objet et le soleil pour déterminer le rayon de l'orbite
            Vec3 orbitCenter = influentialObject->getPositionSimulation();
            double rayonOrbite = (object->getPositionSimulation() - orbitCenter).norm();

            // Dessinez un cercle représentant l'orbite de l'objet autour du soleil
            DrawEllipse(orbitCenter, rayonOrbite, rayonOrbite, Vec3(0, 0, 1), 0, object); // Utiliser le rayon de l'orbite pour a et b
        }
    }
}




void OrbitTool::DrawEllipse(const Vec3& center, float a, float b, const Vec3& axis, float angle, CelestialObject* object) {
    const int numSegments = 100; // Plus ce nombre est élevé, plus l'ellipse sera lisse
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(axis.x, axis.y, axis.z));
    glLineWidth(5.0f);  // Définissez l'épaisseur de la ligne selon vos besoins
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < numSegments; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);

        // Coordonnées x et z de l'ellipse avant rotation
        float x = a * cos(theta);
        float z = b * sin(theta); // Utilisez z au lieu de y pour aligner avec le plan XZ
        glm::vec4 point = rotationMatrix * glm::vec4(x, 0.0f, z, 1.0f); // Y est maintenant 0

        ImVec4 color = getTypeColor(object->type);
        glColor4f(color.x/255,color.y/255,color.z/255, 0.2f);
        glVertex3f(center.x + point.x, center.y, center.z + point.z); // Utilisez center.y pour la coordonnée Y
    }

    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

ImVec4 OrbitTool::getTypeColor(int type) {
    return typeDict[type].second;
}


