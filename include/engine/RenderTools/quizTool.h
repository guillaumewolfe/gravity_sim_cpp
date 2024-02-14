
#ifndef QuizTool_H
#define QuizTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"
#include "UI/Labbel.h"
#include <chrono>
#include <random>
#include "engine/QuizTools/quiz.h"
#include "engine/QuizTools/progressBar.h"

enum class State {
    Intro,
    Question,
    Results
};

class QuizTool : public RenderComponent {
public:

    explicit QuizTool(RenderContext* renderContext);
    ~QuizTool() override;
    void Draw() override;
    void Open();
    void Close();
    void setConfirmBoxFunction(const std::function<void(std::function<void()> func, const std::string& message)>& confirmBoxFunc);



private:

State currentState;
std::vector<Labbel*> labbels;
ImFont* titleFont = nullptr;
ImFont* planetNameInSolarSystemFont = nullptr;
ImFont* questionFont = nullptr;
int winHeight, winWidth;
int longueur, hauteur;
int difficultyLevel = 0; //0 null, 1 easy, 2 medium, 3 hard
ImVec2 topLeft = ImVec2(0, 0);
ImVec2 center = ImVec2(0, 0);
Labbel* title = nullptr;
ImVec2 bottomRight = ImVec2(0, 0);
Labbel* introText = nullptr;
Labbel* questionLabel = nullptr;
Labbel* questionExplanationLabel = nullptr;
Labbel* resultLabel = nullptr;
Labbel* resultTitleLabel = nullptr; 
Labbel* progressTitleLabel = nullptr;
Labbel* progressNumberLabel = nullptr;
Button* easyMode = nullptr;
Button* mediumMode = nullptr;
Button* hardMode = nullptr; 
Button* closeBtn = nullptr;
Button *nextBtn = nullptr;
Button *buttonNextQuestion = nullptr;
Button* bouttonQuestion1 = nullptr;
Button* bouttonQuestion2 = nullptr;
Button* bouttonQuestion3 = nullptr;
Button* bouttonQuestion4 = nullptr;
ImageButton* quitBtn = nullptr;
void resetButtons();
bool quizStarted = false;
bool forceAnswer = false;
bool isTransiting = false;
float transitionSpeedBackground = 0.04;
float transitionStep = 0;
Quiz* easyQuiz = nullptr;
Quiz* mediumQuiz = nullptr;
Quiz* hardQuiz = nullptr;
Quiz* currentQuiz = nullptr;
ProgressBarQuiz* progressBar = nullptr;
int currentQuestionIndex = 0;
int questionTrials = 0;


std::map<std::string, ImVec4> typeDictColor;
std::vector<float> initialAngles;
std::vector<std::string> systemSolaire = {"Sun","Mercury","Venus","Earth","Mars","Jupiter","Saturn","Uranus","Neptune"};
std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();


void initUI();
void initQuiz();
void drawUI();
void buttonCloseOnly();
void buttonNextAndClose();
void generate_colors();
ImVec2 closeBtn_PositionInitiale = ImVec2(0.5, 0.5);
ImVec2 nextBtn_PositionInitiale = ImVec2(0.5, 0.5);

void nextQuestion();
void previousQuestion();
void drawState();
void drawIntro();
void drawQuestion();
void CheckAnswer(int answer);
void goodAnswer(int answer);
void badAnswer(int answer);
void drawResults();
void newResult();
bool isTransitingResult = false;
float transitionSpeed = 0.1;
float transitionScore = 0;
ImVec4 lerpColor(const ImVec4& start, const ImVec4& end, float t);
void drawTransitingResult();
void drawSolarSystem(float pourcentageX = 0.5, float pourcentageY = 0.5, float pourcentageSize = 0.5);
void drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, ImVec4 color);
void drawSunEffect(ImVec2 planetPos, float radius);
void drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius);
void draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments);
void initializePlanetAngles();
void selectLevel(int level);//0 null, 1 easy, 2 medium, 3 hard
void startQuiz();
void askToClose();
std::function<void(std::function<void()> func, const std::string& message) > confirmBoxFunction;


};

#endif // QuizTool_H