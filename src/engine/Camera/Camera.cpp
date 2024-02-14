#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/Camera/Camera.h"
#include "iostream"
#include <algorithm>
#include "engine/RenderTools/RenderContext.h"
Camera::Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up)
        : position(pos), target(tgt), up(up), originalPosition(pos), originalTarget(tgt), originalUp(up)  {
            globalRotationMatrix = glm::mat4(1.0f); // Ajout de la variable membre
        }

void Camera::Update() {
    if (followedObject) {
        if(firstPersonModeEnabled){
            firstPersonMode();
        }
        else{
                if(isFocusOnAxis && isTransitingAxis){
                    transitionToAxisFocus();
                }
                else if (isTransiting) {
                    transitionToFollowObject();
                
                //followObject();
            } else {
                followObject();
            
        }
    }}
    lookAt();
}

float Camera::lerp(float start, float end, float t) {
    t = glm::clamp(t, 0.0f, 1.0f); // Ensure t is within the range [0, 1]
    return start + (end - start) * t;
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
   up = Vec3(0,1,0);
   if (!followedObject or isTransitingAxis) {return;}
  
   Vec3 objectPosition = followedObject->getPositionSimulation();
   float objectRadius;
   float pourcentOfScreen;
   if(followedObject->type != 1){
       objectRadius = followedObject->getRayon();
       pourcentOfScreen = 0.30f;
   }
   else{
       objectRadius = followedObject->getRayon();
       pourcentOfScreen = 0.70f;
   }
   // Calculer la distance de suivi désirée
   float verticalFOV = angle_perspective * (M_PI / 180.0f);
   float desiredDistance = objectRadius / (tan(verticalFOV / 2) * pourcentOfScreen); // 20% occupation


   if(!globalDistanceCalcuated){
   globalFollowingDistance = calculateGlobalFollowingDistance();}


   this->followingDistance = isGlobalFollowing ? globalFollowingDistance : desiredDistance;
   this->followingDistance = (isComparing && useCompareDistance) ? comparingDistance : this->followingDistance;
   // Calculer la position finale
   Vec3 finalOffset;
   finalOffset.x = followingDistance * cos(orbitalVerticalAngle) * sin(orbitalHorizontalAngle);
   finalOffset.y = followingDistance * sin(orbitalVerticalAngle);
   finalOffset.z = followingDistance * cos(orbitalVerticalAngle) * cos(orbitalHorizontalAngle);
   Vec3 finalPosition = objectPosition + finalOffset;


   // Interpolation temporelle
   float t = (float)transitionStep / transitionThreshold;
   position = lerp(position, finalPosition, t);
   target = lerp(target, objectPosition, t);


   if (glm::abs((position-followedObject->getPositionSimulation()).norm()-followingDistance)<0.01){
       //std::cout<<"Position - final pos: "<<(position-finalPosition).norm()<<" Desired distance: "<<desiredDistance<<std::endl;
       isTransiting = false;
       transitionStep = 0;
       if(!isGlobalFollowing){
           *(m_renderContext->currentSpeedIndex) = currentSimulationSpeedIndexForTransition;
           *(m_renderContext->timeMultiplier) = currentSimulationSpeedForTransition;
       }
       return;
   }


   transitionStep++;


   if (transitionStep >= transitionThreshold) {
       isTransiting = false;
       transitionStep = 0;
       if(!isGlobalFollowing){
           *(m_renderContext->currentSpeedIndex) = currentSimulationSpeedIndexForTransition;
           *(m_renderContext->timeMultiplier) = currentSimulationSpeedForTransition;
       }
       return;
   }
   if(isComparing){
   applyOffsetProjection();}
}


void Camera::followObject() {
   if (!followedObject) return;
   if (firstPersonModeEnabled) return;
  
   float distance;
   if(isGlobalFollowing){
       distance = globalFollowingDistance;
   } else if (isComparing && useCompareDistance){
       distance = comparingDistance;
   }else{
       distance = followingDistance;
   }


   Vec3 objectPosition = followedObject->getPositionSimulation();


   // Utiliser les angles orbitaux pour calculer la position
   Vec3 offset;
   offset.x = distance * cos(orbitalVerticalAngle) * sin(orbitalHorizontalAngle);
   offset.y = distance * sin(orbitalVerticalAngle);
   offset.z = distance * cos(orbitalVerticalAngle) * cos(orbitalHorizontalAngle);


   position = objectPosition + offset;
   target = objectPosition;


   // Appliquer une matrice de projection personnalisée pour le décalage
   if(isComparing){
   applyOffsetProjection();}
}


