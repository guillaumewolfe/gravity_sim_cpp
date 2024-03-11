#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include "engine/RenderTools/quizTool.h"
#include "path_util.h"
#include "engine/RenderTools/soundTool.h"
#include "engine/RenderTools/SuccessTool.h"

QuizTool::QuizTool(RenderContext* renderContext) : RenderComponent(renderContext){
    float fontSize = 16.0f; // Taille de police
    float questionFontSize = 22.0f; // Taille de police pour les questions
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
    float fontSizeScaled = fontSize * winHeight / 1920;
    float questionFontSizeScaled = questionFontSize * winHeight / 1920;
    titleFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaled);
    questionFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), questionFontSizeScaled);
    planetNameInSolarSystemFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(getFullPath("/assets/fonts/Roboto.ttf").c_str(), fontSizeScaled);
    longueur = winWidth * 0.5;
    hauteur = winHeight * 0.6;
    topLeft = ImVec2(winWidth * 0.5 - longueur * 0.5, winHeight * 0.5 - hauteur * 0.5);
    center = ImVec2(winWidth * 0.5, winHeight * 0.5);
    currentState = State::Intro;
    initQuiz();
    initUI();
    generate_colors();
    buttonCloseOnly();

}

QuizTool::~QuizTool() {
//Remove memory used
    delete title;
    delete introText;
    delete questionLabel;
    delete resultLabel;
    delete resultTitleLabel;
    delete easyMode;
    delete mediumMode;
    delete hardMode;
    delete closeBtn;
    delete nextBtn;
    delete buttonNextQuestion;
    delete bouttonQuestion1;
    delete bouttonQuestion2;
    delete bouttonQuestion3;
    delete bouttonQuestion4;
    delete easyQuiz;
    delete mediumQuiz;
    delete hardQuiz;
    delete currentQuiz;
    delete quitBtn;
    delete progressBar;
    delete questionExplanationLabel;
    delete progressTitleLabel;
    delete progressNumberLabel;
    delete playAgainButton;
    delete quitEndOfQuizButton;

}



void QuizTool::Draw() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    drawUI();
    if(!isTransiting){drawState();}
    //print mouse click % of planetScreenPos
    ImVec2 mousePos = ImGui::GetMousePos();
    //if mouseClicked, print position %

    ImGui::End(); 
}

