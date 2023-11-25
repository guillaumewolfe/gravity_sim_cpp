
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


};

#endif // NameTool_H