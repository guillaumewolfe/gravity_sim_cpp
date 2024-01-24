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
ApiTool* apiTool = nullptr;
SystemeSolaire();
std::vector<CelestialObject*> objects = std::vector<CelestialObject*>();

std::vector<std::pair<CelestialObject*, size_t>> deletedObjects = std::vector<std::pair<CelestialObject*, size_t>>();
std::vector<CelestialObject*> initSystem();
double getScale();
double maxSize = 0.0f;
double scale = 1.0f;
float radiusScale = 1.0f;
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
RenderContext* m_renderContext = nullptr;
void setContext(RenderContext* renderContext);
CelestialObject* getSun(CelestialObject* exclude = nullptr);

private:


};

#endif