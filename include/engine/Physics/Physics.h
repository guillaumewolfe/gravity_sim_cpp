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
    void setCollisionFunction(const std::function<void(CelestialObject*, CelestialObject*)>& func);
    void setEndCollisionFunction(const std::function<void(CelestialObject*, CelestialObject*)>& func);
    
private:
    void updateAccel(CelestialObject* obj, double dt);
    void updateVelocity(CelestialObject* obj, double dt);
    void updatePosition(CelestialObject* obj, double dt);
    void updateRotaton(CelestialObject* obj, double dt);
    void updateRotation(CelestialObject* obj, double dt);
    void checkIfTooFar(CelestialObject* obj);

    void sendMessageCollsion(CelestialObject* obj1, CelestialObject* obj2, CelestialObject* objectRestant);
    

    const double G = 6.67430e-11;

    //Collisions
    std::function<void(CelestialObject*, CelestialObject*)> collisionForToolFunction;
    std::function<void(CelestialObject*, CelestialObject*)> endCollisionForToolFunction;
    void checkCollision(std::vector<CelestialObject*> objects);
    void collision(CelestialObject* obj1, CelestialObject* obj2);
    bool isColliding;
};

#endif // OBJECTSTOOL_H