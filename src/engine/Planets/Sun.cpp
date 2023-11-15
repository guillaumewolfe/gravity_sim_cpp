// Sun.cpp
#include "engine/Planets/Sun.h"
#include "opencv2/opencv.hpp"


Sun::Sun() : CelestialObject() {
    std::string name_sub = "Sun";
    std::string path_sub = "../assets/textures/sun.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(0,0,0);
    type = 1;
    textureID = loadTexture(path_sub.c_str());


    //Physics:
    distance_initiale = 0;
    real_radius = 696340e3;
    weight = 1.989e30;
    Vec3 initialVelocity = Vec3(0,0,0);
    inclinaison = 7.25;
    rotationSid = 0;
    rotationSidSpeed = 1/300;
    rotationSidDirection = Vec3(0,1,0);


    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






