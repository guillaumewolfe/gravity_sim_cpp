// Neptune.cpp
#include "engine/Planets/Neptune.h"
#include "opencv2/opencv.hpp"


Neptune::Neptune() : CelestialObject() {
    std::string name_sub = "Neptune";
    std::string path_sub = "../assets/textures/neptune.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(24,0,0);
    type = 4;
    textureID = loadTexture(path_sub.c_str());


    story = "\tNeptune, the farthest known planet, is an ice giant like Uranus. It has the strongest winds in the solar system, reaching supersonic speeds. Neptune is characterized by its deep blue color, due to atmospheric methane. Its Great Dark Spot, similar to Jupiter's storm, comes and goes. Neptune has 14 known moons.";


    //Physics:
    distance_initiale = 4498396441e3;
    real_radius = 24622e3;
    weight = 1.024e26;
    initialVelocity = Vec3(0,0,-5.43e3);
    inclinaison = 28.32;
    rotationSid = 0;
    rotationSidSpeed = 0.0000172;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "164.8 years";

    
    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






