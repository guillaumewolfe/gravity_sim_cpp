// Moon.cpp
#include "engine/Planets/Moon.h"
#include "opencv2/opencv.hpp"


Moon::Moon() : CelestialObject() {
    std::string name_sub = "Moon";
    typeName = "Moon";
    std::string path_sub = "../assets/textures/lunar.jpg";
    type = 5;
    textureID = loadTexture(path_sub.c_str());


    story = "\tEarth's Moon, our planet's only natural satellite, ranks as the fifth largest moon in the solar system. It's about 1/4th the size of Earth, with a diameter of 3,474 kilometers (2,159 miles). The Moon's gravitational influence is a major factor in Earth's tides and has a stabilizing effect on Earth's axial tilt, impacting our climate. Its phases, from new moon to full moon, not only illuminate our night sky but have also been crucial in the development of calendars. The Moon's surface features craters, plains (maria), and mountains, showcasing ancient volcanic activity and cosmic impacts. The far side of the Moon is always facing away from Earth due to tidal locking and remains unseen from our planet. The Moon is believed to have formed about 4.5 billion years ago, likely due to a massive impact between Earth and a Mars-sized body.";



    //Physics:
    distance_initiale = (149597870+384400)*1e3;
    real_radius = 1737e3;
    weight = 7.342e22;
    initialVelocity = Vec3(88.75,1022,-29.78e3);
    inclinaison = 5.14;
    rotationSid = 0;
    rotationSidSpeed = 0.000000424;
    rotationSidDirection = Vec3(0,1,0);
    distanceFromPlanet = 384400;
    temperature = "-173 to 127°C";
    showName = false;
    showOrbit = false;
    showPath = false;

    setName(name_sub);
    setPath(path_sub);
    updateVelocity(initialVelocity);
    updatePositionReal(Vec3(distance_initiale,0,0));
    //updatePositionSimulation();
}






