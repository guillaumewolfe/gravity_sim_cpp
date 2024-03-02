// UITool.h
#ifndef UITOOL_H
#define UITOOL_H

#include "RenderComponent.h"
#include <chrono>
#include "UI/ImageButton.h"
#include "UI/Icon.h"
#include "engine/RenderTools/MusicPlayerTool.h"

class UITool : public RenderComponent {
public:
    float sliderVariable;
    explicit UITool(RenderContext* renderContext);
    ~UITool() override;
    
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void draw_labbels();
    void draw_buttons();
    void draw_rect();
    GLuint backgroundTexture;
    std::chrono::system_clock::time_point start_time;
    void update_time();
    float lastScroll = 0.0f;

    GLuint textureID;

    ImageButton* pauseButton = nullptr;
    MusicPlayerTool* musicPlayerTool = nullptr;
    void drawPlanet();

    bool isSearching = false;
private:
    int winWidth, winHeight;
    ImFont* myFont = nullptr;
    ImFont* fontUI = nullptr;
    std::chrono::steady_clock::time_point lastFrameTime;
    int frameCount;
    float fps;
    void checkScroll();
    void drawLive(ImDrawList* drawList, ImVec2 centerPos);
    void drawConnexionFailed(ImDrawList* drawList, ImVec2 centerPos);
    

    //SearchTool
    void drawSearchTool();
    Icon* searchIcon = nullptr;
    Icon* selectionIcon = nullptr;
    Labbel* searchLabbel = nullptr;
    std::string searchInput = "";
    ImFont* searchFont = nullptr;
    ImFont* typeFont = nullptr;
    void searchItem();
    void DrawSearchBoxes();
    void planetSelectionSearch(CelestialObject* planet);
    bool isHoveringSearchBoxes = false;
    float Rotation = 0.0f;
    std::vector<CelestialObject*> foundObjects;

};

#endif // UITOOL_H