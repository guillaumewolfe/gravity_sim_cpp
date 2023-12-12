#include "engine/SystemeSolaire.h"
#include <iostream>
#include "engine/RenderTools/objectsTool.h"

SystemeSolaire::SystemeSolaire(){
    apiTool = new ApiTool();
    objects = initSystem();
    scale = getScale();
    setRayonInit();
    syncWithNasa();
    //for (auto& object : objects){std::cout<<object->getName()<<" : "<<object->getRayon()<<std::endl;}
    }



std::vector<CelestialObject*> SystemeSolaire::initSystem(){
    std::vector<CelestialObject*> celestialObjects;
    
    Sun* sun = new Sun();
    Mercure* mercure = new Mercure();
    Venus* venus = new Venus();
    Earth* earth = new Earth();
    Moon* moon = new Moon();
    Mars* mars = new Mars();
    Jupiter* jupiter = new Jupiter();
    Saturn* saturn = new Saturn();
    Uranus* uranus = new Uranus();
    Neptune* neptune = new Neptune();
    


    celestialObjects.push_back(sun);
    celestialObjects.push_back(mercure);
    celestialObjects.push_back(venus);
    celestialObjects.push_back(earth);
    celestialObjects.push_back(moon);
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

void SystemeSolaire::setRayonInit(){
    for (auto& object : objects){
        object->setRayonSim(2*scale);
        object->distanceScale = scale;
    }   
}
void SystemeSolaire::setRayon(CelestialObject* obj){
    obj->setRayonSim(2*scale);
    ObjectsTool::initSphere(*obj, 150, 150);
}


void SystemeSolaire::resetPosition(){
        for (auto& object : objects){
        if(object->isCreated){removeObject(object);continue;}
        object->updatePositionReal(object->nasaPosition);
        object->updateVelocity(object->nasaVelocity);
        object->clearPositionHistory();
}
}

void SystemeSolaire::syncWithNasa(){
    if(objects.empty()){return;}
    std::string sunString = apiTool->getBodyData("Sun");
    std::pair<Vec3, Vec3> sunPositionAndVelocity = apiTool->extractBodyData(sunString);
    Vec3 sunPosition = sunPositionAndVelocity.first;
    Vec3 sunVelocity = sunPositionAndVelocity.second;
    for(auto& object : objects){
        std::string buffer = apiTool->getBodyData(object->getName());
        std::pair<Vec3, Vec3> positionAndVelocity = apiTool->extractBodyData(buffer);
        // Multipliez les valeurs par 1000
        Vec3 newPosition = positionAndVelocity.first * 1000;
        Vec3 newVelocity = positionAndVelocity.second * 1000;
        object->nasaPosition = newPosition;
        object->nasaVelocity = newVelocity;
    }    
    
}


void SystemeSolaire::addObject(CelestialObject* newObj){
    if(newObj->real_radius){
    setRayon(newObj);
    }else{
        std::cout<<"NO RAYON"<<std::endl;
    }
    objects.push_back(newObj);
}

void SystemeSolaire::removeObject(CelestialObject* objToRemove){
    auto it = std::find(objects.begin(), objects.end(), objToRemove);

    if (it != objects.end()) {
        objects.erase(it); // Enlever l'objet de la liste
    } else {
        std::cout << "Objet non trouvé." << std::endl;
    }

}