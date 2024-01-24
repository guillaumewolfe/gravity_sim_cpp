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
    void Open();
    void draw_UI();
    void draw_rect();
    void draw_Scene();
    void draw_planets();
    void draw_camera();
    void draw_text();
    void drawSunEffect(ImVec2 planetPos);
    void drawPlanetLight(ImVec2 planetPos);
    void checkMouseSelection(ImVec2 planetPos, CelestialObject* planete);
    void moveMap(bool isX, bool isY, float value);
    void Zoom(bool in);
    void generate_asteroids();
    void updateAsteroidScale(float scale);
    void generate_asteroid_belt(std::mt19937& gen, std::uniform_real_distribution<>& dis, float rayonInterieur, float rayonExterieur, int numAsteroides, std::vector<ImVec2>& asteroidPositions);
    void draw_asteroid_belt(const std::vector<ImVec2>& asteroidPositions, float tailleAsteroide, ImU32 color, float rotation_par_seconde);
    void generate_colors();
    void setCloseButtonFunction(const std::function<void()>& func);
    bool showGravityField = true;
    void drawGravityField(CelestialObject* object, ImVec2 position, float radius, float scale);
    void updateButtonState();
    void resetCamera();

private:
    int winWidth, winHeight;
    float initialCenterX, initialCenterY;
    float scale;
    ImVec2 topLeft;
    ImVec2 bottomRight ;
    ImFont* nameFont = nullptr;
    float radius;
    std::string closestPlanetName;
    std::map<int, std::pair<std::string, ImVec4>> typeDict;
    std::map<std::string, ImVec4> typeDictColor;
    std::vector<ImageButton*> imageButtons;
    float longueur, hauteur;
    float longueurScene, hauteurScene;
    ImVec4 getTypeColor(int type);
    std::vector<ImVec2> asteroid1Positions;
    std::vector<ImVec2> asteroid2Positions;
    std::vector<ImVec2> originalAsteroid1Positions; // Déclarer un vecteur pour stocker les positions originales
    std::vector<ImVec2> originalAsteroid2Positions; // Déclarer un vecteur pour stocker les positions originales
    void draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments = 20);
    CelestialObject* selectedObject = nullptr;
    std::function<void()> closeFunction;
    void generate_buttons();
    void CloseButton();
    bool selected = false;
    void SelectObject(CelestialObject* object);
    Icon* iconCamera;
    void applyZoomToAsteroids();

    //Setup dimension
    void setup_dimension();
    ImVec2 centerPos;
    ImVec2 offset;
    double scaledDistance;
    double scaleAfterLog;
    double logBase;
    float minDistance;
    ImVec2 distanceConverterSimToLog(ImVec2 initialPosition);
    bool isWithinBounds(ImVec2 position, ImVec2 size);

    //Inputs
    void check_inputs();
    ImVec2 mousePos;
    bool isMouseClicked; // Vérifie si le bouton gauche de la souris a été cliqué
    bool isMouseDoubleClicked; 

    //Controls
    float deplacement_X, deplacement_Y;
    bool cameraTransition = false;
    float transitionSpeed = 0.001;
    void transitionCamera();
    float positionCible;
    float zoom=1;


};  

#endif // MinimapTool_H