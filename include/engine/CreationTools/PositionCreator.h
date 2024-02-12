// UITool.h
#ifndef PositionCreator_H
#define PositionCreator_H

#include "engine/CreationTools/StateCreator.h"
#include "UI/ImageButton.h"
#include "UI/Icon.h"


class PositionCreator : public StateCreator {
public:


    explicit PositionCreator(RenderContext* renderContext, CreatorManager* manager);
    ~PositionCreator() override;
    void Draw() override;
    void Enter() override;
    void DrawOpenGL() override;
    void Exit() override;


    void drawBackground();
    void generate_buttons();
    void generate_labels();
    void draw_buttons();
    void draw_labels();
    void next_state();
    void drawOrbitGrids(const ImVec2& sunScreenPos, float maxRadius);
    void previous_state();
    void createNewObject();
    void removeNewObject();
    void DrawOrbits();
    void updatePositionWithMouse();
    void updateDistanceLabel();
    void AdjustPosition();
    void forceClose();
    void cameraHauteurMode();



private:
    int winWidth, winHeight;
    std::vector<Labbel*> labbels = std::vector<Labbel*>();
    std::vector<Button*> buttons = std::vector<Button*>();
    ImFont* nameFontBig = nullptr;
    ImFont* nameFont = nullptr;
    std::vector<ImageButton*> imageButtons;
    Icon* icon = nullptr;
    float hauteur, longueur;
    bool positionSelected = false;
    bool shouldUpdate = true;
    void checkClick();
    void checkInputs();

    ImVec2 bottomRight;
    ImVec2 topLeft;
    ImVec2 centerPos;
    ImVec2 bottomRightCamera;
    ImVec2 topLeftCamera;
    float longueurMidSquare, hauteurMidSquare;
    ImVec2 centerPosMidSquare;
    Vec3 calculateIntersection(const glm::vec3& rayDirection, const glm::vec3& cameraPosition);
    void zoomIn(float zoomFactor = 1.0f);
    void zoomOut(float zoomFactor = 1.0f);
    void moveUp(bool up);
    void moveRight(bool right);
    void resetCamPos();
    bool isHoveringRectangle();
    bool isWithinBounds(const ImVec2& objectPos, const ImVec2& objectSize);
    void createEffects();

    void drawPlanets();
    void drawMainBackground();
    void drawSunEffect(ImVec2 planetPos, float radius);
    void drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, CelestialObject* obj);
    void drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius);
    void draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments);
    void generate_colors();
    std::map<std::string, ImVec4> typeDictColor;
    CelestialObject* sun = nullptr;

    double maxZoomOut = 2476.94;
    double maxZoomIn;

    void drawControls();

    std::vector<Icon*> iconsControls = std::vector<Icon*>();



};

#endif // PositionCreator_H