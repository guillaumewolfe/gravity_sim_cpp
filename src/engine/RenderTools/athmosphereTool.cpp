#include <glad/glad.h> // Assurez-vous d'inclure les bibliothèques OpenGL nécessaires
#include "engine/RenderTools/athmosphereTool.h"
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream> 
#include <glm/gtc/type_ptr.hpp> // Pour convertir les types de GLM en pointeurs pour OpenGL.
#include "opencv2/opencv.hpp"

// Constructeur
AthmosphereTool::AthmosphereTool(CelestialObject* celestialObject, RenderContext* renderContext) : m_celestialObject(celestialObject), m_renderContext(renderContext) {
    initAthmosphere();
    initShaders();
    initCloudsShaders();
}

//destructeur
AthmosphereTool::~AthmosphereTool() {
    for (const auto& sphere : AthmosphereSpheres) {
        glDeleteBuffers(1, &sphere.vboVertices);
        glDeleteBuffers(1, &sphere.vboNormals);
        glDeleteBuffers(1, &sphere.vboTexCoords);
    }
    glDeleteProgram(shaderProgram);
    glDeleteProgram(cloudShaderProgram);
}

// Initialisation du Athmosphere
void AthmosphereTool::initAthmosphere() {
    if (m_celestialObject == nullptr) {
        return;
    }

    float rayonBase = m_celestialObject->getRayon();
    for (int i = 0; i < nbreSphere; ++i) {
        float progression = static_cast<float>(i) / (nbreSphere - 1);
        float rayonSphere = rayonBase * (1.0f + sizeOfAthmosphere * progression);
        AthmosphereSphere sphere;
        sphere.rayon = rayonSphere;

        // Utiliser une fonction exponentielle inversée pour l'alpha
        sphere.alpha =0.1* exp(-progression * 6); // Ajustez le facteur 5.0f selon vos besoins
        
        // Ajustement des couleurs
        glm::vec3 yellowColor = glm::vec3(1.0, 1.0, 0.2); // Jaune
        glm::vec3 whiteColor = glm::vec3(1.0, 1.0, 1.0); // Blanc
        float colorProgression = pow(progression, 0.5);
        sphere.color = glm::mix(whiteColor, yellowColor, colorProgression);

        initSphere(sphere, 50, 50, rayonSphere);
        AthmosphereSpheres.push_back(sphere);
    }
    //Init Clouds
    cloudSphere.textureID = loadTexture("../assets/textures/clouds.jpg");
    float rayonClouds = m_celestialObject->getRayon() * (1.0f + 0.01); // Définir le rayon
    cloudSphere.rayon = rayonClouds;
    initSphere(cloudSphere, 150, 150, rayonClouds, cloudSphere.textureID);
}

void AthmosphereTool::drawAthmosphere(CelestialObject* Sun, Camera* camera) {
    if (m_celestialObject == nullptr || !(m_celestialObject->shouldBeDrawn)) {
        return;
    }

    // Activer le blending pour l'effet de transparence
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Désactivation de l'écriture de profondeur
    glDepthMask(GL_FALSE);

    // Utiliser le shader pour le Athmosphere
    glUseProgram(shaderProgram); // Assurez-vous d'avoir un shader pour le Athmosphere
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();
    camera->lookAt();
    glTranslatef(m_celestialObject->position_simulation.x, m_celestialObject->position_simulation.y, m_celestialObject->position_simulation.z);
    GLint alphaLoc = glGetUniformLocation(shaderProgram, "alpha");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "sphereColor");
    GLint lightDirUniform = glGetUniformLocation(shaderProgram, "lightDirection");

    glm::vec3 lightDir = getLightDirection(Sun);
    glUniform3f(lightDirUniform, lightDir.x, lightDir.y, lightDir.z);


    for (const auto& sphere : AthmosphereSpheres) {
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
    drawClouds(camera);
}

