#ifndef SYSTEMESOLAIRE_H
#define SYSTEMESOLAIRE_H

#include "engine/Vec3.h"
#include "engine/CelestialObject.h"
#include "engine/Planets/planetsIncludes.h"
#include <vector> 

class SystemeSolaire{

public:

SystemeSolaire();
std::vector<CelestialObject*> objects;
std::vector<CelestialObject*> initSystem();
double getScale();
double maxSize = 1500;
double scale;
//methods
void Draw();

void updatePhysics(double dt);
void setRayonInit();
void setRayon(CelestialObject* obj);
void resetPosition();
void addObject(CelestialObject* newObj);
void removeObject(CelestialObject* objToRemove);


private:


};

#endif