// Neptune.cpp
#include "engine/Planets/Neptune.h"
#include "opencv2/opencv.hpp"


Neptune::Neptune() : CelestialObject() {
    std::string name_sub = "Neptune";
    std::string path_sub = "../assets/textures/neptune.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(24,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    //Physics:
    distance_initiale = 4498396441e3;
    real_radius = 24622e3;
    weight = 1.024e26;
    Vec3 initialVelocity = Vec3(0,0,-5.43e3);
    inclinaison = 28.32;
    rotationSid = 0;
    rotationSidSpeed = 0.0062;
    rotationSidDirection = Vec3(0,1,0);

    
    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






