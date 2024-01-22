// UITool.h
#ifndef ParametersCreator_H
#define ParametersCreator_H

#include "engine/CreationTools/StateCreator.h"
#include <unordered_map>
#include "UI/ImageButton.h"
#include "UI/Icon.h"

class ParametersCreator : public StateCreator {
public:


    explicit ParametersCreator(RenderContext* renderContext, CreatorManager* manager);

    void Draw() override;
    void Enter() override;
    void Exit() override;
    void DrawOpenGL() override;



    void drawBackground();
    void generate_buttons();
    void generate_labels();
    void initDictionnaries();
    void draw_sliders();
    void draw_buttons();
    void draw_labels();
    void next_state();
    void previous_state();
    void setParameters();
    void updateEffects();

    void draw_input_name();
    void draw_input_weight();
    void draw_input_radius();
    void draw_input_sideral();
    void getItemInitialParameters();
    void forceClose();

    std::function<void()> updateNamesFunction;

 



private:
    int winWidth, winHeight;
    int mode; // 0 = inactive, 1 = change weight, 2 = change radius, 3- Change Name
    void nextMode();
    void previousMode();

    void initMode1();
    void initMode2();
    void initMode3();

    void drawMode1();
    void drawMode2();
    void drawMode3();
    void drawMode();
    void drawUI();
    void changeMode();
    void resetName();

    std::vector<Labbel*> labbels,labbelsMode1,labbelsMode2,labbelsMode3,labbelsMode4,labbelsMode5;
    std::vector<Icon*> icons,iconsMode1,iconsMode2,iconsMode3,iconsMode4,iconsMode5;    
    std::vector<ImageButton*> imageButtons,imageButtonsMode1,imageButtonsMode2,imageButtonsMode3,imageButtonsMode4,imageButtonsMode5;
    std::vector<Button*> buttons,buttonsMode1,buttonsMode2,buttonsMode3,buttonsMode4,buttonsMode5;

    void drawPlanets();
    std::map<std::string, ImVec4> typeDictColor;
    void generate_colors();
    void drawSunEffect(ImVec2 planetPos, float radius);
    void drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius);
    void draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments);


    void initGeneralUI();


    Icon* icon;
    float hauteur, longueur;
    ImFont* font;
    ImFont* planeteNameFont;
    ImFont* planateSelectedFont;
    ImVec2 bottomRight;
    ImVec2 topLeft;
    ImVec2 centerPos;

    //Weight
    CelestialObject* m_object;
    CelestialObject* closestWeightObject = nullptr;
    float massRatio;
    double mass;
    double lastMass;
    double lastMassRatio;
    bool nameValid;
    CelestialObject* lastClosestWeightObject = nullptr;
    void resetWeight();
    float initialMassRatio;
    double initialMass;
    CelestialObject* initialClosestWeightObject = nullptr;
    std::string currentItemName;
    ImVec2 sunPos;


    //Radius
    CelestialObject* closestRadiusObject = nullptr;
    float radiusRatio;
    double radi;
    void drawPlanetsMode2();
    std::string planeteName;
    double lastRadi;
    double lastRadiusRatio;
    CelestialObject* lastClosestRadiusObject = nullptr;
    CelestialObject* initialClosestRadiusObject = nullptr;
    float initialRadiusRatio;
    double initialRadi;
    bool miniMapWasShown = false;
    void resetRadius();


};

#endif // ParametersCreator_H