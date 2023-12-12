// UITool.h
#ifndef ParametersCreator_H
#define ParametersCreator_H

#include "engine/CreationTools/StateCreator.h"
#include <unordered_map>

class ParametersCreator : public StateCreator {
public:


    explicit ParametersCreator(RenderContext* renderContext, CreatorManager* manager);

    void Draw() override;
    void Enter() override;
    void DrawOpenGL() override;
    void Exit() override;

    void drawBackground();
    void generate_buttons();
    void generate_labels();
    void generate_sliders();
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
 



private:
    int winWidth, winHeight;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
    float hauteur, longueur;

    ImVec2 bottomRight;
    ImVec2 topLeft;
    ImVec2 centerPos;
    char planetName[256];

    int indexWeight;
    int indexRadius;
    std::unordered_map<std::string, double> itemsWeightDictionary;
    std::unordered_map<std::string, double> itemsRadiusDictionary;
    std::vector<std::string> itemNamesWeight;
    std::vector<std::string> itemNamesRadius;
    std::string currentItemWeight = "";
    std::string currentItemRadius = "";

    float massMultiplicator;
    float radiusMultiplicator;
    float sideralMultiplicator;


};

#endif // ParametersCreator_H