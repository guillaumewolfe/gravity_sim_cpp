#ifndef SaturnRingTool_H
#define SaturnRingTool_H

#include "engine/CelestialObject.h" // Assurez-vous d'inclure le bon fichier pour CelestialObject
#include <vector>
#include "engine/RenderTools/RenderContext.h"
#include "engine/Camera/Camera.h"



struct Particule{
    GLuint vboVertices, vboNormals, vboTexCoords;
    int vertexCount;
    float rayon; // Rayon de la Particule
    float alpha; // Transparence de la Particule
    glm::vec3 color; // Couleur de la Particule
    GLuint textureID; 
    glm::vec3 position; // Position de la Particule
};

class SaturnRingTool {
public:
    // Constructeur
    SaturnRingTool(CelestialObject* celestialObject, RenderContext* renderContext);

    // Méthodes pour gérer le Athmosphere
    void initParticules();
    void Draw(Camera* camera);
    void initSphere(Particule& particule, int lats, int longs, float rayon);
    glm::vec3 getLightDirection(CelestialObject* Sun);

private:

    RenderContext* m_renderContext;
    CelestialObject* m_celestialObject; 
    std::vector<Particule> Particules;

    float rotation;
    



    //Shaders
    GLuint shaderProgram;
    void initShaders();
    void checkCompileErrors(GLuint shader, std::string type);
    std::string readShaderFile(const std::string& shaderPath);
    bool fileExists(const std::string& path);


};

#endif // SaturnRingTool_H