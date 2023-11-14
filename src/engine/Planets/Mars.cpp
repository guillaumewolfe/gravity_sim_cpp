// Mars.cpp
#include "engine/Planets/Mars.h"
#include "opencv2/opencv.hpp"


Mars::Mars() : CelestialObject() {
    std::string name_sub = "Mars";
    std::string path_sub = "../assets/textures/mars.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(12,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






