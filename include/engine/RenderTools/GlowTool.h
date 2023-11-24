#ifndef GLOWTOOL_H
#define GLOWTOOL_H

#include "engine/CelestialObject.h" // Assurez-vous d'inclure le bon fichier pour CelestialObject
#include <vector>




struct GlowSphere {
    GLuint vboVertices, vboNormals, vboTexCoords;
    int vertexCount;
    float rayon; // Rayon de la sphère
    float alpha; // Transparence de la sphère
    glm::vec3 color; // Couleur de la sphère
};

class GlowTool {
public:
    // Constructeur
    GlowTool(CelestialObject* celestialObject);

    // Méthodes pour gérer le glow
    void initGlow();
    void drawGlow();
    void initSphere(GlowSphere& sphere, int lats, int longs, float rayon);

private:
    CelestialObject* m_celestialObject; 
    int nbreSphere = 250;
    float sizeOfGlow = 6.00;
    std::vector<GlowSphere> glowSpheres;
    



    //Shaders
    GLuint shaderProgram;
    void initShaders();
    void checkCompileErrors(GLuint shader, std::string type);
    std::string readShaderFile(const std::string& shaderPath);
    bool fileExists(const std::string& path);



};

#endif // GLOWTOOL_H