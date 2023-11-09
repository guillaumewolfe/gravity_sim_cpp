
#include "engine/RenderTools/RenderComponent.h"
#include "engine/RenderTools/RenderContext.h"

#include "engine/RenderTools/backgroundTool.h"
#include "engine/RenderTools/objectsTool.h"
#include "engine/RenderTools/UITool.h"

#ifndef RENDER_H
#define RENDER_H


class Render {
private:

    RenderContext* Context;
    void setBackground();

    //Tools

    UITool* UI_Tool;


//Background:

public:

Render(RenderContext* Context);
void Draw();
void initTools();
};


#endif