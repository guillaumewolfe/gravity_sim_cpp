// Question.cpp
#include "engine/QuizTools/questions.h"
#include <algorithm> // Pour std::random_shuffle
#include <numeric>   // Pour std::iota

// Constructeur de la classe Question
Question::Question(std::string txt, std::vector<std::string> chx, int bonneRep,std::string explication, std::string cheminImg)
    : texte(txt), choix(chx), bonneReponse(bonneRep),explication(explication), cheminImage(cheminImg) {}

void Question::shuffle() {
    std::vector<int> indices(choix.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::random_shuffle(indices.begin(), indices.end());

    std::vector<std::string> choixMelanges;
    int nouvelleBonneReponse;
    for (int i = 0; i < indices.size(); ++i) {
        if (indices[i] == bonneReponse) {
            nouvelleBonneReponse = i;
        }
        choixMelanges.push_back(choix[indices[i]]);
    }

    choix = choixMelanges;
    bonneReponse = nouvelleBonneReponse;
}