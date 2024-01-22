#ifndef CAMERA_H
#define CAMERA_H

#include "engine/Vec3.h" // Inclure le fichier d'en-tête de Vec3
#include "engine/Camera/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "engine/CelestialObject.h"


class RenderContext;

class Camera{
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;
    Vec3 originalPosition;
    Vec3 originalTarget;
    Vec3 originalUp;
    CelestialObject* followedObject;
    CelestialObject* selectedObject;
    CelestialObject* firstPersonTargetObject;
    float zoomFactor;
    GLfloat modelViewMatrix[16];
    GLfloat normalMatrix[9];
    glm::mat4 globalRotationMatrix;
    bool zoomChanged = false;
    bool isGlobalFollowing = false;
    double angle_perspective;
    float distanceToFollowedObject; 
    Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up);
    double orbitalVerticalAngle = 0;
    double orbitalHorizontalAngle =0;
    float accumulatedHorizontalAngle = 0.0f;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    void firstPersonMode();
    void calculateSunAngleOffset(const Vec3& objectPosition, const Vec3& cameraPosition);
    double sunAngleOffset;
    void resetOrbits();
    


    void startTransition(CelestialObject* newObject, int steps);
    void updateTransition();
    void transitionToFollowObject();
    void newFocusOnAxis(std::string axis);
    void transitionToAxisFocus();
    void axisFocus();
    bool isFocusOnAxis = false;
    std::string focusedAxis;
    float targetOrbitalVerticalAngle;
    float targetOrbitalHorizontalAngle;
    //Transition
    bool isTransiting= false;
    double followingDistance;

    Vec3 lerp(const Vec3& start, const Vec3& end, float t);
    float lerp(float start, float end, float t);
    float transitionProgress;

    int transitionStep = 0;
    const int transitionThreshold = 3000; // Ajustez selon la douceur souhaitée

    float* zoomSensitiviy;
    float* rotationSensitivity;
    int transitionStepAxis = 0;
    bool isTransitingAxis = false;
    float followingOffset = 0.0f;

    void Update();
    void lookAt(); 
    void zoom(bool in);
    void zoomFirstPerson(bool in);
    void zoomByDistance(bool in, float speedOffset = 0);
    void checkDistance();
    float calculateScreenOccupationPercentage(CelestialObject* object);
    void stopAxisTransition();
    

    void rotateHorizontal(float angle);
    void rotateVertical(float angle);
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void orbitAroundObject(float horizontalAngle, float verticalAngle);
    void adjustDistanceToTarget();
    void newFollowObject(CelestialObject* obj);
    void newFirstPersonTarget(CelestialObject* obj);
    void newFollowObjectGlobal(CelestialObject* obj);
    float calculateGlobalFollowingDistance();
    void followObject();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    Vec3 getPosition();
    Vec3 getTarget();
    Vec3 getUp();
    void setTarget(Vec3 newTarget);

    void creationMode();


    void resetPosition();

    void setPosition(Vec3 newPos);
    void setInitPosition(Vec3 newPos);


    void updateViewMatrix();
    void setPerspective();
    void setCustomPerspective(float aspectRatio);
    void updateObjectVisibility(CelestialObject* object);
    
    void calculateNormalMatrix();
    const GLfloat* getNormalMatrix() const;
    void setContext(RenderContext* context);

    void changeValue(bool increase);

    double getAnglePerspective();
    bool firstPersonModeEnabled = false;
private: 
    RenderContext* m_renderContext;
    int currentSimulationSpeedIndexForTransition;
    double currentSimulationSpeedForTransition;
    float firstPersonZoomPercentage = 0.0f;
    float firstPersonZoomOffset = 0.0f;
    float maxZoomDistance;
    bool globalDistanceCalcuated = false;
    float calculateDistanceForScreenOccupation(float occupationPercentage);
    float globalFollowingDistance;
};



#endif



