#include <glad/glad.h> // Assurez-vous d'inclure les bibliothèques OpenGL nécessaires
#include "engine/RenderTools/uranusRingTool.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 
#include <glm/gtc/type_ptr.hpp> // Pour convertir les types de GLM en pointeurs pour OpenGL.
#include "opencv2/opencv.hpp"
#include "path_util.h"


// Constructeur
UranusRingTool::UranusRingTool(CelestialObject* celestialObject, RenderContext* renderContext) : m_celestialObject(celestialObject), m_renderContext(renderContext) {
    initParticules();
    initShaders();
}

// Destructeur
UranusRingTool::~UranusRingTool() {
    for (const auto& particule : Particules) {
        glDeleteBuffers(1, &particule.vboVertices);
        glDeleteBuffers(1, &particule.vboNormals);
        glDeleteBuffers(1, &particule.vboTexCoords);
    }
    glDeleteProgram(shaderProgram);
}

void UranusRingTool::initParticules() {
    if (m_celestialObject == nullptr) {
        return;
    }

    // Adjusted Uranus' ring parameters
    float uranusRadius = m_celestialObject->getRayon(); // Radius of Uranus
    float innerRadius = uranusRadius * 2.0f; // Approximate inner radius of the epsilon ring
    float outerRadius = innerRadius + (uranusRadius * 0.075f); // Outer radius, epsilon ring width is approx. 7.5% of Uranus radius
    int nbreParticules = 8000; // Adjusted number of particles
    float heightVariation = uranusRadius * 0.002f; // Height variation, arbitrary small fraction of Uranus radius for visual effect

    // Particle generation
    for (int i = 0; i < nbreParticules; i++) {
        float radius = innerRadius + static_cast<float>(rand()) / RAND_MAX * (outerRadius - innerRadius);

        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
        float heightOffset = heightVariation * (static_cast<float>(rand()) / RAND_MAX - 0.5f);

        glm::vec3 position = glm::vec3(radius * cos(angle), heightOffset, radius * sin(angle));

        ParticuleUranus particule;
        // initSphere(particule, 7, 7, 0.0003f); // Initialize the sphere as needed
        particule.position = position;
        
        // Color definition (can be adjusted to represent the appearance of Uranus's epsilon ring)
        particule.color = glm::vec3(0.6f, 0.7f, 0.8f); // Example color for the epsilon ring

        particule.alpha = 1.0; // Transparency

        // Add the particle to the collection
        Particules.push_back(particule);
    }
}





void UranusRingTool::Draw(Camera* camera) {
    if (m_celestialObject == nullptr || !(m_celestialObject->shouldBeDrawn) || !(m_renderContext->currentCamera->followedObject==m_celestialObject)) {
        return;
    }

    // Activer le blending pour l'effet de transparence
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Désactivation de l'écriture de profondeur
    glDepthMask(GL_FALSE);

    // Réinitialiser et configurer la matrice de modèle-vue
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    camera->lookAt();
    glTranslatef(m_celestialObject->position_simulation.x, m_celestialObject->position_simulation.y, m_celestialObject->position_simulation.z);

    // Configurer les rotations
    glRotatef(98, 1.0f, 0.0f, 0.0f); // Inclinaison
    glRotatef(m_celestialObject->rotationSid / 10, 0.0f, 1.0f, 0.0f); // Rotation

    float distanceToCamera = glm::length(m_renderContext->currentCamera->position.toGlm() - m_celestialObject->position_simulation.toGlm());
    float maxDistance = 9.5f; // Distance maximale
    float minPointSize = 0.0f; // Taille minimale des points
    float maxPointSize = 1.0f; // Taille maximale des points

    // Utiliser une fonction de puissance pour un changement plus rapide vers la fin
    float normalizedDistance =1- distanceToCamera / maxDistance; // Normaliser la distance entre 0 et 1
    float factor = pow(normalizedDistance, 4); // Élever au carré pour un effet plus rapide vers la fin
    if(distanceToCamera>maxDistance){factor=0.0f;}
    // Calculer la taille des points
    float pointSize = minPointSize + (maxPointSize - minPointSize) * (1.0f - factor);
    glPointSize(1);


glBegin(GL_POINTS);
for (const auto& particule : Particules) {

    glColor4f(particule.color.r*factor,
              particule.color.g*factor,
              particule.color.b*factor,
              particule.alpha*0.7*factor);

    glVertex3f(particule.position.x, particule.position.y, particule.position.z);
}
glEnd();
    glPointSize(1.0f); // Rétablir la taille par défaut
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Rétablir la couleur par défaut

    // Rétablir l'état d'origine
    glPopMatrix();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}





void UranusRingTool::initSphere(ParticuleUranus& particule, int lats, int longs, float rayon) {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

    for (int i = 0; i <= lats; ++i) {
        float lat0 = M_PI * (-0.5f + (float) (i - 1) / lats);
        float z0  = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5f + (float) i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        for (int j = 0; j <= longs; ++j) {
            float lng = 2 * M_PI * (float) j / longs;
            float x = cos(lng);
            float y = sin(lng);

            // Coordonnées des vertices et des normales pour les deux points
            normals.push_back(x * zr0); vertices.push_back(x * zr0 * rayon);
            normals.push_back(y * zr0); vertices.push_back(y * zr0 * rayon);
            normals.push_back(z0);      vertices.push_back(z0 * rayon);

            normals.push_back(x * zr1); vertices.push_back(x * zr1 * rayon);
            normals.push_back(y * zr1); vertices.push_back(y * zr1 * rayon);
            normals.push_back(z1);      vertices.push_back(z1 * rayon);

            // Coordonnées de texture (peuvent être omises si non utilisées)
            texCoords.push_back((float)j / longs);
            texCoords.push_back((float)i / lats);
            texCoords.push_back((float)j / longs);
            texCoords.push_back((float)(i + 1) / lats);
        }
        glGenBuffers(1, &particule.vboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, particule.vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &particule.vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, particule.vboNormals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    }

    // Création et remplissage des VBOs pour les vertices
    glGenBuffers(1, &particule.vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, particule.vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Répétez le processus pour les normales et les coordonnées de texture
    glGenBuffers(1, &particule.vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, particule.vboNormals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &particule.vboTexCoords);
    glBindBuffer(GL_ARRAY_BUFFER, particule.vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);

    // Stockez le nombre de vertices
    particule.vertexCount = vertices.size() / 3; // Chaque vertex est composé de 3 floats
}



void UranusRingTool::initShaders() {
    std::string vertexCode = readShaderFile(getFullPath("../src/engine/RenderTools/Shaders/vertex_saturn.glsl"));
    std::string fragmentCode = readShaderFile(getFullPath("../src/engine/RenderTools/Shaders/frag_saturn.glsl"));
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
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    //Delete Shaders 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}





void UranusRingTool::checkCompileErrors(GLuint shader, std::string type) {
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
std::string UranusRingTool::readShaderFile(const std::string& shaderPath) {
    if (!fileExists(shaderPath)) {
        std::cerr << "ERREUR: Le fichier " << shaderPath << " n'existe pas." << std::endl;
        return "";
    }

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

bool UranusRingTool::fileExists(const std::string& path) {
    // Pour les versions antérieures de C++, vous pouvez essayer d'ouvrir le fichier
    std::ifstream file(path);
    return file.good();
}





