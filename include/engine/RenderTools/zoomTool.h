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
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void drawImGui();
    void Open();
    BackgroundTool* backgroundTool;
    ObjectsTool* objectsTool;
    void setCloseButtonFonction(const std::function<void()>& func);


private:
    int winWidth, winHeight;
    int bufferWidth, bufferHeight;
    Camera* zoomCamera;
    float angle_pespective;
    double zoomSpeed = 0.1;
    double zoom;

    void checkInputs();
    std::function<void()> closeButtonFonction;

    Labbel* zoomLabbel;
    void zoomIn();
    void zoomOut();
    void Close();

void updateZoomCameraTarget(Camera* zoomCamera, const glm::vec3& mouseDirection);

void updateCameraTargetWithMouse(int mouseX, int mouseY, int screenWidth, int screenHeight);
void drawCircularMask(int x, int y, int diameter);
};

#endif // ZoomTool_H