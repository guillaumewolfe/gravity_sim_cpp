#include "engine/SystemeSolaire.h"
#include <iostream>

SystemeSolaire::SystemeSolaire(){
    objects = initSystem();
    scale = getScale();
    setRayon();
    }

void SystemeSolaire::Draw(){
}


std::vector<CelestialObject*> SystemeSolaire::initSystem(){
    std::vector<CelestialObject*> celestialObjects;
    
    Earth* earth = new Earth();
    Sun* sun = new Sun();
    Mercure* mercure = new Mercure();
    Venus* venus = new Venus();
    Mars* mars = new Mars();
    Jupiter* jupiter = new Jupiter();
    Saturn* saturn = new Saturn();
    Uranus* uranus = new Uranus();
    Neptune* neptune = new Neptune();


    celestialObjects.push_back(sun);
    celestialObjects.push_back(mercure);
    celestialObjects.push_back(venus);
    celestialObjects.push_back(earth);
    celestialObjects.push_back(mars);
    celestialObjects.push_back(jupiter);
    celestialObjects.push_back(saturn);
    celestialObjects.push_back(uranus);
    celestialObjects.push_back(neptune);
    return celestialObjects;
}

void SystemeSolaire::updatePhysics(double dt){}

double SystemeSolaire::getScale(){
    double maxLength = 0;
    for (auto& object : objects){
        if(object->position_real.norm()>maxLength){maxLength = object->position_real.norm();}
    }
    scale = maxSize/maxLength;
    return scale;
}

void SystemeSolaire::setRayon(){
    for (auto& object : objects){
        object->setRayonSim(5*scale);
    }   
}

void SystemeSolaire::resetPosition(){
    return;
        for (auto& object : objects){
        std::cout<<object->getName()<<": "<<object->getPositionSimulation().print()<<" Rayon: "<<object->getRayon()<<std::endl;
}
    for (auto& object : objects){
        object->position_real = Vec3(object->distance_initiale,0,0);
        object->clearPositionHistory();
    }
}