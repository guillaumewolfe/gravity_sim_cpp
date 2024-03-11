// Volcanic.cpp
#include "engine/Planets/Volcanic.h"
#include "opencv2/opencv.hpp"


Volcanic::Volcanic() : CelestialObject() {
    std::string name_sub = "Volcanic";
    typeName = "Volcanic";
    std::string path_sub = "../assets/textures/volcanic.png";
    real_radius = 24622e3/2;
    type = 2;
    textureID = loadTexture(path_sub.c_str());
    weight = 5.972e24*0.5;
    real_radius = 6371e3*0.5;
    rotationSidSpeed = 0.0000116;
    setName(name_sub);
    setPath(path_sub);
    setDefaultValues();
}






