#ifndef AthmosphereTOOL_H
#define AthmosphereTOOL_H

#include "engine/CelestialObject.h" // Assurez-vous d'inclure le bon fichier pour CelestialObject
#include <vector>
#include "engine/RenderTools/RenderContext.h"




struct AthmosphereSphere {
    GLuint vboVertices, vboNormals, vboTexCoords;
    int vertexCount;
    float rayon; // Rayon de la sphère
    float alpha; // Transparence de la sphère
    glm::vec3 color; // Couleur de la sphère
};

class AthmosphereTool {
public:
    // Constructeur
    AthmosphereTool(CelestialObject* celestialObject, RenderContext* renderContext);

    // Méthodes pour gérer le Athmosphere
    void initAthmosphere();
    void drawAthmosphere(CelestialObject* Sun);
    void initSphere(AthmosphereSphere& sphere, int lats, int longs, float rayon);
    glm::vec3 getLightDirection(CelestialObject* Sun);

private:
    RenderContext* m_renderContext;
    CelestialObject* m_celestialObject; 
    int nbreSphere = 50;
    float sizeOfAthmosphere = 0.25;
    std::vector<AthmosphereSphere> AthmosphereSpheres;
    



    //Shaders
    GLuint shaderProgram;
    void initShaders();
    void checkCompileErrors(GLuint shader, std::string type);
    std::string readShaderFile(const std::string& shaderPath);
    bool fileExists(const std::string& path);



};

#endif // AthmosphereTOOL_H