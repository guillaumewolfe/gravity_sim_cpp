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

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






