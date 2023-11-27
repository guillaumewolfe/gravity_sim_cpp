
#include "engine/RenderTools/RenderComponent.h"
#include "engine/RenderTools/RenderContext.h"

#include "engine/RenderTools/backgroundTool.h"
#include "engine/RenderTools/objectsTool.h"
#include "engine/RenderTools/UITool.h"
#include "engine/RenderTools/AxesTool.h"
#include "engine/RenderTools/PathTool.h"
#include "engine/EngineIncludes.h"
#include "UI/MessageTool.h"
#include "engine/CreationTools/TextureCreator.h"
#include "engine/RenderTools/namesTool.h"
#include "engine/RenderTools/planeteInfoTool.h"


#ifndef RENDER_H
#define RENDER_H


class Render {
private:



//Background:

public:
/// Methodes
RenderContext* Context;

//Tools

UITool* UI_Tool;
ObjectsTool* Objects_Tool;
BackgroundTool* Background_Tool;
AxesTool* Axes_Tool;
SystemeSolaire* SystemeSolaire_Tool;
PathTool* Path_Tool;
TextureCreator* Creator_Tool;
MessageTool* Message_Tool;
NameTool* Name_Tool;
PlaneteInfoTool* PlaneteInfo_Tool;


Render(RenderContext* Context);
void Draw();
void initTools();
void updateCamera();
void initCamera();
};


#endif