void QuizTool::initUI() {
    //Labels
    ImVec4 button_color = ImVec4(0.17f, 0.27f, 0.17f, 1.0f);

    float taille_x = 0.35;
    float taille_y = 0.15;
    title = new Labbel(0.5f,0.25f,ImVec4(255,255,255,255),
                            "Quiz",28.0f,0.8f);
    introText = new Labbel(0.5f,0.55,ImVec4(255,255,255,255),
                            "Choose a difficulty",23.0f,0.6f);
    questionLabel = new Labbel(0.5f,0.325,ImVec4(255,255,255,255),
                            "Quel est le cheval blanc de napoléon?",25.0f,0.85f);
    questionExplanationLabel = new Labbel(0.5f,0.60,ImVec4(255,255,255,255),
                            "Le cheval de napoléon est blanc car c'est écrit dans la question, depuis 1959 le cheval blanc est blanc.",18.0f,0.85f);
    questionExplainationLabel2 = new Labbel(0.5f,0.625,ImVec4(255,255,255,255),
                            "Le cheval de napoléon est blanc car c'est écrit dans la question, depuis 1959 le cheval blanc est blanc.",18.0f,0.85f);
    resultLabel = new Labbel(0.5f,0.5f,ImVec4(255,255,255,255),
                            "2",70.0f,0.8f);
    resultTitleLabel = new Labbel(0.5f,0.4f,ImVec4(255,255,255,255),
                            "Quiz Results:",35.0f,0.6f);
    progressTitleLabel = new Labbel(0.5f,0.675f,ImVec4(255,255,255,255),
                            "Progress:",18.0f,0.5f);
    progressNumberLabel = new Labbel(0.5f,0.72f,ImVec4(255,255,255,255),
                            "1/10",16.0f,0.6f);

    questionExplainationLabel2->isHidden = true;
//Buttons
    std::function<void(std::string)> playSoundFunc = 
    std::bind(&SoundTool::playSound, m_renderContext->soundTool, std::placeholders::_1);

    closeBtn_PositionInitiale = ImVec2(0.5, 0.7);
    nextBtn_PositionInitiale = ImVec2(0.5f, 0.7);
    closeBtn = new Button(playSoundFunc,closeBtn_PositionInitiale.x, closeBtn_PositionInitiale.y, ImVec2(0.045, 0.035),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Close", 0.05f,19.0f,
                            std::bind(&QuizTool::askToClose, this),5); 

    nextBtn = new Button(playSoundFunc,nextBtn_PositionInitiale.x,nextBtn_PositionInitiale.y, ImVec2(0.05, 0.035),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Start", 0.45f,22.0f,
                            std::bind(&QuizTool::startQuiz, this),5); 
    buttonNextQuestion = new Button(playSoundFunc,0.5,0.7625, ImVec2(0.05, 0.035),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Next", 0.4f,20.0f,
                            std::bind(&QuizTool::nextQuestion, this),5); 
    quitBtn = new ImageButton(playSoundFunc,0.738542, 0.218147, ImVec2(taille_x*0.15, taille_y*0.15),0.60,
                        button_color,button_color,
                        "../assets/button/close.png", 0,
                            std::bind(&QuizTool::askToClose, this),3,false,ImVec4(0.17f, 0.27f, 0.17f, 1.0f),false);

    quitEndOfQuizButton = new Button(playSoundFunc,0.465, closeBtn_PositionInitiale.y, ImVec2(0.045, 0.035),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Close", 0.05f,19.0f,
                            std::bind(&QuizTool::Close, this),5); 

    playAgainButton  = new Button(playSoundFunc,0.535, closeBtn_PositionInitiale.y, ImVec2(0.045, 0.035),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(150.0/255.0, 250.0/255.0, 150.0/255.0, 1.0f),
                            "Play again", 0.2f,19.0f,
                            std::bind(&QuizTool::Open, this),5); 
    //Boutton pour les questions, centrer en x et espacés en y, 4 questions
    float beginY = 0.4;
    float spaceY = 0.05;
    float fontSize = 18.0f;
    bouttonQuestion1 = new Button(playSoundFunc,0.5f,beginY, ImVec2(0.25, 0.04),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.75f, 1.0f, 0.75f, 1.0f),
                            "Question 1", 0.2f,fontSize,
                            std::bind(&QuizTool::CheckAnswer, this,0),5);
    bouttonQuestion2 = new Button(playSoundFunc,0.5f,beginY+spaceY, ImVec2(0.25, 0.04),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.75f, 1.0f, 0.75f, 1.0f),
                            "Question 2", 0.2f,fontSize,
                            std::bind(&QuizTool::CheckAnswer, this,1),5);
    bouttonQuestion3 = new Button(playSoundFunc,0.5f,beginY+2*spaceY, ImVec2(0.25, 0.04),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.75f, 1.0f, 0.75f, 1.0f),
                            "Question 3", 0.2f,fontSize,
                            std::bind(&QuizTool::CheckAnswer, this,2),5);
    bouttonQuestion4 = new Button(playSoundFunc,0.5f,beginY+3*spaceY, ImVec2(0.25, 0.04),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.75f, 1.0f, 0.75f, 1.0f),
                            "Question 4", 0.2f,fontSize,
                            std::bind(&QuizTool::CheckAnswer, this,3),5);

    //Buttons for introText
    float hauteur = 0.6;
    easyMode = new Button(playSoundFunc,0.45f,hauteur, ImVec2(0.0475, 0.0325),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                            "Easy", 0.2f,20.0f,
                            std::bind(&QuizTool::selectLevel, this,1),5);
    mediumMode = new Button(playSoundFunc,0.5f,hauteur, ImVec2(0.0475, 0.0325),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(1.0f, 1.0f, 0.5f, 1.0f),
                            "Medium", 0.2f,20.0f,
                            std::bind(&QuizTool::selectLevel, this,2),5);
    hardMode = new Button(playSoundFunc,0.55f,hauteur, ImVec2(0.0475, 0.0325),
                            ImVec4(150.0/255.0, 150.0/255.0, 150.0/255.0, 1.0f),
                            ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                            "Hard", 0.2f,20.0f,
                            std::bind(&QuizTool::selectLevel, this,3),5);
    progressBar = new ProgressBarQuiz();
    progressBar->totalQuestions = easyQuiz->numberOfQuestionsPosed;
}

