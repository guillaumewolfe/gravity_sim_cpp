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

    // Liage du VBO des vertices et configuration de l'attribut de position
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Dessin de l'ISS
    glDrawArrays(GL_POINTS, 0, vertexCount); // Ou GL_TRIANGLES selon votre modèle

    // Désactivation et nettoyage
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopMatrix();

}

void ISSTool::loadOBJ() {
    std::string path = getFullPath("../assets/3dmodels/iss.obj");
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
}


void ISSTool::setPosition(Vec3 pos) {
    position = pos;
}

Vec3 ISSTool::getPosition() const {
    return position;
}