
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



private:

std::vector<Labbel*> labbels;
float maxDelta = 0.05f;
float verticalOffset = 0.01;
float alpha;



};

#endif // NameTool_H