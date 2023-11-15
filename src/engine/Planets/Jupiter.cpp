// Jupiter.cpp
#include "engine/Planets/Jupiter.h"
#include "opencv2/opencv.hpp"


Jupiter::Jupiter() : CelestialObject() {
    std::string name_sub = "Jupiter";
    std::string path_sub = "../assets/textures/jupiter.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(15,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    //Physics:
    distance_initiale = 778340821e3;
    real_radius = 69911e3;
    weight = 1.898e27;
    Vec3 initialVelocity = Vec3(0,0,-13.07e3);
    inclinaison = 3.13;
    rotationSid = 0;
    rotationSidSpeed = 0.0101;
    rotationSidDirection = Vec3(0,1,0);



    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






