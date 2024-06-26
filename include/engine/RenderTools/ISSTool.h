// UITool.h
#ifndef ISSTool_H
#define ISSTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"

struct Face {
    std::vector<unsigned int> indices; // Peut être std::vector<int> selon votre choix
};
struct Vertex {
    glm::vec3 position;
    // Vous pouvez ajouter d'autres attributs ici, comme les normales, les coordonnées de texture, etc.
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class ISSTool : public RenderComponent {
public:
    float lineLength = 0;

    explicit ISSTool(RenderContext* renderContext);
    void Draw() override;

    Vec3 getPosition() const;
    void setPosition(Vec3 pos);
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<unsigned int> indices; // Pour stocker les indices des faces
    GLuint VAO, VBO;
    GLuint vboVertices, ebo;
    GLuint vertexCount;
    
private:
    Vec3 position;
    Camera* camera;

    void loadOBJ();
    void loadMTL(const std::string& mtlPath);
    Material material;  // The object's material
    std::map<std::string, Material> materials; 
};

#endif // ISSTool_H