void Camera::applyOffsetProjection() {
   int winWidth, winHeight;
   glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
   float aspectRatio = static_cast<float>(winWidth) / winHeight;
   float zNear = 0.001;
   float zFar = 10000;
   float fovy = angle_perspective * (M_PI / 180.0f);
   float fH = tan(fovy / 2) * zNear;
   float fW = fH * aspectRatio;


   // Calculer la largeur totale du frustum à zNear
   float totalWidth = 2 * fW;


   // Décalage pour que le bord gauche du frustum soit à 25% de la largeur de l'écran
   float offsetX = fW - 0.25 * totalWidth;


   // Ajuster les paramètres left et right du frustum
   float left = -fW + offsetX;
   float right = fW + offsetX;




   // Appliquer la matrice de projection personnalisée
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(left, right, -fH, fH, zNear, zFar);


   // Revenir à la matrice de modèle-vue
   glMatrixMode(GL_MODELVIEW);
   projectionMatrix = glm::frustum(left, right, -fH, fH, zNear, zFar);
}



float Camera::calculateGlobalFollowingDistance() {
    float maxDistance = 0.0f;
    float verticalFOV = angle_perspective * (M_PI / 180.0f);
    float halfFOV = tan(verticalFOV / 2);

    for (const auto& object : m_renderContext->systemeSolaire->objects) {
        Vec3 objectPosition = object->getPositionSimulation();
        
        // Calculez la position relative à la caméra
        Vec3 relativePosition = objectPosition - target; // Ici target est la position que la caméra vise

        // Utilisez la projection de cette position sur l'axe de la caméra
        Vec3 forward = (target - position).normalize();
        float distanceAlongCameraAxis = relativePosition.dot(forward); // 'forward' est le vecteur directionnel de la caméra

        // Calculez la distance perpendiculaire à l'axe de la caméra
        float perpendicularDistance = sqrt(pow(relativePosition.norm(),2) - distanceAlongCameraAxis * distanceAlongCameraAxis);

        // Calculez la distance maximale à laquelle l'objet est visible
        float visibleDistance = perpendicularDistance / halfFOV;
        
        if (visibleDistance > maxDistance) {
            maxDistance = visibleDistance;
        }
    }
    globalDistanceCalcuated = true;
    return maxDistance/5;
}




void Camera::firstPersonMode() {
    if (!followedObject || !firstPersonModeEnabled || !firstPersonTargetObject) return;

    Vec3 objectPosition = followedObject->getPositionSimulation();
    Vec3 objToLookAtPosition = firstPersonTargetObject->getPositionSimulation();
    Vec3 directionToObject = (objToLookAtPosition - objectPosition).normalize();
    float objectRadius = followedObject->getRayon();
    float heightAboveSurface = objectRadius * 0.01f;

    // Déterminer la distance maximale pour 75% d'occupation de l'écran
    float totalDistance = (firstPersonTargetObject->getPositionSimulation() - followedObject->getPositionSimulation()).norm();

    // Déterminer la distance pour 75% d'occupation de l'écran
    float distanceFor75PercScreenOccupation = calculateDistanceForScreenOccupation(30.0f);

    // maxZoomDistance est la différence entre la distance totale et la distance pour 75% d'occupation
    maxZoomDistance = totalDistance - distanceFor75PercScreenOccupation;
    maxZoomDistance = std::max(maxZoomDistance, 0.0f);
    // Interpoler entre le point minimal (heightAboveSurface) et le point maximal (maxZoomDistance)
    float zoomDistance = heightAboveSurface + (maxZoomDistance - heightAboveSurface) * firstPersonZoomPercentage;

    position = objectPosition + directionToObject * (objectRadius + zoomDistance);
    target = objToLookAtPosition;

    Vec3 globalUp(0, 1, 0);
    Vec3 right = directionToObject.cross(globalUp).normalize();
    up = right.cross(directionToObject).normalize();
    lookAt();
}

