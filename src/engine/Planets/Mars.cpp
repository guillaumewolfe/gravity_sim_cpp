// Mars.cpp
#include "engine/Planets/Mars.h"
#include "opencv2/opencv.hpp"


Mars::Mars() : CelestialObject() {
    std::string name_sub = "Mars";
    std::string path_sub = "../assets/textures/mars.jpg";
    type = 2;
    textureID = loadTexture(path_sub.c_str());


    story = "\tMars, known as the Red Planet, has the largest volcano and the deepest, longest canyon in the solar system. It's home to polar ice caps made of water and dry ice. Mars experiences dust storms and has seasons like Earth due to its similar axial tilt. Evidence suggests it once had liquid water.";


    distance_initiale = 225000000e3;
    real_radius = 3390e3;
    weight = 6.417e23;
    initialVelocity = Vec3(0,0,-24e3);
    inclinaison = 25.19;
    rotationSid = 0;
    rotationSidSpeed = 0.0000113;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "1.88 years";
    temperature = "-125 to 20°C";

    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






