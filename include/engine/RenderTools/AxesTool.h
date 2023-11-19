// UITool.h
#ifndef AXESTOOL_H
#define AXESTOOL_H

#include "RenderComponent.h"
#include "engine/Vec3.h"


class AxesTool : public RenderComponent {
public:
    float lineLength;

    explicit AxesTool(RenderContext* renderContext);
    void Draw() override;


private:


};

#endif // AxesTool_H