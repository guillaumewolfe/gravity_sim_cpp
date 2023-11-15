// Rock.cpp
#include "engine/Planets/Rock.h"
#include "opencv2/opencv.hpp"


Rock::Rock() : CelestialObject() {
    std::string name_sub = "Rock";
    std::string path_sub = "../assets/textures/Rock_real.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(3,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
    updatePositionSimulation();
}






