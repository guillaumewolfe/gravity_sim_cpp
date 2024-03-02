#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/objectsTool.h"
#include <vector>
#include <cmath>
#include <string>       // Pour std::string
#include <fstream>      // Pour std::ifstream
#include <sstream>      // Pour std::stringstream
#include <iostream>  
#include <filesystem>
#include "opencv2/opencv.hpp"
#include <glm/glm.hpp> // Ceci inclut les types de vecteurs et de matrices de base.
#include <glm/gtc/matrix_transform.hpp> // Pour les transformations comme glm::translate, glm::rotate, glm::scale.
#include <glm/gtc/type_ptr.hpp> // Pour convertir les types de GLM en pointeurs pour OpenGL.
#include "engine/RenderTools/uranusRingTool.h"
#include "path_util.h"

ObjectsTool::ObjectsTool(RenderContext* renderContext, Camera* camera) : RenderComponent(renderContext),m_camera(camera) {
    initPlanetsShaders();
    initStarShaders();

    for (auto& object : m_renderContext->systemeSolaire->objects) {
        initSphere(*object, 200, 200); 

        if(object->getName()=="Sun"){
            glowTool = new GlowTool(object, m_renderContext);
            object->glowTool = glowTool;
        }else if(object->getName()=="Earth"){
            athmoTool = new AthmosphereTool(object, m_renderContext);
            object->athmosphereTool = athmoTool;
    }else if(object->getName()=="Saturn"){
            saturnRingTool = new SaturnRingTool(object, m_renderContext);
            object->saturnRingTool = saturnRingTool;
    }else if(object->getName()=="Uranus"){
            object->uranusRingTool = new UranusRingTool(object, m_renderContext);
            }
    }
}

// Destructeur
ObjectsTool::~ObjectsTool() {
    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        glDeleteBuffers(1, &object->vboVertices);
        glDeleteBuffers(1, &object->vboNormals);
        glDeleteBuffers(1, &object->vboTexCoords);
    }
    glDeleteProgram(shaderProgram);
    glDeleteProgram(starShaderProgram);
}




void ObjectsTool::Draw() {
    for (const auto& object : m_renderContext->systemeSolaire->objects) {
            //m_camera->updateObjectVisibility(object);
        }

    glEnable(GL_DEPTH_TEST);
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        if (object->shouldBeDrawn) {
        if(object->type==1){
            drawStars(object);
        }else if(object->type>1 || object->type==0){
        drawPlanets(object);
    }
    }}
    drawEffects();
    // Nettoyage*/
    glDisable(GL_DEPTH_TEST);
    //std::cout<<"-----"<<std::endl;
}


void ObjectsTool::drawStars(CelestialObject* object){
        glUseProgram(starShaderProgram);
        // Mise à jour des matrices de transformation
        glMatrixMode(GL_MODELVIEW);

        glPushMatrix();
        glLoadIdentity();
        m_camera->lookAt();
        glTranslatef(object->position_simulation.x, object->position_simulation.y, object->position_simulation.z);
        glRotatef(object->rotationSid,0,1,0);
        glBindTexture(GL_TEXTURE_2D, object->getTexture());

        // Liage des VBOs et activation des attributs de vertex
        glBindBuffer(GL_ARRAY_BUFFER, object->vboVertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, object->vboNormals);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, object->vboTexCoords);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);
        // Dessin de l'objet
        glDrawArrays(GL_TRIANGLE_STRIP, 0, object->vertexCount);
        // Désactivation des attributs de vertex
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glPopMatrix();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);

        //Q:Comment savoir si "glowTool" existe pour l'objet?
}


void ObjectsTool::drawPlanets(CelestialObject* object){
        glUseProgram(shaderProgram);
        // Mise à jour des matrices de transformation
        updateLumiere(object);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        m_camera->lookAt();
        glTranslatef(object->position_simulation.x, object->position_simulation.y, object->position_simulation.z);
        glRotatef(object->rotationSid,0,1,0);
        glBindTexture(GL_TEXTURE_2D, object->getTexture());

        // Liage des VBOs et activation des attributs de vertex
        glBindBuffer(GL_ARRAY_BUFFER, object->vboVertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, object->vboNormals);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, object->vboTexCoords);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);
        // Dessin de l'objet
        glDrawArrays(GL_TRIANGLE_STRIP, 0, object->vertexCount);
        // Désactivation des attributs de vertex
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glPopMatrix();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
}

void ObjectsTool::drawEffects(){
    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        if (object->shouldBeDrawn) {
        if(object->glowTool!=nullptr){
            object->glowTool->drawGlow(m_camera);
        }
        if(object->athmosphereTool!=nullptr){
            object->athmosphereTool->drawAthmosphere(m_renderContext->systemeSolaire->getSun(object), m_camera);
        }
        if(object->saturnRingTool!=nullptr){
            object->saturnRingTool->Draw(m_camera);
        }
        if(object->uranusRingTool!=nullptr){
            object->uranusRingTool->Draw(m_camera);
        }
    }
    }
}



