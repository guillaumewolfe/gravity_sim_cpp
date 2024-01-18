// UITool.h
#ifndef OBJECTSTOOL_H
#define OBJECTSTOOL_H

#include "RenderComponent.h"
#include "engine/Camera/Camera.h"
#include "engine/Vec3.h"
#include "engine/RenderTools/GlowTool.h"
#include "engine/RenderTools/athmosphereTool.h"
#include "engine/RenderTools/saturnRingTool.h"
#include "engine/Camera/Camera.h"

class ObjectsTool : public RenderComponent {
public:

    std::string readShaderFile(const std::string& shaderPath);


    explicit ObjectsTool(RenderContext* renderContext, Camera* camera);
    ~ObjectsTool() override;
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    static void initSphere(CelestialObject& object, int lats, int longs);
    void drawPlanets(CelestialObject* object);
    void drawStars(CelestialObject* object);
    void drawEffects();
    void drawPostTraitement();
    void updateLumiere(CelestialObject* object);
    bool fileExists(const std::string& path);
    void checkCompileErrors(GLuint shader, std::string type) ;

private:
    //SHADERS:
    GLuint shaderProgram;
    GLuint starShaderProgram;

    Camera* m_camera;

    GlowTool* glowTool;
    AthmosphereTool* athmoTool;
    SaturnRingTool* saturnRingTool;

    void initPlanetsShaders();
    void initStarShaders(); 
    void initFrameBuffer();

    int winWidth, winHeight;

    GLuint loadTexture(const char* filename); 
    GLuint attributeTexCoordLocation;


};

#endif // OBJECTSTOOL_H