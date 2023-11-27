// BlackHole.cpp
#include "engine/Planets/BlackHole.h"


BlackHole::BlackHole() : CelestialObject() {
    std::string name_sub = "Black Hole";
    std::string path_sub = "../assets/textures/black_hole.jpg";
    type = 0;
    textureID = loadTexture(path_sub.c_str());
    

    setName(name_sub);
    setPath(path_sub);

    //updatePositionSimulation();
}






