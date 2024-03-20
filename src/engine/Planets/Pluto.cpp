// Pluto.cpp
#include "engine/Planets/Pluto.h"
#include "opencv2/opencv.hpp"


Pluto::Pluto() : CelestialObject() {
    std::string name_sub = "Pluto";
    typeName = "Pluto";
    std::string path_sub = "../assets/textures/pluto.png";
    type = 2;
    textureID = loadTexture(path_sub.c_str());


    story = "\tPluto, known as the Red Planet, has the largest volcano and the deepest, longest canyon in the solar system. It's home to polar ice caps made of water and dry ice. Pluto experiences dust storms and has seasons like Earth due to its similar axial tilt. Evidence suggests it once had liquid water.";


    distance_initiale = 5.906e12;
    real_radius = 1188e3;
    weight = 1.303e22;  
    initialVelocity = Vec3(0,0,0);
    inclinaison = 25.19;
    rotationSid = 0;
    rotationSidSpeed = 0.001;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "248 years";
    temperature = "-225°C to -240°C";

    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
    setDefaultValues();
}






