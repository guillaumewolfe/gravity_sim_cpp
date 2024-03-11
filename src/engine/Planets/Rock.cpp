// Rock.cpp
#include "engine/Planets/Rock.h"
#include "opencv2/opencv.hpp"


Rock::Rock() : CelestialObject() {
    std::string name_sub = "Rock";
    typeName = "Rock";
    std::string path_sub = "../assets/textures/fictionnal_terre.jpg";
    type = 2;
    textureID = loadTexture(path_sub.c_str());
    weight = 5.972e24*0.3;
    real_radius = 6371e3*2;
    rotationSidSpeed = 0.0000116;
    setName(name_sub);
    setPath(path_sub);
    setDefaultValues();

}






