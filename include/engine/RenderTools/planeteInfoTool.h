
#ifndef PlaneteInfoTool_H
#define PlaneteInfoTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include "UI/Labbel.h"
#include "UI/Buttons.h"
#include "engine/CelestialObject.h"
#include <utility> 
#include <map>


class PlaneteInfoTool : public RenderComponent {
public:

    int winWidth, winHeight;
    explicit PlaneteInfoTool(RenderContext* renderContext);
    void Draw() override;
    void drawBackground();

    void initLabels();
    void draw_labels();
    void updateLabels();


    //Accessoire
    std::string getTypeName(int type);
    ImVec4 getTypeColor(int type);
    std::string formatScientific(double value);
    void drawTexturedSphere(float radius, int numSegments, int numSlices);



private:

    CelestialObject* m_object;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
    std::map<int, std::pair<std::string, ImVec4>> typeDict;
    double Rotation=0;


    
};

#endif // PlaneteInfoTool_H