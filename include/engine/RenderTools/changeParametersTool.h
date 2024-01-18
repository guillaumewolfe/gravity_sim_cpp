// UITool.h
#ifndef changeParametersTool_H
#define changeParametersTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include "UI/Icon.h"


class ChangeParametersTool : public RenderComponent {
public:

    explicit ChangeParametersTool(RenderContext* renderContext);
    ~ChangeParametersTool() override;
    void Draw() override;
    void Update(CelestialObject* otherObj);
    void setMode(int mode, CelestialObject* object);
    int getMode();
    void setupdateNamesFunction(const std::function<void()>& func);
    void setCloseMinimapFunction(const std::function<void()>& func);
    void setEditPositionFunction(const std::function<void(CelestialObject* object)>& func);


private:
    int mode; // 0 = inactive, 1 = change weight, 2 = change radius, 3 = change position, 4 = name, 5= speed
    int winWidth, winHeight;
    bool showConfirmChangeBox = false;
    bool changeConfirmed = false;
    ImFont* font;
    ImFont* planeteNameFont;
    ImFont* planateSelectedFont;

    CelestialObject* m_object;
    CelestialObject* closestWeightObject = nullptr;
    float massRatio;
    double mass;

    double lastMass;
    double lastMassRatio;
    CelestialObject* lastClosestWeightObject = nullptr;
    void resetWeight();

    float initialMassRatio;
    double initialMass;
    CelestialObject* initialClosestWeightObject = nullptr;
    std::string currentItemName;
    ImVec2 sunPos;



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

    void Open();
    void Close();
    void Save();

    void drawUI();
    void drawMode();
    void drawMode1();
    void drawMode2();
    void drawMode3();
    void drawMode4();
    void drawMode5();

    void initMode1();
    void initMode2();
    void initMode3();
    void initMode4();
    void initMode5();

    void changeMode();
    void initGeneralUI();

    void closeButton();
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


    std::function<void()> updateNamesFunction;
    std::function<void()> closeMinimapFunction;
    std::function<void(CelestialObject* object)> editPositionFunction;
    

};

#endif // changeParametersTool_H