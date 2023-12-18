// Uranus.cpp
#include "engine/Planets/Uranus.h"
#include "opencv2/opencv.hpp"


Uranus::Uranus() : CelestialObject() {
    std::string name_sub = "Uranus";
    std::string path_sub = "../assets/textures/uranus.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(21,0,0);
    type = 4;
    textureID = loadTexture(path_sub.c_str());

    story = "\tUranus, an ice giant, is unique for rotating on its side, likely due to a massive collision long ago. Its blue color comes from methane in the atmosphere. Uranus has faint rings and 27 known moons. It's the coldest planetary atmosphere in the solar system, with minimum temperatures of -224°C.";


    distance_initiale = 2870658186e3;
    real_radius = 25362e3;
    weight = 8.681e25;
    initialVelocity = Vec3(0,0,-6.8e3);
    inclinaison = 97.77;
    rotationSid = 0;
    rotationSidSpeed = 0.0000161;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "84.01 years";
    temperature = "-216°C";

    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






