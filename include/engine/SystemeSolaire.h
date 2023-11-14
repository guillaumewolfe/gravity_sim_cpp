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
//methods
void Draw();

void updatePhysics(double dt);


private:


};

#endif