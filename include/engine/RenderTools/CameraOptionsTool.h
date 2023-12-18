// UITool.h
#ifndef CameraOptionsTool_H
#define CameraOptionsTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include <map>
#include "UI/Icon.h"
#include "UI/ImageButton.h"
#include "UI/Slider.h"
#include "UI/ToggleButton.h"
#include <functional>


class CameraOptionsTool : public RenderComponent {
public:
    explicit CameraOptionsTool(RenderContext* renderContext);
    void Draw() override;
    void setCloseButtonFunction(const std::function<void()>& func);
    void Open();

private:

    void drawBackground();
    void generate_buttons();
    void generate_labels();
    void draw_UI();
    void draw_mode1();
    void draw_mode2();
    void drawDropDownMenusMode2();
    void draw_mode3();
    void update_label();

    void resetCamPos();
    void ZoomIn();
    void ZoomOut();
    void setMode(int newMode);
    void updateButtonMode();
    void globalViewButton();
    void changeFollowObject(CelestialObject* obj);
    void changeFirstPersonTarget(CelestialObject* targetObject);

    void initcameraModeList();
    void closeWindow();


    ImVec2 topLeft;
    ImVec2 centerPos;
    int winWidth, winHeight;

    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
    std::vector<ImageButton*> imageButtons;
    std::vector<Slider*> sliders;
    std::vector<Icon*> icons;
    std::vector<ToggleButton*> toggleButtons;

    std::vector<ImageButton*> imageButtonsMode1;
    std::vector<Labbel*> labbelsMode1;
    std::vector<ImageButton*> imageButtonsMode2;
    std::vector<Labbel*> labbelsMode2;
    std::vector<ToggleButton*> toggleButtonsMode2;
    std::vector<ImageButton*> imageButtonsMode3;
    std::vector<Labbel*> labbelsMode3;

    int mode = 1;
    std::map<int, std::string> cameraMode;
    bool focusOnAxis = false;

    
    std::function<void()> closeButtonFunction;


};

#endif // CameraOptionsTool_H