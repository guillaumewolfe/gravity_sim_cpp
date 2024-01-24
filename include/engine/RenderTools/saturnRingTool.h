#ifndef SaturnRingTool_H
#define SaturnRingTool_H

#include "engine/CelestialObject.h" // Assurez-vous d'inclure le bon fichier pour CelestialObject
#include <vector>
#include "engine/RenderTools/RenderContext.h"
#include "engine/Camera/Camera.h"



struct Particule{
    GLuint vboVertices, vboNormals, vboTexCoords = 0;
    int vertexCount = 0;
    float rayon = 1; // Rayon de la Particule
    float alpha = 0; // Transparence de la Particule
    glm::vec3 color = glm::vec3(1,1,1); // Couleur de la Particule
    GLuint textureID = 0; // ID de la texture de la Particule
    glm::vec3 position = glm::vec3(0,0,0); // Position de la Particule
};

class SaturnRingTool {
public:
    // Constructeur
    SaturnRingTool(CelestialObject* celestialObject, RenderContext* renderContext);
    // Destructeur
    ~SaturnRingTool();

    // Méthodes pour gérer le Athmosphere
    void initParticules();
    void Draw(Camera* camera);
    void initSphere(Particule& particule, int lats, int longs, float rayon);
    glm::vec3 getLightDirection(CelestialObject* Sun);

private:

    RenderContext* m_renderContext = nullptr;
    CelestialObject* m_celestialObject = nullptr; 
    std::vector<Particule> Particules = std::vector<Particule>();

    float rotation;
    



    //Shaders
    GLuint shaderProgram = 0;
    void initShaders();
    void checkCompileErrors(GLuint shader, std::string type);
    std::string readShaderFile(const std::string& shaderPath);
    bool fileExists(const std::string& path);


};

#endif // SaturnRingTool_H