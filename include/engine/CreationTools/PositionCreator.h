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



private:
    int winWidth, winHeight;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
    ImVec2 topLeft;
    float hauteur, longueur;


};

#endif // PositionCreator_H