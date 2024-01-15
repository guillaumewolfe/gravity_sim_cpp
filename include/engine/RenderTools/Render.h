
#include "engine/RenderTools/RenderComponent.h"
#include "engine/RenderTools/RenderContext.h"

#include "engine/RenderTools/backgroundTool.h"
#include "engine/RenderTools/objectsTool.h"
#include "engine/RenderTools/UITool.h"
#include "engine/RenderTools/AxesTool.h"
#include "engine/RenderTools/PathTool.h"
#include "engine/EngineIncludes.h"
#include "UI/MessageTool.h"
#include "engine/CreationTools/CreatorManager.h"
#include "engine/RenderTools/namesTool.h"
#include "engine/RenderTools/planeteInfoTool.h"
#include "engine/RenderTools/CameraOptionsTool.h"
#include "engine/RenderTools/OptionsTool.h"
#include "engine/RenderTools/settingsTool.h"
#include "engine/RenderTools/zoomTool.h"
#include "engine/RenderTools/orbitTool.h"
#include "engine/RenderTools/minimapTool.h"
#include "engine/RenderTools/keysUI.h"
#include "engine/RenderTools/collisionTool.h"

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
PathTool* Path_Tool;
CreatorManager* Creator_Manager;
MessageTool* Message_Tool;
NameTool* Name_Tool;
PlaneteInfoTool* PlaneteInfo_Tool;
CameraOptionsTool* CameraOptions_Tool;
OptionsTool* Options_Tool;
SettingsTool* Settings_Tool;
ZoomTool* Zoom_Tool;
OrbitTool* Orbit_Tool;
MinimapTool* Minimap_Tool;
KeysUI* Keys_UI;
CollisionTool* Collision_Tool;




Render(RenderContext* Context);
~Render();
void Draw();
void initTools();
void updateCamera();
void initCamera();
};


#endif