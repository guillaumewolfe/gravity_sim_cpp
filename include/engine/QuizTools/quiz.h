// Quiz.h
#ifndef QUIZ_H
#define QUIZ_H

#include "engine/QuizTools/questions.h" // Assurez-vous d'avoir une classe Question définie
#include <vector>
#include <string>

class Quiz {
public:
    Quiz();
    ~Quiz();
    void demarrer();
    void afficherResultat() const;
    void generateEasyQuiz();
    void generateMediumQuiz();
    void generateHardQuiz();
    std::string getQuestionText(int index) const;
    std::vector<std::string> getChoices(int index) const;
    int getCorrectAnswer(int index) const;
    std::string getExplanation(int index) const;
    std::string getHint(int index) const;
    float getScore() const;
    int getQuestionSize() const;
    void addScore(int questionTrials);
    int numberOfQuestionsPosed = 10;
private:
    std::vector<Question> questions;
    int score;
    
    void addQuestion(Question question);

};

#endif // QUIZ_H
