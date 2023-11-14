// Volcanic.cpp
#include "engine/Planets/Volcanic.h"
#include "opencv2/opencv.hpp"


Volcanic::Volcanic() : CelestialObject() {
    std::string name_sub = "Volcanic";
    std::string path_sub = "../assets/textures/volcanic.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(3,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






