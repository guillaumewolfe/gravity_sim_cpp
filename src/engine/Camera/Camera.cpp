#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/Camera/Camera.h"


Camera::Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up)
        : position(pos), target(tgt), up(up) {}

void Camera::lookAt() {
    Vec3 f = (target - position).normalize();
    Vec3 u = up.normalize();

    // Utilisez les instances de Vec3 pour appeler la méthode cross
    Vec3 s = f.cross(u).normalize();  // Modifié ici
    u = s.cross(f);  // Modifié ici

    double m[16] = {
        s.x, u.x, -f.x, 0,
        s.y, u.y, -f.y, 0,
        s.z, u.z, -f.z, 0,
        -s.dot(position), -u.dot(position), f.dot(position), 1  // Modifié ici
    };
    float mFloat[16];
for (int i = 0; i < 16; ++i) {
    mFloat[i] = static_cast<float>(m[i]);
}
    glMultMatrixf(mFloat);
}

void Camera::setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    // Calculer la hauteur et la largeur de la fenêtre à la distance de clipping près
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * winWidth/winHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);

    // Revenir à la matrice de modèle-vue
    glMatrixMode(GL_MODELVIEW);
}
