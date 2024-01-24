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
    std::vector<Labbel*> labbels = std::vector<Labbel*>();
    std::vector<Button*> buttons= std::vector<Button*>();
    std::vector<Slider*> sliders = std::vector<Slider*>();
    std::vector<ImageButton*> imageButtons = std::vector<ImageButton*>();
    Icon* icon = nullptr;
    float hauteur, longueur;
    ImFont* nameFont = nullptr;
    ImFont* nameFontBig = nullptr;

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
    void calculateVelocityAngle(CelestialObject* planet, CelestialObject* sun);

    void drawUI();
    void drawPlanets();
    void drawAngleSelector(CelestialObject* planet, CelestialObject* sun, CelestialObject* selectedObject);
    void drawSunEffect(ImVec2 planetPos, float radius);
    void drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius);
    void drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, CelestialObject* obj);
    void draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments);
    
    void generate_colors();
    std::map<std::string, ImVec4> typeDictColor;
    ImDrawList* drawList = nullptr;


    std::vector<float> valuesAngle;
    float normalizeAngle(float angle);

    //Selection
    CelestialObject* selectedObject = nullptr;
    bool checkSelection(CelestialObject* m_object, CelestialObject* sun, CelestialObject* otherObject, ImVec2 position, float radius);
    bool isExited;
};


#endif // VelocityCreator_H