void Camera::zoomFirstPerson(bool in) {
    // Base zoom adjustment factor
    float baseZoomAdjustmentFactor = 0.0025f; 

    // Apply a quadratic scaling to the zoom adjustment factor
    float zoomAdjustmentFactor = baseZoomAdjustmentFactor * (1.0f - firstPersonZoomPercentage * firstPersonZoomPercentage);

    if (in) {
        // Increase zoom, but not beyond 100%
        firstPersonZoomPercentage = std::min(firstPersonZoomPercentage + zoomAdjustmentFactor, 1.0f);
    } else {
        // Decrease zoom, but not below 0%
        firstPersonZoomPercentage = std::max(firstPersonZoomPercentage - zoomAdjustmentFactor, 0.0f);
    }
}


float Camera::calculateDistanceForScreenOccupation(float occupationPercentage) {
    if (!firstPersonTargetObject) return 0.0f;

    float objectRadius = firstPersonTargetObject->getRayon();
    float verticalFOV = this->angle_perspective * (M_PI / 180.0f);

    // Calculer la taille angulaire attendue pour le pourcentage d'occupation donné
    float angularSize = 2 * atan(tan(verticalFOV / 2.0f) * (occupationPercentage / 100.0f));

    // Calculer la distance à partir de la taille angulaire
    float distance = objectRadius / tan(angularSize / 2.0f);

    return distance;
}



float Camera::calculateScreenOccupationPercentage(CelestialObject* object) {
    if (!object) return 0.0f;

    Vec3 objectPosition = object->getPositionSimulation();
    float objectRadius = object->getRayon();

    // Calculer la distance entre la caméra et l'objet
    float distance = (objectPosition - this->position).norm();

    // Calculer la taille angulaire de l'objet (en radians)
    float angularSize = 2 * atan(objectRadius / distance);

    // Calculer le FOV vertical de la caméra (en radians)
    float verticalFOV = this->angle_perspective * (M_PI / 180.0f);

    // Calculer le pourcentage de l'écran occupé par l'objet
    float screenOccupation = (angularSize / verticalFOV) * 100.0f; // Converti en pourcentage

    return screenOccupation;
}

void Camera::transitionToAxisFocus() {
    if (!followedObject || !isFocusOnAxis) return;
    // Interpolate orbital angles
    float t = (float)transitionStepAxis / transitionThreshold;
    orbitalVerticalAngle = lerp(orbitalVerticalAngle, targetOrbitalVerticalAngle, t);
    orbitalHorizontalAngle = lerp(orbitalHorizontalAngle, targetOrbitalHorizontalAngle, t);

    // Continue to follow the object using the interpolated angles
    followObject();

    // Use a fixed angle tolerance for comparison (e.g., 5 degrees in radians)
    float angleToleranceHorizontal = glm::radians(0.001f);
    float angleToleranceVertical = glm::radians(0.1f);
    bool closeToVertical = glm::abs(orbitalVerticalAngle - targetOrbitalVerticalAngle) < angleToleranceVertical;
    bool closeToHorizontal = glm::abs(orbitalHorizontalAngle - targetOrbitalHorizontalAngle) < angleToleranceHorizontal;


    if (closeToVertical && closeToHorizontal) {
        isTransitingAxis = false;
        isFocusOnAxis = false; // You might also want to reset this flag
        transitionStepAxis = 0;
        return;
    }else if(transitionStepAxis >= transitionThreshold) {
        transitionStepAxis = 0;
        isTransitingAxis = false;
        isFocusOnAxis = false; // You might also want to reset this flag
        return;
    }else{transitionStepAxis++;}
}




void Camera::changeValue(bool increase){
    if(increase){}
        else{}
    
}