void ObjectsTool::updateLumiere(CelestialObject* object){
    CelestialObject* sun = m_renderContext->systemeSolaire->getSun(object);
    bool isSunPresent = (sun != nullptr && sun->type == 1);
    CelestialObject* soleil = m_renderContext->systemeSolaire->getSun(object);
    GLint isThereSunUniform = glGetUniformLocation(shaderProgram, "isThereSun");
    glUniform1i(isThereSunUniform, isSunPresent ? GL_TRUE : GL_FALSE); 
    if(soleil==nullptr){return;}
    glm::vec3 positionSoleil = soleil->position_simulation.toGlm();
    glm::vec3 positionObjet = glm::vec3(object->position_simulation.x, object->position_simulation.y, object->position_simulation.z);
    glm::vec3 lightDir = glm::normalize(positionSoleil - positionObjet);

    // Calculer la rotation inverse
    float rotationInverse = -object->rotationSid; // Si rotationSid est en degrés
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationInverse), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Appliquer la rotation inverse à la direction de la lumière
    glm::vec3 rotatedLightDir = glm::mat3(rotationMatrix) * lightDir;

    // Passer la direction de la lumière ajustée au shader
    GLint lightDirUniform = glGetUniformLocation(shaderProgram, "lightDirection");
    glUniform3f(lightDirUniform, rotatedLightDir.x, rotatedLightDir.y, rotatedLightDir.z);

}





void ObjectsTool::initSphere(CelestialObject& object, int lats, int longs) {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texCoords;

            for (int i = 0; i <= lats; ++i) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double lat1 = M_PI * (-0.5 + (double) i / lats);

        for (int j = 0; j <= longs; ++j) {
            double lng = 2 * M_PI * (double) j / longs;
            double x = cos(lng);
            double y = sin(lng);

            // Ici, nous allons ajuster la manière dont z0, zr0, z1 et zr1 sont calculés
            double z0  = sin(lat0);
            double zr0 = cos(lat0);
            double z1 = sin(lat1);
            double zr1 = cos(lat1);

            double u = (double)j / longs;
            double v = (double)i / lats;

            // Normals and vertices for the first point
            normals.push_back(x * zr0);
            normals.push_back(z0);  // Échange y * zr0 avec z0
            normals.push_back(y * zr0);  // Échange z0 avec y * zr0
            vertices.push_back(x * zr0 * object.rayon_simulation);
            vertices.push_back(z0 * object.rayon_simulation);
            vertices.push_back(y * zr0 * object.rayon_simulation);
            texCoords.push_back(u);
            texCoords.push_back(v);

            // Normals and vertices for the second point
            normals.push_back(x * zr1);
            normals.push_back(z1);  // Échange y * zr1 avec z1
            normals.push_back(y * zr1);  // Échange z1 avec y * zr1
            vertices.push_back(x * zr1 * object.rayon_simulation);
            vertices.push_back(z1 * object.rayon_simulation);
            vertices.push_back(y * zr1 * object.rayon_simulation);
            texCoords.push_back(u);
            texCoords.push_back(v + 1.0f / lats);
        }
    }

        // Create and populate VBOs
        glGenBuffers(1, &object.vboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, object.vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &object.vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, object.vboNormals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &object.vboTexCoords);
        glBindBuffer(GL_ARRAY_BUFFER, object.vboTexCoords);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);

        object.vertexCount = vertices.size() / 3;
    }




void ObjectsTool::initPlanetsShaders() {
    
    std::string vertexCode = readShaderFile(getFullPath("../src/engine/RenderTools/Shaders/vertex_shader.glsl"));
    std::string fragmentCode = readShaderFile(getFullPath("../src/engine/RenderTools/Shaders/fragment_shader.glsl"));
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

void ObjectsTool::initStarShaders() {
    std::string vertexCode = readShaderFile(getFullPath("../src/engine/RenderTools/Shaders/vertex_star.glsl"));
    std::string fragmentCode = readShaderFile(getFullPath("../src/engine/RenderTools/Shaders/fragment_star.glsl"));
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
    starShaderProgram = glCreateProgram();
    glAttachShader(starShaderProgram, vertexShader);
    glAttachShader(starShaderProgram, fragmentShader);
    glLinkProgram(starShaderProgram);
    checkCompileErrors(starShaderProgram, "PROGRAM");

    //Delete Shaders 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}





void ObjectsTool::checkCompileErrors(GLuint shader, std::string type) {
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
std::string ObjectsTool::readShaderFile(const std::string& shaderPath) {
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

bool ObjectsTool::fileExists(const std::string& path) {
    // Pour les versions antérieures de C++, vous pouvez essayer d'ouvrir le fichier
    std::ifstream file(path);
    return file.good();
}

GLuint ObjectsTool::loadTexture(const char* filename) {
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




