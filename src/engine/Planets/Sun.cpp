// Sun.cpp
#include "engine/Planets/Sun.h"
#include "opencv2/opencv.hpp"


Sun::Sun() : CelestialObject() {
    std::string name_sub = "Sun";
    std::string path_sub = "../assets/textures/sun.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(0,0,0);
    type = 1;
    textureID = loadTexture(path_sub.c_str());

    story = "\tFormed 4.6 billion years ago, the Sun, made mostly of hydrogen and helium, fuels itself through nuclear fusion. Spanning 109 Earth diameters, it holds 99.8% of the solar system's mass. In about 5 billion years, as it exhausts its hydrogen, the Sun will become a red giant, potentially swallowing Earth. It'll then form a planetary nebula, leaving a cooling white dwarf, its final state over the next trillions of years.";


    //Physics:
    distance_initiale = 0;
    real_radius = 696340e3;
    weight = 1.989e30;
    initialVelocity = Vec3(0,0,0);
    inclinaison = 7.25;
    rotationSid = 0;
    rotationSidDirection = Vec3(0,1,0);
    rotationSidSpeed = 1.0/2116800.0;
    temperature = "5500°C"; 

    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
}






