#include "engine/QuizTools/progressBar.h"
#include <iostream>

ProgressBarQuiz::ProgressBarQuiz() {
}

ProgressBarQuiz::~ProgressBarQuiz() {
}

void ProgressBarQuiz::Draw(ImDrawList* drawList, ImVec2 centerPos, ImVec2 size, int winWidth, int winHeight) {
    // Draw the progress bar background
    ImVec2 topLeft = ImVec2(centerPos.x*winWidth - size.x*winWidth / 2, centerPos.y*winHeight - size.y*winHeight / 2);
    ImVec2 bottomRight = ImVec2(centerPos.x*winWidth + size.x*winWidth / 2, centerPos.y*winHeight + size.y*winHeight / 2);
    drawList->AddRectFilled(topLeft,bottomRight, IM_COL32(255,255,255, 10),winHeight*0.025f);
    // Calculate width of each answer segment
    float spacing = winWidth * 0.001f;
    float segmentWidth = ((size.x * winWidth) - (totalQuestions - 1) * spacing) / totalQuestions;
    
    for (int i = 0; i < answerQualityVector.size(); ++i) {
        // Determine color based on answer quality
        ImVec4 color = colors[answerQualityVector[i]]; // Assumes colors are correctly set up for each answer quality
        color.w = 0.1f; // Adjust alpha to be fully opaque

        // Calculate segment position, including spacing
        ImVec2 segTopLeft = ImVec2(topLeft.x + (segmentWidth + spacing) * i, topLeft.y);
        ImVec2 segBottomRight = ImVec2(segTopLeft.x + segmentWidth, bottomRight.y);

        // Draw segment
        drawList->AddRectFilled(segTopLeft, segBottomRight, IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255), winHeight * 0.025f);
        drawList->AddRect(segTopLeft, segBottomRight, IM_COL32(255,255,255,50),winHeight * 0.025f,0, winHeight * 0.0025f);
    }
}

void ProgressBarQuiz::reset() {
    indexQuestion = 0;
    answerQualityVector.clear();
}

void ProgressBarQuiz::addAnswer(int answerQuality) {
    answerQualityVector.push_back(answerQuality);
    indexQuestion++;
}