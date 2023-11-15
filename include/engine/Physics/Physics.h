// UITool.h
#ifndef PHYSICS_H
#define PHYSICS_H

#include "engine/RenderTools/RenderContext.h"
#include "engine/SystemeSolaire.h"

class Physics{
public:
    RenderContext* m_renderContext;

    Physics(RenderContext* renderContext);
    void Update(double dt);

private:
    void updateAccel(CelestialObject* obj, double dt);
    void updateVelocity(CelestialObject* obj, double dt);
    void updatePosition(CelestialObject* obj, double dt);
    void updateRotaton(CelestialObject* obj, double dt);
    void updateRotation(CelestialObject* obj, double dt);
    

    const double G = 6.67430e-11;

};

#endif // OBJECTSTOOL_H