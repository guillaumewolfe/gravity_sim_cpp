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
    ~CameraOptionsTool() override;
    void Draw() override;
    void setCloseButtonFunction(const std::function<void()>& func);
    void Open();
    void Close();
    bool isClosed = false;
    bool mouseOnCameraOptions();

private:
    float posX;
    float longueur, hauteur;

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
    void followObjectButton();
    void POVButton();
    CelestialObject* POVtarget = nullptr;
    CelestialObject* getNewPOVtarget();
    void changeFollowObject(CelestialObject* obj);
    void changeFirstPersonTarget(CelestialObject* targetObject);

    void initcameraModeList();
    void closeWindow();


    ImVec2 topLeft;
    ImVec2 centerPos;
    int winWidth, winHeight;
    ImFont* nameFont = nullptr;
    ImFont* nameFontBig = nullptr;

    std::vector<Labbel*> labbels = std::vector<Labbel*>();
    std::vector<Button*> buttons,buttonsMode2 = std::vector<Button*>();
    std::vector<ImageButton*> imageButtons = std::vector<ImageButton*>();
    std::vector<Slider*> sliders = std::vector<Slider*>();
    std::vector<Icon*> icons,iconsMode1,iconsMode2,iconsMode3 = std::vector<Icon*>();
    std::vector<ToggleButton*> toggleButtons = std::vector<ToggleButton*>();

    std::vector<ImageButton*> imageButtonsMode1 = std::vector<ImageButton*>();
    std::vector<Labbel*> labbelsMode1 = std::vector<Labbel*>();
    std::vector<ImageButton*> imageButtonsMode2 = std::vector<ImageButton*>();
    std::vector<Labbel*> labbelsMode2 = std::vector<Labbel*>();
    std::vector<ToggleButton*> toggleButtonsMode2 = std::vector<ToggleButton*>();
    std::vector<ImageButton*> imageButtonsMode3 = std::vector<ImageButton*>();
    std::vector<Labbel*> labbelsMode3 = std::vector<Labbel*>();

    int mode = 1;
    std::map<int, std::string> cameraMode;
    bool focusOnAxis = false;

    std::map<std::string, ImVec4> typeDictColor;
    void generate_colors();
    void draw_planetsMode3();
    void drawSunEffect(ImVec2 planetPos, float radius);
    void drawPlanetLight(ImVec2 planetPos, float radius, CelestialObject* planet);
    void resetViewAxis();
    void updateSliderPosition();
    void draw_global_following();
    void draw_focus_following();
    void draw_combo(float x1, float y1, float x2, float y2);

    void draw_POV_following();
    std::function<void()> closeButtonFunction;

    bool isTransitioning = false;
    float isClosing = false;
    float transitionProgress;

    bool isGlobalView = false;
    bool isFollowingObject = false;
    bool isFollowingFirstPerson = false;
    
    void focusOnAxisButton(std::string axis);


};

#endif // CameraOptionsTool_H