// Mercure.cpp
#include "engine/Planets/Mercure.h"
#include "opencv2/opencv.hpp"


Mercure::Mercure() : CelestialObject() {
    std::string name_sub = "Mercure";
    std::string path_sub = "../assets/textures/mercure.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(3,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






