#ifndef CAMERA_H
#define CAMERA_H

#include "engine/Vec3.h" // Inclure le fichier d'en-tête de Vec3
#include "engine/Camera/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera{
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;
    Vec3 originalPosition;
    Vec3 originalTarget;
    Vec3 originalUp;
    float zoomFactor;
    GLfloat modelViewMatrix[16];
    GLfloat normalMatrix[9];
    glm::mat4 globalRotationMatrix;
    Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up);

    void lookAt(); 
    void zoom(bool in);
    void rotateAround(const Vec3& center, float angle, const Vec3& axis);
    void resetPosition();
    void setPosition(Vec3 newPos);
    void setInitPosition(Vec3 newPos);
    void DrawUp();


    void updateViewMatrix();
    void setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    
    void calculateNormalMatrix();
    const GLfloat* getNormalMatrix() const;

};



#endif



