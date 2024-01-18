// UITool.h
#ifndef OrbitTool_H
#define OrbitTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"


class OrbitTool : public RenderComponent {
public:

    explicit OrbitTool(RenderContext* renderContext);
    void Draw() override;
    void init();
    void DrawEllipse(const Vec3& center, float a, float b, const Vec3& axis, float angle, CelestialObject* object);
    CelestialObject* mostInfluentialObject(CelestialObject* object);
    double calculateGravitationalForce(CelestialObject* obj1, CelestialObject* obj2);


private:
    std::map<int, std::pair<std::string, ImVec4>> typeDict;
    ImVec4 getTypeColor(int type);


};

#endif // OrbitTool_H