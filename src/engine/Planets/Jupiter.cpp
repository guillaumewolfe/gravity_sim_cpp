// Jupiter.cpp
#include "engine/Planets/Jupiter.h"
#include "opencv2/opencv.hpp"


Jupiter::Jupiter() : CelestialObject() {
    std::string name_sub = "Jupiter";
    typeName = "Jupiter";
    std::string path_sub = "../assets/textures/jupiter.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(15,0,0);
    type = 3;
    textureID = loadTexture(path_sub.c_str());

    story = "\tJupiter, the largest planet, is a gas giant with no solid surface. Its Great Red Spot, a massive storm, has raged for centuries. This enormous storm is about 1.3 times the width of Earth. Jupiter's fast rotation creates strong jet streams, separating its clouds into dark belts and bright zones. It has over 79 moons, including Ganymede, the largest moon in the solar system, which is bigger than the planet Mercury.";


    //Physics:
    distance_initiale = 778340821e3;
    real_radius = 69911e3;
    weight = 1.898e27;
    initialVelocity = Vec3(0,0,-13.07e3);
    inclinaison = 3.13;
    rotationSid = 0;
    rotationSidSpeed = 0.000028;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "11.86 years";
    temperature = "-145°C";



    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






