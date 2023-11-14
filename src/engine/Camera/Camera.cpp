#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/Camera/Camera.h"
#include "iostream"

Camera::Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up)
        : position(pos), target(tgt), up(up), originalPosition(pos), originalTarget(tgt), originalUp(up)  {
            globalRotationMatrix = glm::mat4(1.0f); // Ajout de la variable membre
        }

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
    glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
}


void Camera::zoom(bool in) {
    
    if (in){zoomFactor = 0.9;}
        else{zoomFactor = 1.1;}
    // Calculez le vecteur entre la position et la cible actuelle
    Vec3 viewVector = target - position;
    // Modifiez la distance entre la caméra et la cible en fonction du facteur de zoom
    viewVector *= zoomFactor;
    // Mettez à jour la position et la cible
    Vec3 newPosition = target - viewVector;

    if (in && Vec3(newPosition-target).norm()>3.2){position = newPosition;}
    else if (!in){position = newPosition;}
    

}

void Camera::DrawUp(){
    // Activer le mode de dessin de lignes
    glBegin(GL_LINES);
    Vec3 end = Vec3(position + (up * 2));
        // Définir la couleur de la ligne (par exemple, rouge)
        glColor3f(1.0f, 0.0f, 0.0f);

        // Définir les points de la ligne
        glVertex3f(0,0,0);
        glVertex3f(end.x, end.y, end.z);
    glEnd();

    // Revenir à la couleur par défaut
    glColor3f(1.0f, 1.0f, 1.0f);    
}

void Camera::rotateAround(const Vec3& center, float angle, const Vec3& axis) {
    // Créez une matrice de rotation en utilisant glm
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(axis.x, axis.y, axis.z));
    glm::quat rotationQuat = glm::angleAxis(glm::radians(angle), glm::vec3(axis.x, axis.y, axis.z));

    // Appliquez la rotation à la position de la caméra par rapport au centre de rotation
    glm::vec4 relativePosition = glm::vec4(position.x - center.x, position.y - center.y, position.z - center.z, 1.0f);
    relativePosition = rotationMatrix * relativePosition;

    // Translatez la caméra de retour au centre d'origine
    position = Vec3(center.x + relativePosition.x, center.y + relativePosition.y, center.z + relativePosition.z);

    // Mettez à jour la cible pour qu'elle pointe vers le centre
    target = center;
    Vec3 forward = (target - position).normalize();
    Vec3 right = forward.cross(up).normalize();
    up = right.cross(forward).normalize();
    // Mettez à jour la matrice de rotation globale
    globalRotationMatrix = rotationMatrix * globalRotationMatrix;

    // Réinitialisez la matrice de vue à l'identité
    glLoadIdentity();

    // Appliquez la matrice de rotation globale à la vue
    glMultMatrixf(glm::value_ptr(globalRotationMatrix));
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


void Camera::resetPosition() {
    position = originalPosition;
    target = originalTarget;
    up = originalUp;
}

void Camera::setPosition(Vec3 newPos){
    position = newPos;

}

















void Camera::calculateNormalMatrix() {
    GLfloat subMatrix[9] = {
        modelViewMatrix[0], modelViewMatrix[1], modelViewMatrix[2],
        modelViewMatrix[4], modelViewMatrix[5], modelViewMatrix[6],
        modelViewMatrix[8], modelViewMatrix[9], modelViewMatrix[10]
    };

    GLfloat det = subMatrix[0] * (subMatrix[4] * subMatrix[8] - subMatrix[7] * subMatrix[5]) -
                  subMatrix[1] * (subMatrix[3] * subMatrix[8] - subMatrix[5] * subMatrix[6]) +
                  subMatrix[2] * (subMatrix[3] * subMatrix[7] - subMatrix[4] * subMatrix[6]);

    if (det != 0) {
        GLfloat inverse[9] = {
            (subMatrix[4] * subMatrix[8] - subMatrix[5] * subMatrix[7]) / det,
            -(subMatrix[1] * subMatrix[8] - subMatrix[2] * subMatrix[7]) / det,
            (subMatrix[1] * subMatrix[5] - subMatrix[2] * subMatrix[4]) / det,
            -(subMatrix[3] * subMatrix[8] - subMatrix[5] * subMatrix[6]) / det,
            (subMatrix[0] * subMatrix[8] - subMatrix[2] * subMatrix[6]) / det,
            -(subMatrix[0] * subMatrix[5] - subMatrix[2] * subMatrix[3]) / det,
            (subMatrix[3] * subMatrix[7] - subMatrix[4] * subMatrix[6]) / det,
            -(subMatrix[0] * subMatrix[7] - subMatrix[1] * subMatrix[6]) / det,
            (subMatrix[0] * subMatrix[4] - subMatrix[1] * subMatrix[3]) / det
        };

        normalMatrix[0] = inverse[0]; normalMatrix[1] = inverse[3]; normalMatrix[2] = inverse[6];
        normalMatrix[3] = inverse[1]; normalMatrix[4] = inverse[4]; normalMatrix[5] = inverse[7];
        normalMatrix[6] = inverse[2]; normalMatrix[7] = inverse[5]; normalMatrix[8] = inverse[8];
    }
}

const GLfloat* Camera::getNormalMatrix() const {
    return normalMatrix;
}
