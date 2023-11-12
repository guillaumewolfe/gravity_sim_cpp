// UITool.h
#ifndef OBJECTSTOOL_H
#define OBJECTSTOOL_H

#include "RenderComponent.h"


class ObjectsTool : public RenderComponent {
public:

    explicit ObjectsTool(RenderContext* renderContext);
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void drawSphere(double r, int lats, int longs);
    void drawCube(float size);
    void iniShaders();
    std::string readShaderFile(const std::string& shaderPath);
    bool fileExists(const std::string& path);
    void checkCompileErrors(GLuint shader, std::string type) ;

private:
    GLuint shaderProgram;
    GLuint textureID;

    void drawBackground();
    GLuint loadTexture(const char* filename); 
    GLuint attributeTexCoordLocation;
};

#endif // OBJECTSTOOL_H