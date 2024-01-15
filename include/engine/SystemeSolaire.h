#ifndef SYSTEMESOLAIRE_H
#define SYSTEMESOLAIRE_H

#include "engine/Vec3.h"
#include "engine/CelestialObject.h"
#include "engine/Planets/planetsIncludes.h"
#include <vector> 
#include <utility> // Pour std::pair
#include "engine/DataAPI/ApiTool.h"

class RenderContext;
class SystemeSolaire{

public:
ApiTool* apiTool;
SystemeSolaire();
std::vector<CelestialObject*> objects;
std::vector<std::pair<CelestialObject*, size_t>> deletedObjects;
std::vector<CelestialObject*> initSystem();
double getScale();
double maxSize;
double scale;
float radiusScale;
//methods
void Draw();
void syncWithNasa();

void updatePhysics(double dt);
void setRayonInit();
void setRayon(CelestialObject* obj);
void resetPosition();
void addObject(CelestialObject* newObj);
void removeObject(CelestialObject* objToRemove);
void updateEffects(CelestialObject* obj);
RenderContext* m_renderContext;
void setContext(RenderContext* renderContext);
CelestialObject* getSun();

private:


};

#endif