void QuizTool::drawUI() {
    longueur = currentState == State::Intro ? winWidth * 0.275 : winWidth * 0.5;
    hauteur = currentState == State::Intro ? winHeight * 0.5 : winHeight * 0.6;
    center = ImVec2(winWidth * 0.5, winHeight * 0.5);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float cornerRadius = currentState == State::Intro ? winWidth*0.015 : winWidth*0.02;
    topLeft = ImVec2(winWidth * 0.5 - longueur * 0.5, winHeight * 0.5 - hauteur * 0.5);
    bottomRight = ImVec2(winWidth * 0.5 + longueur * 0.5, winHeight * 0.5 + hauteur * 0.5);
    if(isTransiting){
        topLeft = ImVec2(winWidth * 0.5 - longueur * 0.5*transitionStep, winHeight * 0.5 - hauteur * 0.5*transitionStep);
        bottomRight = ImVec2(winWidth * 0.5 + longueur * 0.5*transitionStep, winHeight * 0.5 + hauteur * 0.5*transitionStep);
    }
    transitionStep += transitionSpeedBackground;
    if(transitionStep > 1){
        isTransiting = false;
        transitionStep = 0;
    }
    drawList->AddRectFilled(ImVec2(0,0), 
                            ImVec2(winWidth, winHeight), 
                            IM_COL32(0, 0, 0, 175), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(0, 0, 0, 255), // Color
                            cornerRadius);
    drawList->AddRectFilled(topLeft, 
                            bottomRight, 
                            IM_COL32(20, 25, 30, 150), // Color
                            cornerRadius);
    if(isTransiting){return;}
    ImVec2 titlePosition = currentState == State::Intro ?  ImVec2(0.5, 0.3) : ImVec2(0.5, 0.25f);
    title->UpdatePosition(titlePosition.x, titlePosition.y);
    title->Draw();
    ImVec2 quitBtnPosition = currentState == State::Intro ? ImVec2(0.627083, 0.270349) : ImVec2(0.735542, 0.22147);
    quitBtn->UpdatePosition(quitBtnPosition.x, quitBtnPosition.y);
    quitBtn->Draw();

    ImVec2 position = currentState == State::Intro ? ImVec2(0.5, 0.407) : ImVec2(0.280208, 0.255093);
    float size = currentState == State::Intro ? 0.075 : 0.05;
    drawSolarSystem(position.x,position.y,size);
}

void QuizTool::initQuiz(){
    easyQuiz = new Quiz();
    mediumQuiz = new Quiz();
    hardQuiz = new Quiz();
    easyQuiz->generateEasyQuiz();
    mediumQuiz->generateMediumQuiz();
    hardQuiz->generateHardQuiz();
}


void QuizTool::buttonCloseOnly(){
    closeBtn->hidden = false;
    nextBtn->hidden = true;
}
void QuizTool::buttonNextAndClose(){
    nextBtn->hidden = false;
    closeBtn->hidden = true;
}

void QuizTool::drawState() {
    switch (currentState) {
        case State::Intro:
            drawIntro();
            break;
        case State::Question:
            drawQuestion();
            break;
        case State::Results:
            drawResults();
            break;
    }
}

void QuizTool::Open() {
    *(m_renderContext->showInfo) = false;
    closeBtn->hidden = false;
    nextBtn->hidden = true;
    currentState = State::Intro;
    quizStarted = false;
    currentQuestionIndex = 0;
    selectLevel(0);
    isTransiting = true;
}

