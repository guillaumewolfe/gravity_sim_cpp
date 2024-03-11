
#ifndef SuccessTool_H
#define SuccessTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include "UI/Labbel.h"
#include "UI/Icon.h"
#include <functional>

#include <engine/RenderTools/notificationTool.h>   

class SaveTool;

class Success {
    public:
    Success(int id, RenderContext* m_renderContext,int totalSteps, std::string title, std::string description, std::function<bool()> func)
    :id(id), totalSteps(totalSteps),  title(title), description(description), checkSuccessFunction(func), m_renderContext(m_renderContext) {}

    int totalSteps;
    int id;
    int stepsDone = 0;
    bool isDone = false;
    GLuint badgeNOTCOMPLTETED = 0;
    GLuint badgeCOMPLETED = 0;
    std::string title;
    std::string description;
    std::function<bool()> checkSuccessFunction;
    RenderContext* m_renderContext;

    int getStepsDone() {return stepsDone;}
    int getTotalSteps() {return totalSteps;}
    bool isCompleted() {return isDone;}

    bool checkSuccess() {
        return checkSuccessFunction();
    }
};

class SuccessTool : public RenderComponent {
public:

    explicit SuccessTool(RenderContext* renderContext);
    ~SuccessTool() override;
    void Draw() override;
    void Open();
    void Close();
    void setConfirmBoxFunction(const std::function<void(std::function<void()> func, const std::string& message)>& confirmBoxFunc);

    std::vector<Success*> getSuccessList();
    int getSuccessCount();
    void setNotificationTool(NotificationTool* notificationTool);
    void checkSuccess();

private:
    ImVec2 centerPos, topLeft, bottomRight;
    int winWidth, winHeight;
    int longueur,hauteur;

    float longueurRects;
    float hauteurRectBadge;
    float hauteurRectQuest;
    float centerYBagde;
    float centerYQuest;
    ImVec2 topLeftBadge, bottomRightBadge, topLeftQuest, bottomRightQuest;
    SaveTool* saveTool = nullptr;
    int questOffset = 0;
    int badgeOffset = 0;
    int maxQuestShown = 4;
    int maxBadgeShown = 5;


    std::vector<Success*> successList;
    NotificationTool* notificationTool;
    void initSuccess();

    //Drawing
    bool isTransiting;
    float transitionStep;
    float transitionSpeedBackground = 0.05;
    void initUI();
    void drawUI();

    Labbel* title = nullptr;
    Labbel* badgeTitle = nullptr;
    Labbel* questTitle = nullptr;

    Button* viewAllButton = nullptr;
    Button* viewCompletedButton = nullptr;
    Button* viewInProgressButton = nullptr;
    ImageButton* closeBtn = nullptr;
    ImageButton* nextBadge = nullptr;
    ImageButton* prevBadge = nullptr;

    Icon* mainIcon = nullptr;

    ImFont* questTitleFont = nullptr;
    ImFont* questExplanationFont = nullptr;
    ImFont* questProgressFont = nullptr;
    ImFont* badgeTitleFont = nullptr;

    void drawScrollBarQuest();
    void drawScrollBarBadge();
    void changeScrollOffsetQuest(int direction);
    void changeScrollOffsetBadge(int direction);
    void drawQuests();
    void drawBadgesForQuest(ImVec2 pos, float radius, Success* success);
    void drawBadges();
    Success* selectedSuccess = nullptr;

    //Images
    GLuint loadTexture(const char* relativeFilename, float alpha);
    GLuint loadTextureCOMPLETED(const char* relativeFilename, float alpha);
    GLuint checkMark = 0;


public:
    //For success:
    std::vector<std::string> elementsToVerify;

    //states showALL, showCompleted, showInProgress
    enum class ShowMode {
        showAll,
        showCompleted,
        showInProgress
    };
    ShowMode showMode = ShowMode::showAll;
    void getSuccessListToShow();
    std::vector<Success*> successToShow;
    void setShowMode(int mode);



    void checkCollision(CelestialObject* obj1, CelestialObject* obj2);
    void QUEST_DESTROY_EARTH();
    void QUEST_QUIZ_DONE(int difficulty, float score);
    void QUEST_SAVE_LOAD_SIMULATION(bool saved);
    void QUEST_MINIMAP_TRAVEL();
    bool earthDestroyed = false;
    bool twoStarsCollided = false;
    bool oneCollision = false;
    bool HungryForMore = false;
    bool DavidVsGoliath = false;
    bool quizDone = false;
    bool quiz1Done = false;
    bool quiz2Done = false;
    bool quiz3Done = false;
    bool simulationSaved = false;
    bool simulationSavedMultiple = false;
    bool simulationLoaded = false;
    bool simulationLoadedMultiple = false;
    bool minimapTravel = false;
};

#endif // SuccessTool_H