
// Saturn.cpp
#include "engine/Planets/Saturn.h"
#include "opencv2/opencv.hpp"


Saturn::Saturn() : CelestialObject() {
    std::string name_sub = "Saturn";
    std::string path_sub = "../assets/textures/saturn.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(18,0,0);
    type = 3;
    textureID = loadTexture(path_sub.c_str());

    distance_initiale = 1426666422e3;
    real_radius = 58232e3;
    weight = 5.683e26;
    initialVelocity = Vec3(0,0,-9.68e3);
    inclinaison = 26.73;
    rotationSid = 0;
    rotationSidSpeed = 0.0000261;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "29.46 years";


    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






