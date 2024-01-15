#ifndef UranusRingTool_H
#define UranusRingTool_H

#include "engine/CelestialObject.h" // Assurez-vous d'inclure le bon fichier pour CelestialObject
#include <vector>
#include "engine/RenderTools/RenderContext.h"
#include "engine/Camera/Camera.h"



struct ParticuleUranus{
    GLuint vboVertices, vboNormals, vboTexCoords;
    int vertexCount;
    float rayon; // Rayon de la ParticuleUranus
    float alpha; // Transparence de la ParticuleUranus
    glm::vec3 color; // Couleur de la ParticuleUranus
    GLuint textureID; 
    glm::vec3 position; // Position de la ParticuleUranus
};

class UranusRingTool {
public:
    // Constructeur
    UranusRingTool(CelestialObject* celestialObject, RenderContext* renderContext);
    // Destructeur
    ~UranusRingTool();

    // Méthodes pour gérer le Athmosphere
    void initParticules();
    void Draw(Camera* camera);
    void initSphere(ParticuleUranus& ParticuleUranus, int lats, int longs, float rayon);
    glm::vec3 getLightDirection(CelestialObject* Sun);

private:

    RenderContext* m_renderContext;
    CelestialObject* m_celestialObject; 
    std::vector<ParticuleUranus> Particules;

    float rotation;
    



    //Shaders
    GLuint shaderProgram;
    void initShaders();
    void checkCompileErrors(GLuint shader, std::string type);
    std::string readShaderFile(const std::string& shaderPath);
    bool fileExists(const std::string& path);


};

#endif // UranusRingTool_H