// Earth.cpp
#include "engine/Planets/Earth.h"
#include "opencv2/opencv.hpp"


Earth::Earth() : CelestialObject() {
    std::string name_sub = "Terre";
    std::string path_sub = "../assets/textures/earth_real.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(9,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






