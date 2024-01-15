// MinimapTool.h
#ifndef MinimapTool_H
#define MinimapTool_H

#include "RenderComponent.h"
#include "UI/ImageButton.h"
#include <random> // Inclure pour la génération de nombres aléatoires
#include <functional>
#include "UI/Icon.h"


class MinimapTool : public RenderComponent {
public:
    float sliderVariable;
    explicit MinimapTool(RenderContext* renderContext);
    ~MinimapTool() override;
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void draw_UI();
    void draw_rect();
    void draw_Scene();
    void draw_planets();
    void generate_asteroids();
    void generate_asteroid_belt(std::mt19937& gen, std::uniform_real_distribution<>& dis, float rayonInterieur, float rayonExterieur, int numAsteroides, std::vector<ImVec2>& asteroidPositions);
    void draw_asteroid_belt(ImDrawList* drawList, const std::vector<ImVec2>& asteroidPositions, ImVec2 centerPos, float tailleAsteroide, ImU32 color, float rotation_par_seconde);
    void generate_colors();
    void setCloseButtonFunction(const std::function<void()>& func);
    bool showGravityField = true;
    void drawGravityField(CelestialObject* object, ImVec2 position, float radius, float scale);

private:
    int winWidth, winHeight;
    float scale;
    ImVec2 topLeft;
    ImVec2 bottomRight ;
    ImFont* nameFont;
    std::map<int, std::pair<std::string, ImVec4>> typeDict;
    std::map<std::string, ImVec4> typeDictColor;
    std::vector<ImageButton*> imageButtons;
    float longueur, hauteur;
    float longueurScene, hauteurScene;
    ImVec4 getTypeColor(int type);
    double logBase;
    std::vector<ImVec2> asteroid1Positions;
    std::vector<ImVec2> asteroid2Positions;
    void draw_half_circle_shadow(ImDrawList* drawList, ImVec2 center, float radius, ImU32 color, float angle, int num_segments = 20);
    CelestialObject* selectedObject = nullptr;
    std::function<void()> closeFunction;
    void generate_buttons();
    void CloseButton();
    bool selected = false;
    void SelectObject(CelestialObject* object);
    Icon* iconCamera;
};

#endif // MinimapTool_H