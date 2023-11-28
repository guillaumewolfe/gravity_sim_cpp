// UITool.h
#ifndef TextureCreator_H
#define TextureCreator_H

#include "engine/CreationTools/StateCreator.h"

struct CelestialObjectInfo {
    float posX;
    float posY;
    std::string name;
    GLuint texture;
    float hoverRadius;
};


struct drawSphereComp{
    float posX;
    float posY;
    ImVec4 color;
    float radius;
};
class TextureCreator : public StateCreator {
public:

    bool drawSphereHover = false;
    bool drawSphereClick = false;


    explicit TextureCreator(RenderContext* renderContext, CreatorManager* manager);
    void Draw() override;
    void Enter() override;
    void DrawOpenGL() override;
    void Exit() override;


    void drawBackground();


    void generate_buttons();
    void generate_labels();
    void draw_buttons();
    void draw_labels();

    void CloseButtonPressed();

    void drawTextures();
    void generatePlanetLabels();
    void drawCelestialObjects();
    void draw2DSphereHover();
    void draw2DSphereClicked();
    void updateClickStatus();
    void drawSelectionSphere();
    void reset();
    void NextButtonPressed();
    void resetLabelColor();


private:
    drawSphereComp selectionSphereHover;
    drawSphereComp selectionSphereClick;
    int winWidth, winHeight;
    std::vector<Labbel*> labbels;
    std::vector<Labbel*> planeteNames;
    std::vector<Button*> buttons;
    std::vector<CelestialObject*> objects;
    std::vector<CelestialObject*> initSystem();

    void drawTexturedSphere(float radius, int numSegments, int numSlices, float offsetX, float offsetY,GLuint texture);
    double Rotation=0;

    ImVec2 topLeft;
    float hauteur, longueur;
    std::vector<CelestialObjectInfo> objectPositions;
    void calculateObjectPositions();
    void updateHoverStatus();
    float radius;
    size_t selectedIndex;

};

#endif // TextureCreator_H