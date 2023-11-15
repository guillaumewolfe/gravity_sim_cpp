// Venus.cpp
#include "engine/Planets/Venus.h"
#include "opencv2/opencv.hpp"


Venus::Venus() : CelestialObject() {
    std::string name_sub = "Venus";
    std::string path_sub = "../assets/textures/venus.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(6,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());


    //Physics:
    distance_initiale = 108208930e3;
    real_radius = 6051.8e3;
    weight = 4.867e24;
    Vec3 initialVelocity = Vec3(0,0,-35.02e3);
    inclinaison = 177.4;
    rotationSid = 0;
    rotationSidSpeed = 1/5832;
    rotationSidDirection = Vec3(0,-1,0);


    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






