
// Saturn.cpp
#include "engine/Planets/Saturn.h"
#include "opencv2/opencv.hpp"


Saturn::Saturn() : CelestialObject() {
    std::string name_sub = "Saturn";
    typeName = "Saturn";
    std::string path_sub = "../assets/textures/saturn.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(18,0,0);
    type = 3;
    textureID = loadTexture(path_sub.c_str());

    story = "\tSaturn is famous for its stunning rings, made primarily of ice particles with a smaller amount of rocky debris and dust. Spanning up to 282,000 kilometers (175,000 miles) in width, these rings could fit nearly 75 Earths lined up in a row. Despite their grand scale, the rings are surprisingly thin, often just about 10 meters (33 feet) thick. It's a gas giant like Jupiter, with the most extensive ring system of any planet. Saturn's moon Titan, larger than Mercury, has a thick atmosphere and liquid methane lakes.";


    distance_initiale = 1426666422e3;
    real_radius = 58232e3;
    weight = 5.683e26;
    initialVelocity = Vec3(0,0,-9.68e3);
    inclinaison = 26.73;
    rotationSid = 0;
    rotationSidSpeed = 0.0000261;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "29.46 years";
    temperature = "-178°C";

    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
    setDefaultValues();
}






