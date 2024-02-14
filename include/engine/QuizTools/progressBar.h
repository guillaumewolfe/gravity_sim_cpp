// Quiz.h
#ifndef ProgressBarQuiz_H
#define ProgressBarQuiz_H

#include <vector>
#include <string>
#include <imgui.h>

class ProgressBarQuiz {
public:
    ProgressBarQuiz();
    ~ProgressBarQuiz();
    void Draw(ImDrawList* drawList,ImVec2 centerPos, ImVec2 size, int winWidth, int winHeight);
    void reset();
    void addAnswer(int answerQuality); // 0 = wrong, 1 = orange, 2 = yellow, 3 = green, 4 = blue
    //Vector to counter answer answerQuality
    std::vector<int> answerQualityVector;
    int totalQuestions = 10;

private:
    int indexQuestion = 0;
    std::vector<ImVec4> colors = {ImVec4(1,0,0,1), ImVec4(1,0.75,0,1),ImVec4(1,1,0,1), ImVec4(0,1,0,1)};



};

#endif // QUIZ_H