void QuizTool::askToClose() {
    if (quizStarted) {
        confirmBoxFunction(std::bind(&QuizTool::Close, this), "Are you sure you want to quit the quiz?");
    } else {
        Close();
    }
}
void QuizTool::setConfirmBoxFunction(const std::function<void(std::function<void()> func, const std::string& message)>& confirmBoxFunc) {
    confirmBoxFunction = confirmBoxFunc;
}
void QuizTool::Close() {
    selectLevel(0);
    quizStarted = false;
    m_renderContext->showQuiz = false;
}

void QuizTool::startQuiz() {
    if(difficultyLevel ==0){
        return;
    }
    currentState = State::Question;
    quizStarted = true;
    currentQuiz->demarrer();
    buttonCloseOnly();
    currentQuestionIndex = -1;
    nextQuestion();
    progressBar->reset();
}


void QuizTool::selectLevel(int level) {
    switch (level) {
        case 0:
            hardMode->isOn = false;
            easyMode->isOn = false;
            mediumMode->isOn = false;
            difficultyLevel = 0;
            buttonCloseOnly();
            currentQuiz = nullptr;
            break;
        case 1://Easy
            easyMode->isOn = true;
            mediumMode->isOn = false;
            hardMode->isOn = false;
            difficultyLevel = 1;
            currentQuiz = easyQuiz;
            buttonNextAndClose();
            break;
        case 2:
            easyMode->isOn = false;
            mediumMode->isOn = true;
            hardMode->isOn = false;
            difficultyLevel = 2;
            currentQuiz = mediumQuiz;
            buttonNextAndClose();
            break;
        case 3:
            easyMode->isOn = false;
            mediumMode->isOn = false;
            hardMode->isOn = true;
            difficultyLevel = 3;
            currentQuiz = hardQuiz;
            buttonNextAndClose();
            break;
    }
}


void QuizTool::drawQuestion(){
    questionLabel->Draw();
    bouttonQuestion1->Draw();
    bouttonQuestion2->Draw();
    bouttonQuestion3->Draw();
    bouttonQuestion4->Draw();
    buttonNextQuestion->Draw();
    progressNumberLabel->Draw();
    questionExplanationLabel->Draw();
    questionExplainationLabel2->Draw();
    progressBar->Draw(ImGui::GetWindowDrawList(), ImVec2(0.5, 0.7), ImVec2(0.3, 0.01), winWidth, winHeight);
    progressTitleLabel->Draw();
}

void QuizTool::nextQuestion() {
    questionTrials = 0;
    forceAnswer = false;
    questionExplanationLabel->isHidden = true;
    questionExplainationLabel2->isHidden = true;
    buttonNextQuestion->hidden = true;
    if (currentQuestionIndex < currentQuiz->numberOfQuestionsPosed - 1) {
        currentQuestionIndex++;
        resetButtons();
    } else {
        // change states
        currentState = State::Results;
        // display results
        newResult();
    }
}
void QuizTool::previousQuestion() {
    questionTrials = 0;
}
void QuizTool::CheckAnswer(int answer){
    questionTrials++;
    if(currentQuiz->getCorrectAnswer(currentQuestionIndex) == answer){
        currentQuiz->addScore(questionTrials);
        goodAnswer(answer);
    }else{
        badAnswer(answer);
    }
    if(questionTrials == 3 && currentQuiz->getCorrectAnswer(currentQuestionIndex) != answer){
        //Force end of round if only 1 left and bad answer
       CheckAnswer(currentQuiz->getCorrectAnswer(currentQuestionIndex));
       forceAnswer = true;
        }
}

void QuizTool::goodAnswer(int answer){
    buttonNextQuestion->hidden = false;
    if(answer == 0){
        bouttonQuestion1->isOn = true;
        bouttonQuestion1->enabled = false;
        bouttonQuestion2->enabled = false;
        bouttonQuestion3->enabled = false;
        bouttonQuestion4->enabled = false;
    }else if(answer == 1){
        bouttonQuestion2->isOn = true;
        bouttonQuestion1->enabled = false;
        bouttonQuestion2->enabled = false;
        bouttonQuestion3->enabled = false;
        bouttonQuestion4->enabled = false;
    }else if(answer == 2){
        bouttonQuestion3->isOn = true;
        bouttonQuestion1->enabled = false;
        bouttonQuestion2->enabled = false;
        bouttonQuestion3->enabled = false;
        bouttonQuestion4->enabled = false;
    }else if(answer == 3){
        bouttonQuestion4->isOn = true;
        bouttonQuestion1->enabled = false;
        bouttonQuestion2->enabled = false;
        bouttonQuestion3->enabled = false;
        bouttonQuestion4->enabled = false;
    }
    questionExplanationLabel->isHidden = false;
    if(explanationTooLong){
        questionExplainationLabel2->isHidden = false;
    }
    if(!forceAnswer){
    m_renderContext->soundTool->playSound("goodAnswer");}
    progressBar->addAnswer(4-questionTrials);
}

