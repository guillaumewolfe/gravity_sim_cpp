#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H


#include <string>
#include <vector>
#include "engine/Vec3.h"
#include <GLFW/glfw3.h>


class CelestialObject{
protected:
    CelestialObject();
public:
    std::string name;
    std::string texture_path;
    double real_radius = 1;
    double weight = 1;
    int object_type = 1; 
    int distance_initiale = 1;
    double rayon_simulation = 1;
    int type;
    GLuint textureID;

    //SCALE
    double radiusScale = 1;


    Vec3 position_real = {0,0,0};
    Vec3 velocity = {0,0,0};
    Vec3 force = {0,0,0};
    Vec3 accel = {0,0,0};
    Vec3 position_simulation = {0,0,0};

    // Constructeur avec tout
    CelestialObject(std::string name, std::string texture_path, double real_radius, double weight, int object_type, int distance_initiale, double rayon_simulation,
    const Vec3& pos_real = Vec3(0,0,0), const Vec3& velocity = Vec3(0,0,0), const Vec3& force = Vec3(0,0,0),const Vec3& accel = Vec3(0,0,0),const Vec3& pos_simul = Vec3(0,0,0));


    //Update methodes to change their values
    virtual void updatePositionReal(const Vec3& newPosReal);
    virtual void updatePositionSimulation(const Vec3& newPosSimul);
    virtual void updateVelocity(const Vec3& newVel);
    virtual void updateAccel(const Vec3& newAcel);
    virtual void updateForce(const Vec3& newForce);
    GLuint loadTexture(const char* filename);
    GLuint getTexture();

    virtual void setName(std::string);
    virtual std::string getName();

    virtual void setPath(std::string);
    virtual std::string getPath();

    virtual void setRayon(double rayon);
    virtual double getRayon();
    virtual void realRadiusToSimRadius();


};

#endif


