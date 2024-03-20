#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/ISSTool.h"
#include "path_util.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>

ISSTool::ISSTool(RenderContext* renderContext) : RenderComponent(renderContext){
    position = Vec3(0, 0, 0);
    camera = renderContext->currentCamera;
    loadOBJ();
}


void ISSTool::Draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    camera->lookAt();

    // Set material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(material.ambient));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(material.diffuse));
    glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(material.specular));
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

    // Bind the VBO of vertices and set up the position attribute
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Draw the ISS
    glDrawArrays(GL_POINTS, 0, vertexCount); // Or GL_TRIANGLES, depending on your model

    // Disable and clean up
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopMatrix();
}

void ISSTool::loadOBJ() {
    std::string path = getFullPath("../assets/3dAssets/sun.obj");
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not find ISS model file at " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(Vertex{vertex}); // Assurez-vous que votre vecteur est de type std::vector<Vertex>
        }
    }
    vertexCount = vertices.size();

    // Génération et remplissage du VBO pour les vertices
    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind
    std::string mtlPath = getFullPath("../assets/3dAssets/sun.mtl");
    loadMTL(mtlPath);
}


void ISSTool::setPosition(Vec3 pos) {
    position = pos;
}

Vec3 ISSTool::getPosition() const {
    return position;
}


void ISSTool::loadMTL(const std::string& mtlPath) {
    std::ifstream mtlFile(mtlPath);
    if (!mtlFile.is_open()) {
        std::cerr << "Error: Could not find material file at " << mtlPath << std::endl;
        return;
    }

    std::string line, matName;
    Material mat;
    bool matDefined = false;

    while (std::getline(mtlFile, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "newmtl") {
            if (matDefined) {
                // Save the previously defined material before moving on to the next
                materials[matName] = mat;
            }
            iss >> matName;
            mat = Material();
            matDefined = true;
        } else if (prefix == "Ka") {
            iss >> mat.ambient.r >> mat.ambient.g >> mat.ambient.b;
        } else if (prefix == "Kd") {
            iss >> mat.diffuse.r >> mat.diffuse.g >> mat.diffuse.b;
        } else if (prefix == "Ks") {
            iss >> mat.specular.r >> mat.specular.g >> mat.specular.b;
        } else if (prefix == "Ns") {
            iss >> mat.shininess;
            // OpenGL's shininess ranges from 0 to 128, so you might need to normalize this value
            mat.shininess = std::max(std::min(mat.shininess / 1000.0f * 128.0f, 128.0f), 0.0f);
        } else if (prefix == "d") {
            // Transparency is handled differently in OpenGL, you might want to store it but it won't be used like this
        }
        // Handle other properties as needed
    }

    if (matDefined) {
        materials[matName] = mat; // Don't forget the last material
    }
}