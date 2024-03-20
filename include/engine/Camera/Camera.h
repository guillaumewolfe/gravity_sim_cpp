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
    Vec3 position = Vec3(0,0,0);
    Vec3 target = Vec3(0,0,0);
    Vec3 up = Vec3(0,0,0);
    Vec3 originalPosition = Vec3(0,0,0);
    Vec3 originalTarget = Vec3(0,0,0);
    Vec3 originalUp = Vec3(0,0,0);
    CelestialObject* followedObject = nullptr;
    CelestialObject* selectedObject = nullptr;
    CelestialObject* firstPersonTargetObject = nullptr;
    float zoomFactor = 1.0f;
    GLfloat modelViewMatrix[16];
    GLfloat normalMatrix[9];
    glm::mat4 globalRotationMatrix  = glm::mat4(1.0f);
    bool zoomChanged = false;
    bool isGlobalFollowing = false;
    double angle_perspective = 0.0;
    double distanceToFollowedObject; 
    Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up);
    double orbitalVerticalAngle = 0;
    double orbitalHorizontalAngle =0;
    float accumulatedHorizontalAngle = 0.0f;
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    void firstPersonMode();
    void calculateSunAngleOffset(const Vec3& objectPosition, const Vec3& cameraPosition);
    double sunAngleOffset = 0.0;
    void resetOrbits();
    


    void startTransition(CelestialObject* newObject, int steps);
    void updateTransition();
    void transitionToFollowObject();
    void newFocusOnAxis(std::string axis);
    void transitionToAxisFocus();
    void axisFocus();
    bool isFocusOnAxis = false;
    std::string focusedAxis = "";
    float targetOrbitalVerticalAngle = 0.0f;
    float targetOrbitalHorizontalAngle = 0.0f;
    //Transition
    bool isTransiting= false;
    double followingDistance = 0.0;

    Vec3 lerp(const Vec3& start, const Vec3& end, double t);
    double lerp(double start, double end, double t);
    float transitionProgress = 0.0f;

    int transitionStep = 0;
    const int transitionThreshold = 3000; // Ajustez selon la douceur souhaitée

    float* zoomSensitiviy = nullptr;
    float* rotationSensitivity = nullptr;
    int transitionStepAxis = 0;
    bool isTransitingAxis = false;
    float followingOffset = 0.0f;

    void Update();
    void lookAt(); 
    void zoom(bool in);
    void zoomFirstPerson(bool in);
    void zoomByDistance(bool in, float speedOffset = 0);
    void checkDistance();
    double calculateScreenOccupationPercentage(CelestialObject* object);
    void stopAxisTransition();
    glm::vec2 convert3DPosToScreenPos(const glm::vec3& pos3D);
    

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
    double calculateGlobalFollowingDistance();
    void followObject();

    //comparer
    bool isComparing = false;
    double comparingDistance = 0.0f;
    bool useCompareDistance = false;
    void applyOffsetProjection();

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
    RenderContext* m_renderContext = nullptr;
    int currentSimulationSpeedIndexForTransition = 0;
    double currentSimulationSpeedForTransition = 0.0;
    double firstPersonZoomPercentage = 0.0f;
    double firstPersonZoomOffset = 0.0f;
    double maxZoomDistance = 0.0f;
    bool globalDistanceCalcuated = false;
    double calculateDistanceForScreenOccupation(float occupationPercentage);
    double globalFollowingDistance = 0.0f;
};



#endif



