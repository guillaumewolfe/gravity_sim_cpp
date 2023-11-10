#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/RenderTools/objectsTool.h"
#include <vector>
#include <cmath>



ObjectsTool::ObjectsTool(RenderContext* renderContext) : RenderComponent(renderContext){}

void ObjectsTool::Draw() {
    glEnable(GL_DEPTH_TEST);
    
    // Dessiner une sphère
    // Exemple : sphère de rayon 0.5, sans texture (0), avec 16 tranches et 16 piles
    drawSphere(0.5, 0, 16, 16);

    glDisable(GL_DEPTH_TEST);
}

void ObjectsTool::drawSphere(float radius, GLuint texture, int slices, int stacks) {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;

    for (int i = 0; i <= stacks; ++i) {
        float V = i / (float)stacks;
        float phi = V * M_PI;

        for (int j = 0; j <= slices; ++j) {
            float U = j / (float)slices;
            float theta = U * (M_PI * 2);

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
    }   

    if (texture != 0) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    glColor3f(1.0f, 1.0f, 1.0f); // Couleur blanche pour la sphère
    for (int i = 0; i < stacks; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            int idx1 = (i + j) * (slices + 1);
            int idx2 = idx1 + slices + 1;

            glNormal3fv(&normals[3 * idx1]);
            glVertex3fv(&vertices[3 * idx1]);

            glNormal3fv(&normals[3 * idx2]);
            glVertex3fv(&vertices[3 * idx2]);
        }
        glEnd();
    }

    if (texture != 0) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}


