// Uranus.cpp
#include "engine/Planets/Uranus.h"
#include "opencv2/opencv.hpp"


Uranus::Uranus() : CelestialObject() {
    std::string name_sub = "Uranus";
    std::string path_sub = "../assets/textures/uranus.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(21,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    distance_initiale = 2870658186e3;
    real_radius = 25362e3;
    weight = 8.681e25;
    Vec3 initialVelocity = Vec3(0,0,-6.8e3);
    inclinaison = 97.77;
    rotationSid = 0;
    rotationSidSpeed = 0.0058;
    rotationSidDirection = Vec3(0,1,0);


    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






