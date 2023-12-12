// Rock.cpp
#include "engine/Planets/Rock.h"
#include "opencv2/opencv.hpp"


Rock::Rock() : CelestialObject() {
    std::string name_sub = "Rock";
    std::string path_sub = "../assets/textures/fictionnal_terre.jpg";
    real_radius = 24622e3/4;
    type = 3;
    textureID = loadTexture(path_sub.c_str());
    

    setName(name_sub);
    setPath(path_sub);

}