void QuizTool::badAnswer(int answer){
    //Desactivate the button
    if(answer == 0){
        bouttonQuestion1->isDisabled = true;
    }else if(answer == 1){
        bouttonQuestion2->isDisabled = true;
    }else if(answer == 2){
        bouttonQuestion3->isDisabled = true;
    }else if(answer == 3){
        bouttonQuestion4->isDisabled = true;
    }
    m_renderContext->soundTool->playSound("badAnswer");
}

void QuizTool::resetButtons(){
    questionLabel->UpdateText(currentQuiz->getQuestionText(currentQuestionIndex));
    bouttonQuestion1->UpdateText(currentQuiz->getChoices(currentQuestionIndex)[0]);
    bouttonQuestion2->UpdateText(currentQuiz->getChoices(currentQuestionIndex)[1]);
    bouttonQuestion3->UpdateText(currentQuiz->getChoices(currentQuestionIndex)[2]);
    bouttonQuestion4->UpdateText(currentQuiz->getChoices(currentQuestionIndex)[3]);

    //If explaination is too long, split it in 2 after the next space. 
    std::string explanation = currentQuiz->getExplanation(currentQuestionIndex);
    if(explanation.size() > 100){
        explanationTooLong = true;
        int spaceIndex = explanation.find(" ", 100); // Trouver le premier espace après le 100e caractère
        if(spaceIndex != std::string::npos){ // Vérifier si un espace a été trouvé
            questionExplanationLabel->UpdateText(explanation.substr(0, spaceIndex));
            // S'assurer de commencer le substring suivant après l'espace trouvé
            questionExplainationLabel2->UpdateText(explanation.substr(spaceIndex + 1));
        }else{
            // Si aucun espace n'est trouvé après le 100e caractère, afficher tout dans le premier label
            questionExplanationLabel->UpdateText(explanation);
            questionExplainationLabel2->UpdateText("");
        }
    }else{
        explanationTooLong = false;
        questionExplanationLabel->UpdateText(explanation);
        questionExplainationLabel2->UpdateText("");
    }
    progressNumberLabel->UpdateText(std::to_string(currentQuestionIndex+1) + "/" + std::to_string(currentQuiz->numberOfQuestionsPosed));
    bouttonQuestion1->isDisabled = false;
    bouttonQuestion2->isDisabled = false;
    bouttonQuestion3->isDisabled = false;
    bouttonQuestion4->isDisabled = false;
    bouttonQuestion1->isOn = false;
    bouttonQuestion2->isOn = false;
    bouttonQuestion3->isOn = false;
    bouttonQuestion4->isOn = false;
    bouttonQuestion1->enabled = true;
    bouttonQuestion2->enabled = true;
    bouttonQuestion3->enabled = true;
    bouttonQuestion4->enabled = true;
}






void QuizTool::newResult() {
    resultLabel->UpdateText("0 %");
    isTransitingResult = true;
    transitionScore = 0;
    int difficulty = difficultyLevel;
    m_renderContext->successTool->QUEST_QUIZ_DONE(difficulty, currentQuiz->getScore());
}