void Camera::zoomByDistance(bool in, float speedOffset){
    if (!followedObject) return;
    float speed = *zoomSensitiviy*0.02+speedOffset;

    //Global
    if(isGlobalFollowing){

        if(in){globalFollowingDistance /= (1.0+speed);}
        else{globalFollowingDistance *= (1.01+speed);}
        }

    //Focus 
    else{
        float screenOccupation = calculateScreenOccupationPercentage(followedObject);
        //Si screenOccupation > 80%, return; sinon zoom out/in comme d'habitude
        if(in){
            if(screenOccupation>80){return;}
            else{followingDistance /= (1.0+speed);}
        }
        else{
            if(screenOccupation<1){return;}
            else{followingDistance *= (1.0+speed);}
        }

    }



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
    viewMatrix = glm::lookAt(position.toGlm(), target.toGlm(), up.toGlm());
}

void Camera::updateObjectVisibility(CelestialObject* object) {
    if (!object) {
        return;
    }

    // Obtenir la position de l'objet et son rayon
    Vec3 objectPosition = object->getPositionSimulation();
    float objectRadius = object->getRayon();

    // Calculer la distance entre la caméra et l'objet
    float distance = (objectPosition - this->position).norm();

    // Calculer la taille angulaire de l'objet (en radians)
    float angularSize = 2 * atan(objectRadius / distance);

    // Calculer le FOV vertical de la caméra (en radians)
    float verticalFOV = this->angle_perspective * (M_PI / 180.0f);

    // Déterminer si l'objet occupe au moins 1% de l'écran
    // (comparer la taille angulaire de l'objet avec le FOV)
    float screenOccupation = angularSize / verticalFOV;
    object->shouldBeDrawn = (screenOccupation >= 0.00025);
}

void Camera::creationMode(){
    followedObject=nullptr;  
    up=Vec3(0,0,-1);
    angle_perspective = 70;
    setPerspective();
}


void Camera::zoom(bool in) {
    if (firstPersonModeEnabled) {
        zoomFirstPerson(in);
    } else {
        // Mode non première personne, ajustez l'angle de perspective
        if (in) {
            angle_perspective *= 0.99;
        } else {
            angle_perspective *= 1.01;
        }
        if (angle_perspective > 150) angle_perspective = 150;
        if (angle_perspective < 10) angle_perspective = 10; // Évitez un angle de perspective trop petit

        setPerspective();
    }
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
    float speed = *rotationSensitivity*2;
    orbitalHorizontalAngle += horizontalAngle*speed;
    orbitalVerticalAngle += verticalAngle*speed;
    if (orbitalVerticalAngle>1.57){orbitalVerticalAngle = 1.57;}
    if (orbitalVerticalAngle<-1.57){orbitalVerticalAngle = -1.57;}
}





void Camera::setPerspective() {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    int winWidth, winHeight;
    float zNear = 0.001;
    float zFar = 10000;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    // Calculer la hauteur et la largeur de la fenêtre à la distance de clipping près
    fH = tan(angle_perspective / 360 * pi) * zNear;
    fW = fH * winWidth/winHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);

    // Revenir à la matrice de modèle-vue
    glMatrixMode(GL_MODELVIEW);
    projectionMatrix = glm::frustum(static_cast<float>(-fW), static_cast<float>(fW), static_cast<float>(-fH), static_cast<float>(fH), static_cast<float>(zNear), static_cast<float>(zFar));
}
void Camera::setCustomPerspective(float aspectRatio) {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    float zNear = 0.001;
    float zFar = 3000;

    // Utiliser l'aspectRatio fourni pour calculer fW et fH
    fH = tan(angle_perspective / 360 * pi) * zNear;
    fW = fH * aspectRatio;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);

    // Revenir à la matrice de modèle-vue
    glMatrixMode(GL_MODELVIEW);
    projectionMatrix = glm::frustum(static_cast<float>(-fW), static_cast<float>(fW), static_cast<float>(-fH), static_cast<float>(fH), static_cast<float>(zNear), static_cast<float>(zFar));
}

void Camera::newFollowObject(CelestialObject* obj) {
    //resetPosition();
    isTransiting = true;
    followedObject = obj;
    isFocusOnAxis = false;
    // Exemple de définition d'un nouvel offset
    Vec3 objectPosition = followedObject->getPositionSimulation();
    transitionStep = 0;
    angle_perspective = 40;
    setPerspective();
    currentSimulationSpeedIndexForTransition = *(m_renderContext->currentSpeedIndex);
    *(m_renderContext->currentSpeedIndex) = 6; 
    currentSimulationSpeedForTransition = *(m_renderContext->timeMultiplier);
    *(m_renderContext->timeMultiplier) = 0;
    isGlobalFollowing = false;
}

