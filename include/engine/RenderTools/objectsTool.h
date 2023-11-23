// UITool.h
#ifndef OBJECTSTOOL_H
#define OBJECTSTOOL_H

#include "RenderComponent.h"
#include "engine/Camera/Camera.h"
#include "engine/Vec3.h"
#include "engine/RenderTools/particules.h"

class ObjectsTool : public RenderComponent {
public:

    std::string readShaderFile(const std::string& shaderPath);
    float myVariable;


    explicit ObjectsTool(RenderContext* renderContext);
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void initSphere(CelestialObject& object, int lats, int longs);
    void drawPlanets(CelestialObject* object);
    void drawStars(CelestialObject* object);
    void updateLumiere(CelestialObject* object);
    void initPlanetsShaders();
    void initStarShaders(); 
    void initFrameBuffer();
    bool fileExists(const std::string& path);
    void checkCompileErrors(GLuint shader, std::string type) ;

private:
    GLuint shaderProgram;
    GLuint starShaderProgram;

    GLuint framebuffer, texColorBuffer, rbo;


    GLuint loadTexture(const char* filename); 
    GLuint attributeTexCoordLocation;
};

#endif // OBJECTSTOOL_H