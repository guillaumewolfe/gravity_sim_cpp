// Moon.cpp
#include "engine/Planets/Moon.h"
#include "opencv2/opencv.hpp"


Moon::Moon() : CelestialObject() {
    std::string name_sub = "Moon";
    std::string path_sub = "../assets/textures/lunar.jpg";
    type = 5;
    textureID = loadTexture(path_sub.c_str());

    //Physics:
    distance_initiale = (149597870+384400)*1e3;
    real_radius = 1737e3;
    weight = 7.342e22;
    initialVelocity = Vec3(88.75,1022,-29.78e3);
    inclinaison = 5.14;
    rotationSid = 0;
    rotationSidSpeed = 0.000000424;
    rotationSidDirection = Vec3(0,1,0);
    distanceFromPlanet = 384400;


    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    //updatePositionSimulation();
}






