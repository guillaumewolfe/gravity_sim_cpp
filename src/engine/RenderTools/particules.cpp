
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/particules.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream>  
#include "path_util.h"


ParticleSystem::ParticleSystem(CelestialObject& object, unsigned int maxParticles) 
    : object(object), maxParticles(maxParticles) {
        particles.resize(maxParticles);
        sizeOfSystem = 3.0f;
        initSystem();

        }
// Desctructeur
ParticleSystem::~ParticleSystem() {
    glDeleteProgram(starEffectShaderProgram);
}


void ParticleSystem::update(float deltaTime) {
    // Pour l'instant, laissez cette méthode vide
}


void ParticleSystem::draw(const glm::vec3& cameraPosition, const glm::vec3& cameraDirection) {
    // Calculez le vecteur "haut" de la caméra (si pas déjà fourni)
    glm::vec3 up = glm::vec3(0, 1, 0); // ou une autre valeur selon votre système de coordonnées

    // Calculez la matrice de rotation qui aligne les particules avec le plan de la caméra
    glm::mat4 rotationMatrix = glm::inverse(glm::lookAt(glm::vec3(0.0f), cameraDirection, up));

    glUseProgram(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

glm::vec3 centerPosition = glm::vec3(object.position_simulation.x, object.position_simulation.y, object.position_simulation.z);
    float maxGlowDistance = object.getRayon() * (1+sizeOfSystem); // Ajustez selon vos besoins
    glPointSize(1.0f);
    glBegin(GL_POINTS);

    for (const auto& particle : particles) {
        if (particle.active) {
            // Calcul de la position mondiale de la particule
            glm::vec4 rotatedPosition = rotationMatrix * glm::vec4(particle.position, 1.0f);
            glm::vec4 worldPosition = modelMatrix * rotatedPosition;

            // Calcul de la distance et de l'alpha
            float distance = glm::length(glm::vec3(worldPosition) - centerPosition);
            float alpha = 0.7f - glm::clamp(distance / maxGlowDistance, 0.0f, 0.7f);

            // Définir la couleur et l'alpha
            glColor4f(1.0f, 1.0f, 0.9f, alpha); // Blanc avec l'alpha calculé

            // Dessiner la particule
            glVertex3f(worldPosition.x, worldPosition.y, worldPosition.z);
        }
    }
    glEnd();
    glPointSize(1.0f);
    glColor4f(1,1,1,1);

    glDisable(GL_BLEND);
    glUseProgram(0);
}






void ParticleSystem::initSystem() {
    glm::vec3 soleilPosition = glm::vec3(object.getPositionSimulation().x,object.getPositionSimulation().y,object.getPositionSimulation().z);
    modelMatrix = glm::translate(glm::mat4(1.0f), soleilPosition);
    float rayonSoleil = object.getRayon();
    float rayonMin = rayonSoleil;
    float rayonMax = rayonSoleil * (1.0f + sizeOfSystem); // 110% du rayon

    for (auto& particle : particles) {
        // Générer un angle aléatoire
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;

        // Générer un rayon en utilisant une distribution qui favorise les distances plus courtes
        float distanceFactor = static_cast<float>(rand()) / RAND_MAX; // Valeur entre 0 et 1
        distanceFactor = pow(distanceFactor, 2); // Augmente la probabilité d'être proche du centre (ajustez la puissance selon vos besoins)
        float rayonParticule = rayonMin + distanceFactor * (rayonMax - rayonMin);

        // Convertir les coordonnées polaires en coordonnées cartésiennes
        particle.position.x = soleilPosition.x + rayonParticule * cos(angle);
        particle.position.y = soleilPosition.y + rayonParticule * sin(angle);
        particle.position.z = soleilPosition.z; // Pour un effet 2D, sinon générer un angle pour Z

        particle.active = true;
        // Définir d'autres propriétés de la particule si nécessaire
    }
}





















void ParticleSystem::initStarEffectShaders() {
    std::string vertexCode = readShaderFile(getFullPath("../src/engine/RenderTools/Shaders/vertex_star_effect.glsl"));
    std::string fragmentCode = readShaderFile(getFullPath("../src/engine/RenderTools/Shaders/fragment_star_effect.glsl"));
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    // Compiler le vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Compiler le fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Créer le programme shader
    starEffectShaderProgram = glCreateProgram();
    glAttachShader(starEffectShaderProgram, vertexShader);
    glAttachShader(starEffectShaderProgram, fragmentShader);
    glLinkProgram(starEffectShaderProgram);
    checkCompileErrors(starEffectShaderProgram, "PROGRAM");

    //Delete Shaders 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}



std::string ParticleSystem::readShaderFile(const std::string& shaderPath) {

    std::ifstream shaderFile(shaderPath);
    std::stringstream shaderStream;

    if (shaderFile) {
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    } else {
        std::cerr << "ERREUR: Impossible d'ouvrir le fichier " << shaderPath << std::endl;
        return "";
    }
}


void ParticleSystem::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERREUR::PROGRAM_LINKING_ERROR de type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERREUR::SHADER_COMPILATION_ERROR de type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}