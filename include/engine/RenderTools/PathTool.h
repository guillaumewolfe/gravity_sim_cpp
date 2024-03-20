// UITool.h
#ifndef PathTOOL_H
#define PathTOOL_H

#include "RenderComponent.h"
#include "engine/Vec3.h"


class PathTool : public RenderComponent {
public:

    explicit PathTool(RenderContext* renderContext);
    ~PathTool() override;
    void Draw() override;



private:
    int winWidth, winHeight;


};

#endif // PathTool_H