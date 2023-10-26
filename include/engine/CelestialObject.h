#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

#include <string>
#include <vector>
#include "engine/Vec3.h"

class CelestialObject{
    public:
    std::string name;
    std::string texture_path;
    double real_radius = 1;
    double weight = 1;
    int object_type = 1; 
    int real_distance = 1;
    double rayon_simulation = 1;

    Vec3 position_real = {0,0,0};
    Vec3 velocity = {0,0,0};
    Vec3 force = {0,0,0};
    Vec3 accel = {0,0,0};
    Vec3 position_simulation = {0,0,0};



    //Constructeur avec NAME + TEXTURE
    CelestialObject(std::string nm, std::string texture);

    // Constructeur avec tout
    CelestialObject(std::string name, std::string texture_path, double real_radius, double weight, int object_type, int real_distance, double rayon_simulation,
    const Vec3& pos_real = Vec3(0,0,0), const Vec3& velocity = Vec3(0,0,0), const Vec3& force = Vec3(0,0,0),const Vec3& accel = Vec3(0,0,0),const Vec3& pos_simul = Vec3(0,0,0));



    //Update methodes to change their values
    void updatePositionReal(const Vec3& newPosReal);
    void updatePositionSimulation(const Vec3& newPosSimul);
    void updateVelocity(const Vec3& newVel);
    void updateAccel(const Vec3& newAcel);
    void updateForce(const Vec3& newForce);
};

#endif
