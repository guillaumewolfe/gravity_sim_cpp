
#include <glm/glm.hpp> // Ceci inclut les types de vecteurs et de matrices de base.
#include <glm/gtc/matrix_transform.hpp> // Pour les transformations comme glm::translate, glm::rotate, glm::scale.
#include <glm/gtc/type_ptr.hpp> // Pour convertir les types de GLM en pointeurs pour OpenGL.
#include <vector>
#include "engine/CelestialObject.h"


struct Particle {
    glm::vec3 position;
    glm::vec3 velocity; // Vous pouvez ignorer la vitesse pour l'instant
    glm::vec4 color;
    bool active;
    // Ajoutez d'autres propriétés selon vos besoins
};

class ParticleSystem {
public:
    ParticleSystem(CelestialObject& object, unsigned int maxParticles);
    void update(float deltaTime);
    void draw(const glm::vec3& cameraPosition, const glm::vec3& cameraDirection);
    void initSystem();

private:
    float sizeOfSystem;
    CelestialObject& object;
    unsigned int maxParticles;
    std::vector<Particle> particles;
    glm::mat4 modelMatrix;

    //Shaders
    void initStarEffectShaders();
    std::string readShaderFile(const std::string& shaderPath);
    void checkCompileErrors(GLuint shader, std::string type) ;


    GLuint starEffectShaderProgram;
};