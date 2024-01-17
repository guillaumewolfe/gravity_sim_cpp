// Neptune.cpp
#include "engine/Planets/Neptune.h"
#include "opencv2/opencv.hpp"


Neptune::Neptune() : CelestialObject() {
    std::string name_sub = "Neptune";
    typeName = "Neptune";
    std::string path_sub = "../assets/textures/neptune.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(24,0,0);
    type = 4;
    textureID = loadTexture(path_sub.c_str());


   story = "\tNeptune, the eighth and farthest known planet from the Sun, is an ice giant like Uranus. This distant planet is notable for having the strongest winds in the solar system, with speeds reaching up to 2,100 kilometers per hour (1,300 miles per hour), which are even faster than the speed of sound on Earth. Neptune's deep blue color, more intense than Uranus, is also due to the presence of atmospheric methane, which absorbs red light. One of its most intriguing features is the Great Dark Spot, a high-pressure storm system similar to Jupiter's Great Red Spot, but it appears to be less stable, appearing and disappearing over several years. Neptune has 14 known moons, with Triton being the largest and geologically active, displaying geysers of nitrogen ice. Triton is also unique as it orbits Neptune in the opposite direction of the planet's rotation, suggesting it may have been captured by Neptune's gravity. The planet's long orbital period means that a year on Neptune lasts about 165 Earth years.";


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
    temperature = "-214°C";
    
    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
    setDefaultValues();
}






