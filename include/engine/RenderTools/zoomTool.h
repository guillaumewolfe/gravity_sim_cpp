// ZoomTool.h
#ifndef ZoomTool_H
#define ZoomTool_H


#include "RenderComponent.h"
#include "UI/ImageButton.h"
#include "UI/Labbel.h"
#include "engine/Camera/Camera.h"
#include "engine/Vec3.h"
#include "engine/RenderTools/backgroundTool.h"
#include "engine/RenderTools/objectsTool.h"
#include <functional>




class ZoomTool : public RenderComponent {
public:
   explicit ZoomTool(RenderContext* renderContext);
   ~ZoomTool() override;
   // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
   void Draw() override;
   void drawImGui();
   void Open();
   void Close();
   BackgroundTool* backgroundTool = nullptr;
   ObjectsTool* objectsTool = nullptr;
   Camera* zoomCamera = nullptr;
   void setCloseButtonFonction(const std::function<void()>& func);
    bool* escapeClicked = nullptr;



private:
   int winWidth, winHeight;
   int bufferWidth, bufferHeight;
   float angle_pespective;
   double zoomSpeed = 0.1;
   double zoom;
   


   Labbel* title = nullptr;
   Labbel* radius1 = nullptr;
   Labbel* radius1Static = nullptr;
   Labbel* radius2 = nullptr;
   Labbel* mass1 = nullptr;
   Labbel* mass1Static = nullptr;
   Labbel* mass2 = nullptr;
   Labbel* pourcentageMass = nullptr;
   Labbel* pourcentageRadius = nullptr;
   Labbel* selectLabbel1 = nullptr;
   Labbel* selectLabbel2 = nullptr;
   ImFont* nameFont = nullptr;
   Button* exitButton = nullptr;


   ImVec2 pos1Name = ImVec2(0, 0);
   ImVec2 pos2Name = ImVec2(0, 0);


   void drawCombo1();
   void drawCombo2();
   void updateLabels();
   void exitButtonFunction();




   CelestialObject* obj1 = nullptr;
   CelestialObject* obj2 = nullptr;
   CelestialObject* selectedObject = nullptr;


   void checkInputs();
   void checkScroll();

   std::function<void()> closeButtonFonction;


   Labbel* zoomLabbel = nullptr;
   void zoomIn(float zoom = 0.0f);
   void zoomOut(float zoom = 0.0f);
   void changeOjects();
   bool obj1IsBigger = true;


void updateZoomCameraTarget(Camera* zoomCamera, const glm::vec3& mouseDirection);


void updateCameraTargetWithMouse(int mouseX, int mouseY, int screenWidth, int screenHeight);
void drawCircularMask(int x, int y, int diameter);


float rectHeight, rectWidth;
void drawPlanets();
void generate_colors();
void drawPlanetEffect(ImVec2 planetPos, float radius, CelestialObject* obj);
void drawSunEffect(ImVec2 planetPos, float radius);
std::map<std::string, ImVec4> typeDictColor;


};


#endif // ZoomTool_H



