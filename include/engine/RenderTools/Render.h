
#include "engine/RenderTools/RenderComponent.h"
#include "engine/RenderTools/RenderContext.h"

#include "engine/RenderTools/backgroundTool.h"
#include "engine/RenderTools/objectsTool.h"
#include "engine/RenderTools/UITool.h"
#include "UI/MessageTool.h"

#ifndef RENDER_H
#define RENDER_H


class Render {
private:

    /// Methodes
    RenderContext* Context;

    //Tools

    UITool* UI_Tool;
    ObjectsTool* Objects_Tool;
    BackgroundTool* Background_Tool;


//Background:

public:

MessageTool* Message_Tool;
Render(RenderContext* Context);
void Draw();
void initTools();
void updateCamera();
void initCamera();
};


#endif