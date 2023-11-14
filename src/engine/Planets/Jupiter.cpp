// Jupiter.cpp
#include "engine/Planets/Jupiter.h"
#include "opencv2/opencv.hpp"


Jupiter::Jupiter() : CelestialObject() {
    std::string name_sub = "Jupiter";
    std::string path_sub = "../assets/textures/jupiter.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(15,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






