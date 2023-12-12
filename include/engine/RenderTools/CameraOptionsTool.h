// UITool.h
#ifndef CameraOptionsTool_H
#define CameraOptionsTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include <map>

class CameraOptionsTool : public RenderComponent {
public:
    explicit CameraOptionsTool(RenderContext* renderContext);
    void Draw() override;


private:

    void drawBackground();
    void generate_buttons();
    void generate_labels();
    void draw_buttons();
    void draw_labels();
    void update_label();

    void resetCamPos();
    void ZoomIn();
    void ZoomOut();
    void previousMode();
    void nextMode();

    void initcameraModeList();


    ImVec2 topLeft;
    ImVec2 centerPos;
    int winWidth, winHeight;

    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;

    int indexMode;
    std::map<int, std::string> cameraMode;


};

#endif // CameraOptionsTool_H