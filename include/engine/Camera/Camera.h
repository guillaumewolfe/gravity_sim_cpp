#ifndef CAMERA_H
#define CAMERA_H

#include "engine/Vec3.h" // Inclure le fichier d'en-tête de Vec3
#include "engine/Camera/Camera.h"

class Camera{
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;

    Camera(const Vec3& pos, const Vec3& tgt, const Vec3& up);

    void lookAt(); 
    void updateViewMatrix();
    void setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    


};



#endif



