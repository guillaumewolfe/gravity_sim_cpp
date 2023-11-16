#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/Camera/Camera.h"
#include "iostream"
#include <algorithm>

Camera::Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up)
        : position(pos), target(tgt), up(up), originalPosition(pos), originalTarget(tgt), originalUp(up)  {
            globalRotationMatrix = glm::mat4(1.0f); // Ajout de la variable membre
        }

void Camera::Update(){
    if (followedObject) {
        Vec3 objectPosition = followedObject->getPositionSimulation();

        // Mettez à jour la position de la caméra pour orbiter autour de l'objet
        // Les angles orbitaux doivent être mis à jour ailleurs dans votre code (par exemple, à l'aide des entrées utilisateur)
        float distanceToObject = (position - objectPosition).norm();

        Vec3 offset;
        offset.x = distanceToObject * cos(orbitalVerticalAngle) * sin(orbitalHorizontalAngle);
        offset.y = distanceToObject * sin(orbitalVerticalAngle);
        offset.z = distanceToObject * cos(orbitalVerticalAngle) * cos(orbitalHorizontalAngle);

        position = objectPosition + offset;

        // Cibler la caméra sur l'objet
        target = objectPosition;

    }
    lookAt();
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
    
    if (in){zoomFactor = 0.99;}
        else{zoomFactor = 1.01;}

    angle_perspective *= zoomFactor;
    setPerspective(angle_perspective,0,0.5,1200);

}

void Camera::rotateHorizontal(float angle) {
    Vec3 forward = (target - position).normalize();
    Vec3 right = forward.cross(up).normalize();
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(up.x, up.y, up.z));
    glm::vec3 rotatedForward = glm::vec3(rotation * glm::vec4(forward.x,forward.y,forward.z, 0.0));
    forward = Vec3(rotatedForward.x,rotatedForward.y,rotatedForward.z);
    target = position + forward;
}

void Camera::rotateVertical(float angle) {
    Vec3 forward = (target - position).normalize();
    Vec3 right = forward.cross(up).normalize();
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(right.x, right.y, right.z));
    glm::vec3 rotatedForward = glm::vec3(rotation * glm::vec4(forward.x, forward.y, forward.z, 0.0));
    forward = Vec3(rotatedForward.x, rotatedForward.y, rotatedForward.z);
    target = position + forward;
    up = right.cross(forward).normalize();
}

void Camera::moveForward(float distance) {
    Vec3 forward = (target - position).normalize();
    position.x = position.x + forward.x * distance;
    position.y = position.y + forward.y * distance;
    position.z = position.z + forward.z * distance;
    target.x = target.x + forward.x * distance;
    target.y = target.y + forward.y * distance;
    target.z = target.z + forward.z * distance;
}

void Camera::moveRight(float distance) {
    Vec3 forward = (target - position).normalize();
    Vec3 right = forward.cross(up).normalize();
    position.x = position.x +right.x * distance;
    position.y = position.y+right.y * distance;
    position.z = position.z+right.z * distance;
    target.x = target.x+right.x * distance;
    target.y = target.y+right.y * distance;
    target.z = target.z+right.z * distance;
}

void Camera::orbitAroundObject(float horizontalAngle, float verticalAngle) {
    if (!followedObject) {
        return;
    }

    orbitalHorizontalAngle += horizontalAngle;
    orbitalVerticalAngle += verticalAngle;
    if (orbitalVerticalAngle>M_PI/2){orbitalVerticalAngle = M_PI/2;}
    if (orbitalVerticalAngle<-M_PI/2){orbitalVerticalAngle = -M_PI/2;}
}





void Camera::setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    angle_perspective = fovY;
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

void Camera::followObject(CelestialObject* obj) {
    resetPosition();
    followedObject = obj;
}

void Camera::resetPosition() {
    position = originalPosition;
    target = originalTarget;
    up = originalUp;
    angle_perspective=45;
    followedObject = nullptr;
    orbitalHorizontalAngle = 0;
    orbitalVerticalAngle = 0;
    setPerspective(45,0,0.5,1200);
}

void Camera::setPosition(Vec3 newPos){
    position = newPos;
}

void Camera::setInitPosition(Vec3 newPos){
    position = newPos;
    originalPosition = newPos;
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
