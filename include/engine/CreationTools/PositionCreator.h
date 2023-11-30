// UITool.h
#ifndef PositionCreator_H
#define PositionCreator_H

#include "engine/CreationTools/StateCreator.h"


class PositionCreator : public StateCreator {
public:


    explicit PositionCreator(RenderContext* renderContext, CreatorManager* manager);

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
    void previous_state();
    void createNewObject();
    void removeNewObject();
    void DrawOrbits();
    void updatePositionWithMouse();
    void updateDistanceLabel();
    void AdjustPosition();



private:
    int winWidth, winHeight;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
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
    Vec3 calculateIntersection(const glm::vec3& rayDirection, const glm::vec3& cameraPosition);
    void zoomIn();
    void zoomOut();
    void moveUp(bool up);
    void moveRight(bool right);
    void resetCamPos();
    bool isHoveringRectangle();

    double maxZoomOut = 2476.94;
    double maxZoomIn;


};

#endif // PositionCreator_H