// Uranus.cpp
#include "engine/Planets/Uranus.h"
#include "opencv2/opencv.hpp"


Uranus::Uranus() : CelestialObject() {
    std::string name_sub = "Uranus";
    typeName = "Uranus";
    std::string path_sub = "../assets/textures/uranus.jpg";
    double rayon_sub = 1;
    Vec3 position_initiale_sub = Vec3(21,0,0);
    type = 4;
    textureID = loadTexture(path_sub.c_str());

    story = "\tUranus, the seventh planet from the Sun, is an ice giant that stands out for its unusual rotation on its side. This unique tilt of approximately 98 degrees relative to its orbit is likely the result of a massive collision with an Earth-sized object long ago. The planet's distinct pale blue color is due to methane in its atmosphere, which absorbs red light and reflects blue. Uranus has thirteen faint rings, composed mainly of extremely dark particles, and 27 known moons. Among the coldest planets, Uranus holds the record for the coldest planetary atmosphere in the solar system, with minimum temperatures plummeting to -224°C. This extreme cold is due to its long distance from the Sun and its lack of an internal heat source.";


    distance_initiale = 2870658186e3;
    real_radius = 25362e3;
    weight = 8.681e25;
    initialVelocity = Vec3(0,0,-6.8e3);
    inclinaison = 97.77;
    rotationSid = 0;
    rotationSidSpeed = 0.0000161;
    rotationSidDirection = Vec3(0,1,0);
    orbitalString = "84.01 years";
    temperature = "-224°C";

    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    updatePositionSimulation();
    setDefaultValues();
}






