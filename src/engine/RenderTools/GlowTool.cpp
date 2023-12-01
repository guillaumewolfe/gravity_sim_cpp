#include <glad/glad.h> // Assurez-vous d'inclure les bibliothèques OpenGL nécessaires
#include "engine/RenderTools/GlowTool.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 
#include <glm/gtc/type_ptr.hpp> // Pour convertir les types de GLM en pointeurs pour OpenGL.


// Constructeur
GlowTool::GlowTool(CelestialObject* celestialObject, RenderContext* renderContext) : m_celestialObject(celestialObject),m_renderContext(renderContext) {
    initGlow();
    initShaders();
}

// Initialisation du glow
void GlowTool::initGlow() {
    if (m_celestialObject == nullptr) {
        return;
    }

    float rayonBase = m_celestialObject->getRayon();
    for (int i = 0; i < nbreSphere; ++i) {
        float progression = static_cast<float>(i) / (nbreSphere - 1);
        float rayonSphere = rayonBase * (1.0f + sizeOfGlow * progression);
        GlowSphere sphere;
        sphere.rayon = rayonSphere;

        // Utiliser une fonction exponentielle inversée pour l'alpha
        sphere.alpha =0.015* exp(-progression * 2); // Ajustez le facteur 5.0f selon vos besoins
        
        // Ajustement des couleurs
        glm::vec3 yellowColor = glm::vec3(1.0, 0.6, 0.2); // Jaune
        glm::vec3 whiteColor = glm::vec3(1.0, 1.0, 1.0); // Blanc
        float colorProgression = pow(progression, 0.5);
        sphere.color = glm::mix(whiteColor, yellowColor, colorProgression);

        initSphere(sphere, 50, 50, rayonSphere);
        glowSpheres.push_back(sphere);
    }
}

void GlowTool::drawGlow() {
    if (m_celestialObject == nullptr) {
        return;
    }

    // Activer le blending pour l'effet de transparence
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Désactivation de l'écriture de profondeur
    glDepthMask(GL_FALSE);

    // Utiliser le shader pour le glow
    glUseProgram(shaderProgram); // Assurez-vous d'avoir un shader pour le glow
    GLint alphaLoc = glGetUniformLocation(shaderProgram, "alpha");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "sphereColor");
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    m_renderContext->currentCamera->lookAt();
    glTranslatef(m_celestialObject->position_simulation.x, m_celestialObject->position_simulation.y, m_celestialObject->position_simulation.z);
    for (const auto& sphere : glowSpheres) {
        glUniform1f(alphaLoc, sphere.alpha);
        glUniform3fv(colorLoc, 1, glm::value_ptr(sphere.color));
        // Activer et lier le VBO des positions
        GLint posAttrib = glGetAttribLocation(shaderProgram, "vertexPosition");
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vboVertices);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(posAttrib);

        // Activer et lier le VBO des normales
        GLint normalAttrib = glGetAttribLocation(shaderProgram, "vertexNormal");
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vboNormals);
        glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(normalAttrib);

        // Dessiner la sphère
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sphere.vertexCount);

        // Nettoyage
        glDisableVertexAttribArray(posAttrib);
        glDisableVertexAttribArray(normalAttrib);
    }

    // Réactivation de l'écriture de profondeur
    glDepthMask(GL_TRUE);

    // Désactivation du blending
    glDisable(GL_BLEND);

    // Désactiver le shader
    glPopMatrix();
    glUseProgram(0);
}


void GlowTool::initSphere(GlowSphere& sphere, int lats, int longs, float rayon) {
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
        glGenBuffers(1, &sphere.vboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &sphere.vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vboNormals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    }

    // Création et remplissage des VBOs pour les vertices
    glGenBuffers(1, &sphere.vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, sphere.vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Répétez le processus pour les normales et les coordonnées de texture
    glGenBuffers(1, &sphere.vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, sphere.vboNormals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &sphere.vboTexCoords);
    glBindBuffer(GL_ARRAY_BUFFER, sphere.vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);

    // Stockez le nombre de vertices
    sphere.vertexCount = vertices.size() / 3; // Chaque vertex est composé de 3 floats
}



















void GlowTool::initShaders() {
    std::string vertexCode = readShaderFile("../src/engine/RenderTools/Shaders/vertex_star_effect.glsl");
    std::string fragmentCode = readShaderFile("../src/engine/RenderTools/Shaders/fragment_star_effect.glsl");
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





void GlowTool::checkCompileErrors(GLuint shader, std::string type) {
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
std::string GlowTool::readShaderFile(const std::string& shaderPath) {
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

bool GlowTool::fileExists(const std::string& path) {
    // Pour les versions antérieures de C++, vous pouvez essayer d'ouvrir le fichier
    std::ifstream file(path);
    return file.good();
}
