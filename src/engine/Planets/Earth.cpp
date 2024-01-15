// Earth.cpp
#include "engine/Planets/Earth.h"
#include "opencv2/opencv.hpp"


Earth::Earth() : CelestialObject() {
    std::string name_sub = "Earth";
    typeName = "Earth";
    std::string path_sub = "../assets/textures/earth_real.jpg";
    type = 2;
    textureID = loadTexture(path_sub.c_str());
    //TO CHANGE
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(9,0,0);
    
    story = "\tEarth, the third planet from the Sun, is the only known place with life. Its unique atmosphere and liquid water support diverse ecosystems. Earth's rotation on its tilted axis causes the seasons. The only planet not named after a mythological figure, its surface is 70% water, with the rest being continents and islands.";

    //Physics:
    distance_initiale = 149597870e3;
    real_radius = 6371e3;
    weight = 5.972e24;
    initialVelocity = Vec3(0,0,-29.78e3);
    inclinaison = 23.5;
    rotationSid = 0;
    rotationSidSpeed = 0.0000116;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "1 year";
    temperature = "-88 to 58°C";


    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    //updatePositionSimulation();
}






