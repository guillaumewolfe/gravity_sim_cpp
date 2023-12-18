// Mercure.cpp
#include "engine/Planets/Mercure.h"
#include "opencv2/opencv.hpp"


Mercure::Mercure() : CelestialObject() {
    std::string name_sub = "Mercury";
    std::string path_sub = "../assets/textures/mercure.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(3,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    story = "\tMercury, the closest planet to the Sun, completes an orbit in just 88 Earth days. With no atmosphere to retain heat, temperatures swing drastically. It's slightly larger than Earth's Moon and is covered in craters from cosmic impacts. Mercury's iron core takes up about 75% of its diameter, making it the most iron-rich planet in the solar system.";

    //Physics:
    distance_initiale = 57909175e3;
    real_radius = 2439.7e3;
    weight = 3.301e23;
    initialVelocity = Vec3(0,0,-47.87e3);
    inclinaison = 0.034;
    rotationSid = 0;
    rotationSidSpeed = 0.000000198;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "88 days";
    temperature = "-173 to 427°C"; 
    
    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






