// BlackHole.cpp
#include "engine/Planets/BlackHole.h"


BlackHole::BlackHole() : CelestialObject() {
    std::string name_sub = "Black Hole";
    typeName = "BlackHole";
    std::string path_sub = "../assets/textures/black_hole.jpg";
    type = 0;
    textureID = loadTexture(path_sub.c_str());
    real_radius = 2*696340e3;
    weight = 10*1.989e30;
    

    setName(name_sub);
    setPath(path_sub);
    setDefaultValues();

    //updatePositionSimulation();
}






