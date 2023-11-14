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

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






