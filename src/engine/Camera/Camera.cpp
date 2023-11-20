#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/Camera/Camera.h"
#include "iostream"
#include <algorithm>

Camera::Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up)
        : position(pos), target(tgt), up(up), originalPosition(pos), originalTarget(tgt), originalUp(up)  {
            globalRotationMatrix = glm::mat4(1.0f); // Ajout de la variable membre
            resetPosition();
        }

void Camera::Update() {
    if (followedObject) {
        if (isTransiting) {
            transitionToFollowObject();
            //followObject();
        } else {
            followObject();
        }
    }
    lookAt();
}


Vec3 Camera::lerp(const Vec3& start, const Vec3& end, float t) {
    t = glm::clamp(t, 0.0f, 1.0f); // Assurez-vous que t reste dans la plage [0, 1]

    // Réduisez la vitesse de changement de t en multipliant par un facteur de ralentissement
    float slowdownFactor = 1.0f; // Réglez cette valeur pour contrôler la vitesse de transition
    t = glm::pow(t, slowdownFactor);

    Vec3 newVec;
    newVec.x = start.x + (end.x - start.x) * t;
    newVec.y = start.y + (end.y - start.y) * t;
    newVec.z = start.z + (end.z - start.z) * t;
    return newVec;
}
    
void Camera::transitionToFollowObject() {
    if (!followedObject) return;
    
    Vec3 objectPosition = followedObject->getPositionSimulation();
    float objectRadius = followedObject->getRayon();

    // Calculer la distance de suivi désirée
    float verticalFOV = angle_perspective * (M_PI / 180.0f);
    float desiredDistance = objectRadius / (tan(verticalFOV / 2) * 0.20f); // 20% occupation

    this->followingDistance = desiredDistance;

    // Calculer la position finale
    Vec3 finalOffset;
    finalOffset.x = desiredDistance * cos(orbitalVerticalAngle) * sin(orbitalHorizontalAngle);
    finalOffset.y = desiredDistance * sin(orbitalVerticalAngle);
    finalOffset.z = desiredDistance * cos(orbitalVerticalAngle) * cos(orbitalHorizontalAngle);
    Vec3 finalPosition = objectPosition + finalOffset;

    // Interpolation temporelle
    float t = (float)transitionStep / transitionThreshold;
    position = lerp(position, finalPosition, t);
    target = lerp(target, objectPosition, t);


    if (glm::abs((position-finalPosition).norm()-desiredDistance)<0.1){
        //std::cout<<"Position - final pos: "<<(position-finalPosition).norm()<<" Desired distance: "<<desiredDistance<<std::endl;
        isTransiting = false;
        transitionStep = 0;
        return;
    }

    transitionStep++;

    if (transitionStep >= transitionThreshold) {
        isTransiting = false;
        transitionStep = 0;
    }
}


void Camera::followObject() {
    if (!followedObject) return;

    Vec3 objectPosition = followedObject->getPositionSimulation();

    // Utiliser les angles orbitaux pour calculer la position
    Vec3 offset;
    offset.x = followingDistance * cos(orbitalVerticalAngle) * sin(orbitalHorizontalAngle);
    offset.y = followingDistance * sin(orbitalVerticalAngle);
    offset.z = followingDistance * cos(orbitalVerticalAngle) * cos(orbitalHorizontalAngle);

    position = objectPosition + offset;
    target = objectPosition;
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
    if(angle_perspective>150){angle_perspective = 150;}
    setPerspective();
}

void Camera::rotateHorizontal(float angle) {
    Vec3 forward = (target - position).normalize();
    
    // Utiliser l'axe Y global pour la rotation
    Vec3 globalUp(0, 1, 0);

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(globalUp.x, globalUp.y, globalUp.z));
    glm::vec3 rotatedForward = glm::vec3(rotation * glm::vec4(forward.x, forward.y, forward.z, 0.0));

    forward = Vec3(rotatedForward.x, rotatedForward.y, rotatedForward.z);
    target = position + forward;

    // Recalculer 'up' pour s'assurer qu'il est perpendiculaire au plan XZ
    Vec3 right = forward.cross(globalUp).normalize();
    up = right.cross(forward).normalize();
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

void Camera::moveUp(float distance) {
    Vec3 upDirection = up.normalize();
    position.x = position.x + upDirection.x * distance;
    position.y = position.y + upDirection.y * distance;
    position.z = position.z + upDirection.z * distance;
    target.x = target.x + upDirection.x * distance;
    target.y = target.y + upDirection.y * distance;
    target.z = target.z + upDirection.z * distance;
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





void Camera::setPerspective() {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    int winWidth, winHeight;
    float zNear = 0.01;
    float zFar = 1500;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    // Calculer la hauteur et la largeur de la fenêtre à la distance de clipping près
    fH = tan(angle_perspective / 360 * pi) * zNear;
    fW = fH * winWidth/winHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);

    // Revenir à la matrice de modèle-vue
    glMatrixMode(GL_MODELVIEW);
}

void Camera::newFollowObject(CelestialObject* obj) {
    //resetPosition();
    isTransiting = true;
    followedObject = obj;

    // Exemple de définition d'un nouvel offset
    Vec3 objectPosition = followedObject->getPositionSimulation();
    float initialDistance = 10.0f; // Ou toute autre valeur logique
    Vec3 initialOffset = Vec3(0, 0, initialDistance); // Modifier selon les besoins
    transitionStep = 0;
    angle_perspective = 45;
    setPerspective();

}


void Camera::resetPosition() {
    position = originalPosition;
    target = originalTarget;
    up = originalUp;
    angle_perspective=40;
    followedObject = nullptr;
    orbitalHorizontalAngle = 0;
    orbitalVerticalAngle = 0;
    setPerspective();
    lookAt();
    
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