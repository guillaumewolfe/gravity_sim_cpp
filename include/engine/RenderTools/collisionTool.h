// UITool.h
#ifndef CollisionTool_H
#define CollisionTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"

struct ParticleCollision {
    Vec3 position; // Position de la particule
    Vec3 velocity; // Vitesse de la particule
    float distanceDestruction;
    Vec3 color;
    std::vector<Vec3> previousPositions;
};
class CollisionTool : public RenderComponent {
public:
    float lineLength;

    explicit CollisionTool(RenderContext* renderContext);
    void newCollision(CelestialObject* obj1, CelestialObject* obj2);
    void endCollision(CelestialObject* obj1, CelestialObject* obj2);
    void Draw() override;
    void reset();



private:
    CelestialObject* obj1 = nullptr;
    CelestialObject* obj2 = nullptr;
    std::vector<ParticleCollision> particles;
    void GenerateParticle();
    void computeFactor();
    float lastUpdateTime;
    void updateParticles();
    Vec3 midpoint;
    Vec3 normal;
    float facteur1,facteur2;
    float particleSpeedFactor;
    float particlesPerSecond;
    float lastParticleGenerationTime;
    bool shouldGenerateParticle;
    void drawCircleIntersection();
    float distanceMiddle;
    

std::vector<Vec3> explosionColors = {
    Vec3(1.0f, 0.9f, 0.9f),          // Orange Vif
    Vec3(1.0f, 0.9f, 0.75f),          // Orange Moyen
    Vec3(1.0,1.0,0.7), // Jaune de Flamme
    };
};

#endif // CollisionTool_H