
#ifndef PlaneteInfoTool_H
#define PlaneteInfoTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include "UI/Labbel.h"
#include "UI/Buttons.h"
#include "UI/ImageButton.h"
#include "UI/Slider.h"
#include "UI/ToggleButton.h"
#include "engine/CelestialObject.h"
#include <utility> 
#include <map>
#include "engine/RenderTools/changeParametersTool.h"


class PlaneteInfoTool : public RenderComponent {
public:

    int winWidth, winHeight;
    explicit PlaneteInfoTool(RenderContext* renderContext);
    ~PlaneteInfoTool() override;
    void Draw() override;
    void drawBackground();

    void initLabels();
    void initButtons();
    void draw_labels();
    void updateLabels();
    void editButton();
    void moveToButton();
    void draw_buttons();
    void closeButton();
    void draw_story();
    void removePlanete();
    void changeMass();
    void changeRadius();
    void changePosition();
    void changeName();
    void orbitButton();


    //Accessoire
    std::string getTypeName(int type);
    ImVec4 getTypeColor(int type);
    std::string formatScientific(double value);
    void drawTexturedSphere();

    int mode = 1;
    void setMode(int mode);

    ImFont* storyFont;
    ChangeParametersTool* changeParametersTool;
    bool isHovering = false;
    void setConfirmBoxFunction(const std::function<void(std::function<void()> func, const std::string& message)>& confirmBoxFunc);


private:

    CelestialObject* m_object;
    CelestialObject* previousObject = nullptr;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
    std::vector<ImageButton*> imageButtons;
    std::map<int, std::pair<std::string, ImVec4>> typeDict;
    double Rotation=0;

    std::vector<Labbel*> labbelsMode3;
    std::vector<Button*> buttonsMode3;
    std::vector<ToggleButton*> togglebuttonsMode3;
    std::vector<ImageButton*> imageButtonsMode3;    
    void generate_mode3();
    void draw_mode3();
    void updateMode3();
    std::string phrase = "Test";

    std::function<void(std::function<void()> func, const std::string& message) > confirmBoxFunction;

    float radius = 0.025;
    int numSegments = 40;
    int numSlices = 40;
    float angleCercleOrbit = 0;
    float satelliteX;
    float satelliteY;
    
    bool drawOrbit;
    bool drawPath;
    bool drawName;
};

#endif // PlaneteInfoTool_H