// Volcanic.cpp
#include "engine/Planets/Volcanic.h"
#include "opencv2/opencv.hpp"


Volcanic::Volcanic() : CelestialObject() {
    std::string name_sub = "Volcanic";
    std::string path_sub = "../assets/textures/volcanic.png";

    type = 2;
    textureID = loadTexture(path_sub.c_str());

    setName(name_sub);
    setPath(path_sub);
}






