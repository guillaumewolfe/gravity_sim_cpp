// UITool.h
#ifndef VelocityCreator_H
#define VelocityCreator_H

#include "engine/CreationTools/StateCreator.h"
#include "UI/Slider.h"
#include "UI/ImageButton.h"
#include "UI/Icon.h"

class VelocityCreator : public StateCreator {
public:


    explicit VelocityCreator(RenderContext* renderContext, CreatorManager* manager);

    void Draw() override;
    void Enter() override;
    void DrawOpenGL() override;
    void Exit() override;

    void drawBackground();
    void generate_buttons();
    void generate_labels();
    void generate_sliders();
    void draw_sliders();
    void draw_buttons();
    void draw_labels();
    void next_state();
    void previous_state();
 



private:
    int winWidth, winHeight;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
    std::vector<Slider*> sliders;
    std::vector<ImageButton*> imageButtons;
    Icon* icon;
    float hauteur, longueur;

    ImVec2 bottomRight;
    ImVec2 topLeft;
    ImVec2 centerPos;
    void DrawOrbits();
    Vec3 calculateIntersection(const glm::vec3& rayDirection, const glm::vec3& cameraPosition);

    float velocityValue=0;
    float finalVelocityValue=0;
    float velocityAngle; 
    float velocityAngleOffset;
    float vectorLength;
    float velocityOffset;
    std::vector<float> positionsSlidder;
    void updateVelocityLabel();
    void calculateVelocityVector();
    const double G = 6.67430e-11;
    void calculateOrbitalParameters(const CelestialObject& planet, const CelestialObject& sun, double& a, double& b);
    void computeEscapeVelocitySpeed();
    double escapeVelocity;
    double circularOrbitalVelocity;
    void setNewObjVelocity();
    void forceClose();

};

#endif // VelocityCreator_H