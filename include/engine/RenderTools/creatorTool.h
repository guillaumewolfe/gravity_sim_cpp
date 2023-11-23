// UITool.h
#ifndef CreatorTOOL_H
#define CreatorTOOL_H

#include "RenderComponent.h"


class CreatorTool : public RenderComponent {
public:

    explicit CreatorTool(RenderContext* renderContext);
    void Draw() override;
    void drawBackground();
    void End();


    void generate_buttons();
    void generate_labels();
    void draw_buttons();
    void draw_labels();

    void CloseButtonPressed();


private:
    int winWidth, winHeight;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;

};

#endif // CreatorTool_H