
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
#include "engine/RenderTools/notificationTool.h"
#include "engine/RenderTools/backgroundImageTool.h"

#ifndef RENDER_H
#define RENDER_H


class Render {
private:



//Background:

public:
/// Methodes
RenderContext* Context = nullptr;

//Tools

UITool* UI_Tool = nullptr;
ObjectsTool* Objects_Tool = nullptr;
BackgroundTool* Background_Tool = nullptr;
AxesTool* Axes_Tool = nullptr;
PathTool* Path_Tool = nullptr;
CreatorManager* Creator_Manager = nullptr;
MessageTool* Message_Tool = nullptr;
NameTool* Name_Tool = nullptr;
PlaneteInfoTool* PlaneteInfo_Tool = nullptr;
CameraOptionsTool* CameraOptions_Tool = nullptr;
OptionsTool* Options_Tool = nullptr;
SettingsTool* Settings_Tool = nullptr;
ZoomTool* Zoom_Tool = nullptr;
OrbitTool* Orbit_Tool = nullptr;
MinimapTool* Minimap_Tool = nullptr;
KeysUI* Keys_UI = nullptr;
CollisionTool* Collision_Tool = nullptr;
NotificationTool* Notification_Tool = nullptr;
BackgroundImageTool* BackgroundImage_Tool = nullptr;




Render(RenderContext* Context);
~Render();
void Draw();
void initTools();
void updateCamera();
void initCamera();
void updateShouldClick();
};


#endif