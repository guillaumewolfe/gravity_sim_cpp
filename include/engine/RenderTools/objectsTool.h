// UITool.h
#ifndef OBJECTSTOOL_H
#define OBJECTSTOOL_H

#include "RenderComponent.h"
#include "engine/Camera/Camera.h"
#include "engine/Vec3.h"

class ObjectsTool : public RenderComponent {
public:

    std::string readShaderFile(const std::string& shaderPath);
    float myVariable;


    explicit ObjectsTool(RenderContext* renderContext);
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void drawSphere(double r, int lats, int longs);
    void iniShaders();
    bool fileExists(const std::string& path);
    void checkCompileErrors(GLuint shader, std::string type) ;

private:
    GLuint shaderProgram;
    GLuint textureID;

    std::vector<GLfloat> starPositions;
    std::vector<GLfloat> starSizes;
    GLuint loadTexture(const char* filename); 
    GLuint attributeTexCoordLocation;
};

#endif // OBJECTSTOOL_H