void QuizTool::drawTransitingResult(){
    //Get drawList
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    if(transitionScore < currentQuiz->getScore()){
        transitionScore += transitionSpeed;
        resultLabel->UpdateText(std::to_string((int)transitionScore) + " %");
        float scorePercentage = transitionScore / 100;
        ImVec4 color = lerpColor(ImVec4(255.0f, 150.0f, 150.0f, 1.0f), // Rouge
                                 ImVec4(150.0f, 255.0f, 150.0f, 1.0f), // Vert
                                 scorePercentage);
        resultLabel->UpdateColor(color);
    }else{
        isTransitingResult = false;
    }
}
void QuizTool::drawResults() {
    resultLabel->Draw();
    resultTitleLabel->Draw();
    if(isTransitingResult){drawTransitingResult();}
    if(!isTransitingResult){
        playAgainButton->Draw();
        quitEndOfQuizButton->Draw();
    }
}



ImVec4 QuizTool::lerpColor(const ImVec4& start, const ImVec4& end, float t) {
    return ImVec4(start.x + (end.x - start.x) * t, // R
                  start.y + (end.y - start.y) * t, // G
                  start.z + (end.z - start.z) * t, // B
                  start.w + (end.w - start.w) * t); // A
}




void QuizTool::drawIntro() {
    introText->Draw();
    easyMode->Draw();
    mediumMode->Draw();
    hardMode->Draw();
    closeBtn->Draw();
    nextBtn->Draw();
}

void QuizTool::initializePlanetAngles() {
    std::mt19937 gen(std::random_device{}()); // Générateur de nombres aléatoires
    std::uniform_real_distribution<float> dist(0.0f, 2.0f * M_PI); // Distribution de 0 à 2π

    initialAngles.clear(); // Assurez-vous qu'il est vide avant de commencer
    for(size_t i = 0; i < systemSolaire.size(); ++i) {
        initialAngles.push_back(dist(gen)); // Générer et stocker un angle aléatoire pour chaque planète
    }
}

void QuizTool::drawSolarSystem(float pourcentageX, float pourcentageY, float pourcentageSize){
    if(initialAngles.empty()) {
        initializePlanetAngles(); // Initialisez les angles si ce n'est pas déjà fait
    }
    ImGui::PushFont(planetNameInSolarSystemFont);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 positionCentre = ImVec2(winWidth * pourcentageX, winHeight * pourcentageY);
    float maxSize = winHeight * pourcentageSize;
    float normalRadius = maxSize/(8*4);
    float spacing = maxSize / 9;
    static float globalAngle = 0.0f;

    // Calculez le delta de temps depuis la dernière mise à jour
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - lastUpdateTime).count();
    lastUpdateTime = now;

    // Mettez à jour l'angle global basé sur le temps écoulé
    globalAngle += deltaTime * 0.2f;

    float indice = 0;

    for(std::string planet : systemSolaire){
        ImVec4 color = typeDictColor[planet];
        float distance = indice * spacing;// Assurez-vous que les planètes ne sont pas sur le Soleil

        // La vitesse orbitale diffère pour chaque planète
        float orbitSpeed = 0.2f + (10-indice) * 0.3f;

        float angle = -(globalAngle * orbitSpeed + initialAngles[indice]); // Utilisez l'angle initial aléatoire stocké

        // Calculez la position de la planète sur son orbite
        ImVec2 position = ImVec2(positionCentre.x + cos(angle) * distance, positionCentre.y + sin(angle) * distance);

        drawList->AddCircleFilled(position, normalRadius, IM_COL32(color.x, color.y, color.z, 255), 50);
        if(planet == "Sun"){
            drawSunEffect(position, normalRadius);
        }else{
        drawPlanetLightNOSHADOW(position, normalRadius, color);
        drawPlanetLight(position, positionCentre, normalRadius);
        }
        drawList->AddCircle(positionCentre, distance, IM_COL32(255, 255, 255, 20), 100, winWidth*0.0005); // Dessinez l'orbite
        ImVec2 textSizeObject = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, 0.0f, planet.c_str());
        if(currentState == State::Intro)drawList->AddText(ImVec2(position.x-textSizeObject.x/2, position.y - 5*normalRadius), IM_COL32(255, 255, 255, 100), planet.c_str());
        indice++;
    }
    ImGui::PopFont();
}

