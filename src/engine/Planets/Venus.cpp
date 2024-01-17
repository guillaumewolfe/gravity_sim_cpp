// Venus.cpp
#include "engine/Planets/Venus.h"
#include "opencv2/opencv.hpp"


Venus::Venus() : CelestialObject() {
    std::string name_sub = "Venus";
    typeName = "Venus";
    std::string path_sub = "../assets/textures/venus.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(6,0,0);
    type = 2;
    textureID = loadTexture(path_sub.c_str());

    story = "\tVenus, second from the Sun, is often called Earth's twin due to its similar size and mass. However, it's radically different in climate. Venus' thick, toxic atmosphere, composed mainly of carbon dioxide with clouds of sulfuric acid, traps heat through a runaway greenhouse effect, making it the hottest planet with surface temperatures around 462°C. A day on Venus lasts 243 Earth days and is longer than its year, which is 225 Earth days. This slow rotation causes its surface to be marked by volcanic plains and numerous craters. Uniquely, Venus spins in the opposite direction to most planets, resulting in a sun that rises in the west and sets in the east. Its atmospheric pressure is 92 times that of Earth's, equivalent to being about 900 meters underwater on Earth. Radar mapping has revealed that Venus has more volcanoes than any other planet in the solar system.";

    //Physics:
    distance_initiale = 108208930e3;
    real_radius = 6051.8e3;
    weight = 4.867e24;
    initialVelocity = Vec3(0,0,-35.02e3);
    inclinaison = 177.4;
    rotationSid = 0;
    rotationSidSpeed = -0.0000000476;
    rotationSidDirection = Vec3(0,-1,0);
    orbitalString = "255 days";
    temperature = "462°C"; 

    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
    setDefaultValues();
}