void Camera::newFollowObjectGlobal(CelestialObject* obj) {
    if(m_renderContext->systemeSolaire->objects.size()==1){
        newFollowObject(obj);
        return;
    }
    isTransiting = true;
    followedObject = obj;
    isFocusOnAxis = false;
    // Exemple de définition d'un nouvel offset
    Vec3 objectPosition = followedObject->getPositionSimulation();
    float initialDistance = 10.0f; // Ou toute autre valeur logique
    Vec3 initialOffset = Vec3(0, 0, initialDistance); // Modifier selon les besoins
    transitionStep = 0;
    angle_perspective = 35;
    setPerspective();
    isGlobalFollowing = true;
    globalDistanceCalcuated = false;
}


void Camera::newFirstPersonTarget(CelestialObject* targetObject) {
    firstPersonTargetObject = targetObject;
    firstPersonModeEnabled = true;
    isFocusOnAxis = false;
    firstPersonZoomOffset = 0.0f;
    firstPersonZoomPercentage = 0.0f;
    firstPersonZoomPercentage = 0.0f;
    angle_perspective = 40;
    isGlobalFollowing = false;
    setPerspective();

}


void Camera::resetPosition() {
    // position = originalPosition;
    // target = originalTarget;
    // up = originalUp;
    angle_perspective=40;
    isComparing = false;
    useCompareDistance = false;
    followedObject = nullptr;
    selectedObject = nullptr;
    firstPersonTargetObject = nullptr;
    firstPersonModeEnabled = false;
    isGlobalFollowing = false;
    isFocusOnAxis = false;
    orbitalHorizontalAngle = 0;
    orbitalVerticalAngle = 0;
    isTransitingAxis = false;
    transitionStep = 0;
    transitionStepAxis = 0;
    setPerspective();
    lookAt();
    if (m_renderContext && !m_renderContext->systemeSolaire->objects.empty()) {
        resetOrbits();
        newFollowObjectGlobal(m_renderContext->systemeSolaire->getSun());
        *(m_renderContext->showInfo) = false;
        globalDistanceCalcuated = false;
    }
    selectedObject = followedObject;
}
void Camera::resetOrbits(){
        orbitalVerticalAngle = 0.1;
        orbitalHorizontalAngle = -1.18;
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


glm::mat4 Camera::getViewMatrix() {
        return viewMatrix;}

glm::mat4 Camera::getProjectionMatrix() {
    return projectionMatrix;
}

Vec3 Camera::getPosition(){
    return position;
}

Vec3 Camera::getTarget(){
    return target;
}
void Camera::setTarget(Vec3 newTarget){
    target = newTarget;
}

void Camera::setContext(RenderContext* context){
    this->m_renderContext = context;
}

double Camera::getAnglePerspective(){
    return angle_perspective;
}

Vec3 Camera::getUp(){
    return up;
}

void Camera::newFocusOnAxis(std::string axis) {
    if (!followedObject) return;
    isTransitingAxis = true;
    isFocusOnAxis = true;
    transitionStepAxis = 0;

    if(*(m_renderContext->isOrbiting)){
        *(m_renderContext->isOrbiting) = false;}

    // Determine target orbital angles based on the specified axis
    if (axis == "x") {
        targetOrbitalVerticalAngle = 0;
        targetOrbitalHorizontalAngle = M_PI / 2;
    } else if (axis == "y") {
        targetOrbitalVerticalAngle = M_PI / 2;
        targetOrbitalHorizontalAngle = 0;
    } else if (axis == "z") {
        targetOrbitalVerticalAngle = 0;
        targetOrbitalHorizontalAngle = 0;
    } else {
        std::cerr << "Invalid axis specified: " << axis << std::endl;
        isTransiting = false;
        isFocusOnAxis = false;
        return;
    }
}

void Camera::stopAxisTransition(){
    isTransitingAxis = false;
    isFocusOnAxis = false;
    transitionStepAxis = 0;
}
