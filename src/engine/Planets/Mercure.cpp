// Mercure.cpp
#include "engine/Planets/Mercure.h"
#include "opencv2/opencv.hpp"


Mercure::Mercure() : CelestialObject() {
    std::string name_sub = "Mercure";
    std::string path_sub = "../assets/textures/mercure.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(3,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    //Physics:
    distance_initiale = 57909175e3;
    real_radius = 2439.7e3;
    weight = 3.301e23;
    Vec3 initialVelocity = Vec3(0,0,-47.87e3);
    inclinaison = 0.034;
    rotationSid = 0;
    rotationSidSpeed = 1/5064;
    rotationSidDirection = Vec3(0,1,0);

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






