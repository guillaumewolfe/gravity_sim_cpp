// Ice.cpp
#include "engine/Planets/Ice.h"


Ice::Ice() : CelestialObject() {
    std::string name_sub = "Ice";
    typeName = "Ice";
    std::string path_sub = "../assets/textures/fictionnal_ice.jpg";
    real_radius = 2*24622e3;
    type = 4;
    textureID = loadTexture(path_sub.c_str());
    weight = 5.972e24*1.5;
    real_radius = 6371e3*1.5;
    rotationSidSpeed = 0.0000116*2;
    setName(name_sub);
    setPath(path_sub);
    setDefaultValues();

    //updatePositionSimulation();
}






