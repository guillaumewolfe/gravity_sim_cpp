// Uranus.cpp
#include "engine/Planets/Uranus.h"
#include "opencv2/opencv.hpp"


Uranus::Uranus() : CelestialObject() {
    std::string name_sub = "Uranus";
    std::string path_sub = "../assets/textures/uranus.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(21,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
    setRayon(rayon_sub);
    updatePositionSimulation(position_initiale_sub);
}