void QuizTool::generate_colors() {
    // Modifier le dictionnaire pour utiliser des chaînes de caractères comme clés
    typeDictColor["Black Hole"] = ImVec4(0, 0, 0, 1);
    typeDictColor["Sun"] = ImVec4(250, 250, 120, 1); // Jaune pour une étoile par exemple
    typeDictColor["Mercury"] = ImVec4(100, 100, 100, 1);
    typeDictColor["Venus"] = ImVec4(238, 147, 17, 1); // Orange pour Venus
    typeDictColor["Earth"] = ImVec4(50, 100, 255, 1); // Bleu pour la Terre
    typeDictColor["Mars"] = ImVec4(197, 84, 62, 1); // Rougeâtre pour Mars
    typeDictColor["Jupiter"] = ImVec4(234, 206, 180, 1);
    typeDictColor["Saturn"] = ImVec4(217, 186, 140, 1); // Jaune pour Saturne
    typeDictColor["Uranus"] = ImVec4(155, 221, 227, 1);
    typeDictColor["Neptune"] = ImVec4(39, 114, 210, 1); // Bleu pour Neptune
    typeDictColor["Ice"] = ImVec4(122, 137, 150, 1); // Bleu clair pour les objets glacés
    typeDictColor["Rock"] = ImVec4(137, 80, 77, 1); // Gris pour les objets rocheux
    typeDictColor["Volcanic"] = ImVec4(62, 95, 86, 1); // Gris foncé pour les objets volcaniques
    typeDictColor["Moon"] = ImVec4(200, 200, 200, 1); // Gris pour les lunes
}

void QuizTool::drawPlanetLightNOSHADOW(ImVec2 planetPos, float radius, ImVec4 colorCenterDot){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float numBlurCircles = 20;
    float blurIncrease = radius*0.04; // How much larger each successive blur circle is
    float initialAlpha = 40; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);}
}

void QuizTool::drawSunEffect(ImVec2 planetPos, float radius){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec4 colorCenterDot = ImVec4(255,215,80,255);
    float numBlurCircles = 40;
    float blurIncrease = radius*0.05; // How much larger each successive blur circle is
    float initialAlpha = 30; // Starting alpha value for the outermost blur circle
    float alphaDecrease = initialAlpha / numBlurCircles; // How much alpha decreases per circle
    for (int i = 0; i < numBlurCircles; ++i) {
        float blurRadius = radius + blurIncrease * (i + 1);
        float alpha = initialAlpha - alphaDecrease * i;
        drawList->AddCircleFilled(planetPos, blurRadius, IM_COL32(colorCenterDot.x,colorCenterDot.y,colorCenterDot.z, alpha), 100);}
}

void QuizTool::drawPlanetLight(ImVec2 planetPos, ImVec2 sunPos, float radius){
        ImVec2 directionToSun = ImVec2(planetPos.x - sunPos.x, planetPos.y - sunPos.y);
        float shadowAngle = atan2(directionToSun.y, directionToSun.x) + IM_PI; // Ajouter PI pour que l'ombre soit opposée au Soleil
        // Dessiner l'ombre comme un demi-cercle
        draw_half_circle_shadow(planetPos, radius, IM_COL32(0, 0, 0, 150), shadowAngle, 100);
}



void QuizTool::draw_half_circle_shadow(ImVec2 center, float radius, ImU32 color, float angle, int num_segments) {
    // Calcule les angles pour le demi-cercle d'ombre
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    radius*=1.2;
    float start_angle = angle - IM_PI / 2-M_PI; // Commence à 90 degrés par rapport à l'angle d'ombre
    float end_angle = angle + IM_PI / 2-M_PI;   // Termine à 270 degrés par rapport à l'angle d'ombre

    // Début du chemin par le centre
    drawList->PathLineTo(center);

    // Ajout de l'arc pour le demi-cercle
    for (int i = 0; i <= num_segments; ++i) {
        float segment_angle = start_angle + (float(i) / float(num_segments)) * (end_angle - start_angle);
        ImVec2 vertex = ImVec2(center.x + cos(segment_angle) * radius, center.y + sin(segment_angle) * radius);
        drawList->PathLineTo(vertex);
    }

    // Fermeture du chemin en revenant au centre
    drawList->PathLineTo(center);

    // Remplissage du demi-cercle
    drawList->PathFillConvex(color);
}