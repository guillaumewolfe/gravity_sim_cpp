#ifndef GLOWTOOL_H
#define GLOWTOOL_H

#include "engine/CelestialObject.h" // Assurez-vous d'inclure le bon fichier pour CelestialObject
#include <vector>
#include "engine/RenderTools/RenderContext.h"
#include "engine/Camera/Camera.h"



struct GlowSphere {
    GLuint vboVertices, vboNormals, vboTexCoords = 0;
    int vertexCount = 0;
    float rayon = 1; // Rayon de la sphère
    float alpha = 1; // Transparence de la sphère
    glm::vec3 color = glm::vec3(1,1,1); // Couleur de la sphère
};

class GlowTool {
public:
    // Constructeur
    GlowTool(CelestialObject* celestialObject, RenderContext* renderContext);
    // Destructeur
    ~GlowTool();
    // Méthodes pour gérer le glow
    void initGlow();
    void drawGlow(Camera* camera);
    void initSphere(GlowSphere& sphere, int lats, int longs, float rayon);

private:
    RenderContext* m_renderContext = nullptr;
    CelestialObject* m_celestialObject = nullptr; 
    int nbreSphere = 30;
    float sizeOfGlow = 0.4;
    std::vector<GlowSphere> glowSpheres;
    



    //Shaders
    GLuint shaderProgram = 0;
    void initShaders();
    void checkCompileErrors(GLuint shader, std::string type);
    std::string readShaderFile(const std::string& shaderPath);
    bool fileExists(const std::string& path);



};

#endif // GLOWTOOL_H