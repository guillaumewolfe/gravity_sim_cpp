
#ifndef NameTool_H
#define NameTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include "UI/Labbel.h"


class NameTool : public RenderComponent {
public:

    explicit NameTool(RenderContext* renderContext);
    void Draw() override;
    void initLabbels();
    void updateLabelPositions();
    void synchronizeLabels();
    float calculateCircleRadius(float planetRadius, float distanceToCamera, const glm::mat4& projectionMatrix);



private:

std::vector<Labbel*> labbels;
float maxDelta = 0.05f;
float verticalOffset = 0.01;
float alpha;
ImFont* customFont;

void detectClickAndPrintName();
glm::vec2 convert3DPosToScreenPos(const glm::vec3& pos3D, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, int screenWidth, int screenHeight);



};

#endif // NameTool_H