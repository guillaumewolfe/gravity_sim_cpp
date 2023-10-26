#include "engine/CelestialObject.h"
#include <cmath>
#include <tuple>
#include <string>


//Constructeur avec NAME + TEXTURE
CelestialObject::CelestialObject(std::string nm, std::string texture)
: name(nm),texture_path(texture){}

// Constructeur avec tout
CelestialObject::CelestialObject(std::string name, std::string texture_path, double real_radius, double weight, int object_type, int real_distance, double rayon_simulation,
    const Vec3& pos_real, const Vec3& velocity, const Vec3& force ,const Vec3& accel ,const Vec3& pos_simul)
: position_real(pos_real),velocity(velocity),force(force),accel(accel),position_simulation(pos_simul),
name(name),texture_path(texture_path),real_radius(real_radius),weight(weight),object_type(object_type),real_distance(real_distance),rayon_simulation(rayon_simulation){}


void CelestialObject::updatePositionReal(const Vec3& newPosReal){
    position_real = newPosReal;
};
void CelestialObject::updatePositionSimulation(const Vec3& newPosSimul){
    position_simulation = newPosSimul;
};
void CelestialObject::updateVelocity(const Vec3& newVel){
    velocity = newVel;
};
void CelestialObject::updateAccel(const Vec3& newAcel){
    accel = newAcel;
};
void CelestialObject::updateForce(const Vec3& newForce){
    force = newForce;
};
