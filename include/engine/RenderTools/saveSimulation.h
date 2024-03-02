
#ifndef SaveSimulationTool_H
#define SaveSimulationTool_H

#include "RenderComponent.h"
#include <chrono>
#include <random>
#include "UI/Icon.h"



class SaveTool;
class GameState;
class CelestialObjectData;

class SaveSimulationTool : public RenderComponent {
public:

    explicit SaveSimulationTool(RenderContext* renderContext);
    ~SaveSimulationTool() override;
    void Draw() override;
    void Open();
    void Close();
    std::function<void()> updateNamesFunction;

enum class State {
    Intro,
    SaveSimulation,
    LoadSimulation,
    SaveConfirmed
};

private:

    SaveTool* saveTool = nullptr;
    std::vector<Labbel*> labbels;
    ImFont* titleFont = nullptr;
    ImFont* planetNameInSolarSystemFont = nullptr;
    ImFont* SelectedplanetNameInSolarSystemFont = nullptr;
    ImFont* wrongInputNameFont = nullptr;
    ImFont* saveTableFont = nullptr;
    int winHeight, winWidth;
    int longueur, hauteur;
    ImVec2 topLeft, center;
    ImVec2 closeBtn_PositionInitiale, nextBtn_PositionInitiale;
    ImVec2 selectedObjectPosition;
    ImVec4 selectedObjectColor;
    Button* loadIntroBtn = nullptr;
    Button *saveIntroBtn = nullptr;
    Button* saveSimulationBtn = nullptr;
    Button* ExitSaveSimulationBtn = nullptr;
    Button* switchStateBtn = nullptr;
    Button* loadBtn = nullptr;
    Labbel*title = nullptr;
    Labbel* saveTitleLabbel = nullptr;
    Labbel* LoadTitleLabbel = nullptr;
    Labbel* loadTitleLabbel = nullptr;
    Labbel* simulationTimeLabel = nullptr;
    Labbel* saveHistorySave = nullptr;
    Icon* iconIntro = nullptr;
    Icon* verticalArrow = nullptr;
    Icon* weightIcon = nullptr;
    Icon* diameterIcon = nullptr;
    Icon* saveIcon = nullptr;
    Icon* notificationIcon = nullptr;

    ImageButton* arrowUp = nullptr;
    ImageButton* arrowDown = nullptr;
    ImageButton* removeStateButton = nullptr;

    void removeState();
    void confirmRemove();
    void cancelRemove();
    void drawConfirmRemove();
    bool confirmRemoveState = false;
    Button* confirmRemoveButton = nullptr;
    Button* cancelRemoveButton = nullptr;
    Labbel* removeLabbel = nullptr;


    std::vector<GameState*> gameStates;
    GameState* currentGameStateSave;


    ImageButton* quitBtn = nullptr;
    ImageButton* quitSaveBtn = nullptr;




    //states
    State currentState = State::Intro;
    void enterSaveSimulationState();
    void enterLoadSimulationState();
    void drawGameStatesBoxForSave();
    void drawGameStatesBoxForLoad();
    
    //Load
    void drawPlanetSystemLoad();
    GameState* currentLoadState = nullptr;
    int maxGameStateShown = 5;
    CelestialObjectData* selectedObjectData = nullptr;
    CelestialObjectData* hoverObjectData = nullptr;
    void drawInfosLoad(CelestialObjectData* planete,CelestialObjectData* sun, ImVec2 planetPos);
    void drawDistanceFromSunLogoLoad(CelestialObjectData* planete, CelestialObjectData* sun,ImVec2 centerPos,float longueurRec,float hauteurRec,ImVec4 color);
    void drawWeightLogoLoad(CelestialObjectData* planete, ImVec2 centerPos,float longueurRec,float hauteurRec);
    void drawDiameterLodoLoad(CelestialObjectData* planete, ImVec2 centerPos,float longueurRec,float hauteurRec);
    void nextSelectedState(int direction);
    void loadingIndicator();
    Labbel* loadingLabbel = nullptr;
    bool isLoading = false;
    void load();
    bool shouldLoad = false;

    //Chrono time 
    std::chrono::time_point<std::chrono::system_clock> timeSaveConfirmed;
    
    void saveCurrentState();
    void saveSimulationButton();
    void LoadSimulationsButton();
    void drawUI();
    void drawModes();
    void drawSaveSimulation();
    void drawLoadSimulation();
    void drawSaveConfirmed();
    void drawIntro();
    void SwitchState();
    void changeState(int newState);
    void drawDistanceFromSunLogo(ImVec2 centerPos,float longueur,float hauteur,ImVec4 color);
    void drawWeightLogo(ImVec2 centerPos,float longueurRec,float hauteurRec);
    void drawDiameterLodo(ImVec2 centerPos,float longueurRec,float hauteurRec);

    void initUI();

    //Drawing
    void drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, ImVec4 color);
    void drawSunEffect(ImVec2 planetPos, float radius);
    void drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius);
    void draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments);
    ImFont* planetNamesFont = nullptr;
    void checkInputs();
    void drawSpeedArrow(CelestialObjectData planete, ImVec2 planetPos, CelestialObjectData sun);

    void drawSolarSystem();
    void drawNameInputBox();
    void drawInfos(CelestialObject* planete, ImVec2 planetPos, ImVec4 color);
    void drawUISaveSimulation();
    void setupDimension();

    void drawUILoadSimulation();

    void changeScrollOffset(int direction);
    int idOffset=0;


    //SaveSimulation variables
    std::string stateIDName;
    std::vector<std::string> gameStatesIds;
    //Dimensions
    ImVec2 centerPosSolarSystem;
    float radiusSolarSystem;
    float minDistanceMouseDetection;
    float scaledDistance;
    float scaleAfterLog;
    float logBase;
    ImVec2 offsetSolarSystem;
    ImVec2 distanceConverterSimToLog(ImVec2 initialPosition);
    void adjustLogScale();
    void adjustLogScaleLOAD(std::vector<CelestialObjectData> objects);
    CelestialObject* selectedObject = nullptr;
    CelestialObject* hoverObject = nullptr;
    bool checkMouseSelection(ImVec2 planetPos, CelestialObject* planete,float radius,float &closestDistance);
    bool checkMouseSelectionLoad(ImVec2 planetPos, CelestialObjectData* planete, float radius, float &closestDistance);
    void drawSelectedState(GameState *gameState);
    ImVec2 distanceConverterSimToLogSmallSystem(ImVec2 positionInitiale, float solarsystemSize);

    //asteroidPositions
    std::vector<ImVec2> asteroid1Positions;
    std::vector<ImVec2> asteroid2Positions;
    void generate_asteroids();
    void generate_asteroid_belt(std::mt19937& gen, std::uniform_real_distribution<>& dis, float rayonInterieur, float rayonExterieur, int numAsteroides, std::vector<ImVec2>& asteroidPositions);
    void draw_asteroid_belt(const std::vector<ImVec2>& asteroidPositions, float tailleAsteroide, ImU32 color);

};

#endif // SaveSimulationTool_H