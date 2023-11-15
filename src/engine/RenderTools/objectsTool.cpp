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



ObjectsTool::ObjectsTool(RenderContext* renderContext) : RenderComponent(renderContext){
    iniShaders();
    textureID = m_renderContext->systemeSolaire->objects[0]->textureID;
}



void ObjectsTool::Draw() {
    glEnable(GL_DEPTH_TEST);
    glUseProgram(shaderProgram);

    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        glLoadIdentity();
        m_renderContext->currentCamera->lookAt();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,object->getTexture());
        glPushMatrix();
        //if(object->getName()=="Terre"){std::cout<<object->position_simulation.print()<<std::endl;}
        glTranslatef(object->position_simulation.x,object->position_simulation.y,object->position_simulation.z);
        glRotatef(-90,1,0,0);
        glRotatef(object->inclinaison,1,0,0);
        glRotatef(object->rotationSid,0,0,1);
        drawSphere(object->rayon_simulation, 20, 20);
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
    // Nettoyage
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
}

void ObjectsTool::updateLumiere(){
        // Position de la lumière
    GLint lightDirUniform = glGetUniformLocation(shaderProgram, "lightPosition");
    float time = *(m_renderContext->simulationTime);
    glUniform3f(lightDirUniform, 0, -100, 0); //Position
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


void ObjectsTool::drawSphere(double r, int lats, int longs) {
    bool drawPoint = false;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

    for (int i = 0; i <= lats; ++i) {
        for (int j = 0; j <= longs; ++j) {
            double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
            double z0  = sin(lat0);
            double zr0 = cos(lat0);

            double lat1 = M_PI * (-0.5 + (double) i / lats);
            double z1 = sin(lat1);
            double zr1 = cos(lat1);

            double lng = 2 * M_PI * (double) j / longs;
            double x = cos(lng);
            double y = sin(lng);

            double u = (double)j / longs;
            double v = (double)i / lats;

            // Normals and vertices for the first point
            normals.push_back(x * zr0);
            normals.push_back(y * zr0);
            normals.push_back(z0);
            vertices.push_back(x * zr0 * r);
            vertices.push_back(y * zr0 * r);
            vertices.push_back(z0 * r);
            texCoords.push_back(u);
            texCoords.push_back(v);

            // Normals and vertices for the second point
            normals.push_back(x * zr1);
            normals.push_back(y * zr1);
            normals.push_back(z1);
            vertices.push_back(x * zr1 * r);
            vertices.push_back(y * zr1 * r);
            vertices.push_back(z1 * r);
            texCoords.push_back(u);
            texCoords.push_back(v + 1.0f / lats);
        }
    }

    // Create and populate VBOs
    GLuint vboVertices, vboNormals, vboTexCoords;
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboNormals);
    glGenBuffers(1, &vboTexCoords);

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glVertexAttribPointer(attributeTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attributeTexCoordLocation);

    // Draw the sphere
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);

    // Cleanup
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(attributeTexCoordLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboNormals);
    glDeleteBuffers(1, &vboTexCoords);

    if(drawPoint){
    // Dessiner les points
    glUseProgram(0);
    glPointSize(5.0f); // Taille des points
    glColor3f(1.0f, 0.0f, 0.0f); // Couleur rouge pour les points
    glBegin(GL_POINTS);
    for (int i = 0; i <= lats; i++) {
        for (int j = 0; j <= longs; j++) {
            double lat = M_PI * (-0.5 + (double) i / lats);
            double lng = 2 * M_PI * (double) j / longs;
            double x = r * cos(lng) * cos(lat);
            double y = r * sin(lng) * cos(lat);
            double z = r * sin(lat);

            glVertex3f(x, y, z);
        }
    }
    glEnd();}

    glColor3f(1.0f, 1.0f, 1.0f); // Réinitialiser la couleur
}


void ObjectsTool::iniShaders() {
    std::string vertexCode = readShaderFile("../src/engine/RenderTools/Shaders/vertex_shader.glsl");
    std::string fragmentCode = readShaderFile("../src/engine/RenderTools/Shaders/fragment_shader.glsl");
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
    attributeTexCoordLocation = glGetAttribLocation(shaderProgram, "texCoord");
}
void ObjectsTool::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERREUR::SHADER_COMPILATION_ERROR de type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERREUR::PROGRAM_LINKING_ERROR de type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
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

