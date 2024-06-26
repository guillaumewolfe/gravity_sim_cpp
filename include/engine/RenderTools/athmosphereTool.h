#ifndef AthmosphereTOOL_H
#define AthmosphereTOOL_H

#include "engine/CelestialObject.h" // Assurez-vous d'inclure le bon fichier pour CelestialObject
#include <vector>
#include "engine/RenderTools/RenderContext.h"
#include "engine/Camera/Camera.h"



struct AthmosphereSphere {
    GLuint vboVertices, vboNormals, vboTexCoords;
    int vertexCount;
    float rayon; // Rayon de la sphère
    float alpha; // Transparence de la sphère
    glm::vec3 color; // Couleur de la sphère
    GLuint textureID; 
};

class AthmosphereTool {
public:
    // Constructeur
    AthmosphereTool(CelestialObject* celestialObject, RenderContext* renderContext);
    // Destructeur
    ~AthmosphereTool();
    // Méthodes pour gérer le Athmosphere
    void initAthmosphere();
    void drawAthmosphere(CelestialObject* Sun,Camera* camera);
    void initSphere(AthmosphereSphere& sphere, int lats, int longs, float rayon, GLuint textureID = 0);
    glm::vec3 getLightDirection(CelestialObject* Sun);

private:
    RenderContext* m_renderContext = nullptr;
    CelestialObject* m_celestialObject = nullptr; 
    int nbreSphere = 30;
    float sizeOfAthmosphere = 0.3;
    std::vector<AthmosphereSphere> AthmosphereSpheres= std::vector<AthmosphereSphere>(nbreSphere);
    



    //Shaders
    GLuint shaderProgram = 0;
    void initShaders();
    void checkCompileErrors(GLuint shader, std::string type);
    std::string readShaderFile(const std::string& shaderPath);
    bool fileExists(const std::string& path);


    //Clouds
    GLuint cloudTexture = 0;
    GLuint loadTexture(const char* filename);
    AthmosphereSphere cloudSphere;
    void drawClouds(Camera* camera);
    GLuint cloudShaderProgram  = 0;
    void initCloudsShaders();
    void updateLumiere(CelestialObject* object);
    float rotationOffset = 0;


};

#endif // AthmosphereTOOL_H