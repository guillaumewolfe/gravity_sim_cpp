// UITool.h
#pragma once
#ifndef WELCOMETOOL_H
#define WELOMETOOL_H

#include "UI/Icon.h"
#include "UI/Labbel.h"
#include "UI/ImageButton.h"
#include "UI/Buttons.h"


class SoundTool;

class WelcomeTool {
public:
    float lineLength = 0;

    explicit WelcomeTool();
    void Draw();

    void Open();
    void Close();
    void setMode(int mode);
    void nextMode(bool next);
    bool* shouldDraw = nullptr;



private:
    int winWidth,winHeight;
    int mode = 1;
    int nbreModes = 3;
    ImVec2 centerPos, topLeft, bottomRight;
    float longueur, hauteur;

    void drawMode();

    void drawUI();
    void drawMode1();
    void drawMode2();
    void drawMode3();
    void initUI();
    void drawCircles();

    Labbel* title = nullptr;
    Labbel* subTitle = nullptr;

    ImageButton* nextBtn = nullptr;
    ImageButton* backBtn = nullptr;
    ImageButton* closeBtn = nullptr;

    Icon* SaveIcon = nullptr;
    Icon* resetCameraIcon = nullptr;
    Icon* resetModelIcon = nullptr;
    Icon* playIcon = nullptr;
    Icon* pauseIcon = nullptr;
    Icon* stepForwardIcon = nullptr;
    Icon* stepBackwardIcon = nullptr;
    Icon* miniMapIcon = nullptr;
    Icon* compareSizeIcon = nullptr;
    Icon* addPlanetIcon = nullptr;
    Icon* cameraIcon = nullptr;
    Icon* QuizIcon = nullptr;

    void initIconsMode1();
    ImFont* gameModeMainFont = nullptr;
    ImFont* gameModeSubFont = nullptr;


    SoundTool* soundTool = nullptr;
    Icon* keyboardIcon = nullptr;

    void initPresentation();
    std::string presentation;
    std::string presentation2;
    std::string presentation3;
    Labbel* WelcomeLabbel = nullptr;
    Labbel* presentatonText1 = nullptr;
    Labbel* presentatonText2 = nullptr;
    Labbel* presentatonText3 = nullptr;

    Labbel* presentatonText4 = nullptr;
    Labbel* presentatonText5 = nullptr;
    Labbel* presentatonText6 = nullptr;
    Labbel* presentatonText7 = nullptr;
    Labbel* presentatonText8 = nullptr;
    Labbel* presentatonText9 = nullptr;

    Icon* astronautIcon = nullptr;

};

#endif // WelcomeTool_H