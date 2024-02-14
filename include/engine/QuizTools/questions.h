// Question.h
#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

class Question {
public:
    std::string texte;
    std::vector<std::string> choix;
    int bonneReponse;
    std::string cheminImage; // Optionnel
    std::string explication; // Optionnel
    std::string hint; // Optionnel

    Question(std::string txt, std::vector<std::string> chx, int bonneRep,std::string explication = "", std::string cheminImg = "");
    void shuffle();
};

#endif // QUESTION_H