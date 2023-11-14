
// Saturn.cpp
#include "engine/Planets/Saturn.h"
#include "opencv2/opencv.hpp"


Saturn::Saturn() : CelestialObject() {
    std::string name_sub = "Saturn";
    std::string path_sub = "../assets/textures/saturn.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(18,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






