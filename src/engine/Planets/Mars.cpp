// Mars.cpp
#include "engine/Planets/Mars.h"
#include "opencv2/opencv.hpp"


Mars::Mars() : CelestialObject() {
    std::string name_sub = "Mars";
    std::string path_sub = "../assets/textures/mars.jpg";
    type = 2;
    textureID = loadTexture(path_sub.c_str());


    distance_initiale = 225000000e3;
    real_radius = 3390e3;
    weight = 6.417e23;
    initialVelocity = Vec3(0,0,-24e3);
    inclinaison = 25.19;
    rotationSid = 0;
    rotationSidSpeed = 0.0000113;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "1.88 years";


    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






