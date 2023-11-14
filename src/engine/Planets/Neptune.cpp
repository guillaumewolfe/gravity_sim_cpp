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

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