glm::vec3 AthmosphereTool::getLightDirection(CelestialObject* Sun){
    glm::vec3 positionSoleil = Sun->getPositionSimulation().toGlm();
    glm::vec3 positionObjet = glm::vec3(m_celestialObject->position_simulation.x, m_celestialObject->position_simulation.y, m_celestialObject->position_simulation.z);
    glm::vec3 lightDir = glm::normalize(positionSoleil - positionObjet);
    return lightDir;

}


void AthmosphereTool::drawClouds(Camera* camera){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUseProgram(cloudShaderProgram);
        // Mise à jour des matrices de transformation
        updateLumiere(m_celestialObject);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        camera->lookAt();
        glTranslatef(m_celestialObject->position_simulation.x, m_celestialObject->position_simulation.y, m_celestialObject->position_simulation.z);
        glRotatef(m_celestialObject->rotationSid,0,1,0);
        glRotatef(rotationOffset,0,1,0);
        rotationOffset += 0.001;
        glBindTexture(GL_TEXTURE_2D, cloudSphere.textureID);

        // Liage des VBOs et activation des attributs de vertex
        glBindBuffer(GL_ARRAY_BUFFER, cloudSphere.vboVertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, cloudSphere.vboNormals);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, cloudSphere.vboTexCoords);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);
        // Dessin de l'objet
        glDrawArrays(GL_TRIANGLE_STRIP, 0, cloudSphere.vertexCount);
        // Désactivation des attributs de vertex
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glPopMatrix();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
        glUseProgram(0);
        
}




void AthmosphereTool::updateLumiere(CelestialObject* object){
    glm::vec3 positionSoleil = glm::vec3(0,0,0);
    glm::vec3 positionObjet = glm::vec3(object->position_simulation.x, object->position_simulation.y, object->position_simulation.z);
    glm::vec3 lightDir = glm::normalize(positionSoleil - positionObjet);

    // Calculer la rotation inverse
    float rotationInverse = -object->rotationSid-rotationOffset; // Si rotationSid est en degrés
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationInverse), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Appliquer la rotation inverse à la direction de la lumière
    glm::vec3 rotatedLightDir = glm::mat3(rotationMatrix) * lightDir;

    // Passer la direction de la lumière ajustée au shader
    GLint lightDirUniform = glGetUniformLocation(shaderProgram, "lightDirection");
    glUniform3f(lightDirUniform, rotatedLightDir.x, rotatedLightDir.y, rotatedLightDir.z);
}




void AthmosphereTool::initSphere(AthmosphereSphere& sphere, int lats, int longs, float rayon, GLuint textureID) {
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

void AthmosphereTool::initCloudsShaders(){
    std::string vertexCode = readShaderFile("../src/engine/RenderTools/Shaders/vertex_clouds.glsl");
    std::string fragmentCode = readShaderFile("../src/engine/RenderTools/Shaders/frag_clouds.glsl");
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
    cloudShaderProgram = glCreateProgram();
    glAttachShader(cloudShaderProgram, vertexShader);
    glAttachShader(cloudShaderProgram, fragmentShader);
    glLinkProgram(cloudShaderProgram);
    checkCompileErrors(cloudShaderProgram, "PROGRAM");

    //Delete Shaders 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void AthmosphereTool::initShaders() {
    std::string vertexCode = readShaderFile("../src/engine/RenderTools/Shaders/vertex_athmosphere.glsl");
    std::string fragmentCode = readShaderFile("../src/engine/RenderTools/Shaders/frag_athmosphere.glsl");
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





void AthmosphereTool::checkCompileErrors(GLuint shader, std::string type) {
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
std::string AthmosphereTool::readShaderFile(const std::string& shaderPath) {
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

bool AthmosphereTool::fileExists(const std::string& path) {
    // Pour les versions antérieures de C++, vous pouvez essayer d'ouvrir le fichier
    std::ifstream file(path);
    return file.good();
}


//Clouds
GLuint AthmosphereTool::loadTexture(const char* filename) {
    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        std::cerr << "Erreur: Image non trouvée " << filename << std::endl;
        return 0;
    }
    cv::flip(image, image, 0); // Inversion verticale de l'image

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}


