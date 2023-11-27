// Ice.cpp
#include "engine/Planets/Ice.h"


Ice::Ice() : CelestialObject() {
    std::string name_sub = "Ice";
    std::string path_sub = "../assets/textures/fictionnal_ice.jpg";
    type = 4;
    textureID = loadTexture(path_sub.c_str());
    

    setName(name_sub);
    setPath(path_sub);

    //updatePositionSimulation();
}






