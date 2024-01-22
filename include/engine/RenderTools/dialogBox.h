// UITool.h
#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include "RenderComponent.h"
#include "engine/Vec3.h"


class DialogBox : public RenderComponent {
public:

    explicit DialogBox(RenderContext* renderContext);
    void Draw() override;

    void Open(const std::string& title, const std::string& phrase, const std::string& button1Label = "Ok", const std::string& button2Label = "", std::function<void()> customDialogFunc = nullptr);
    void Close();


private:
    int winWidth, winHeight;
    float longueur, hauteur;
    ImVec2 centerPos;
    float transitionTime = 0.5f;
    float transitionTimer = 0.0f;
    bool isTransitioning = false;
    bool shouldClose = false;

    void draw_rect();
    void draw_UI();
    void transition();
    void generate_UI();
    void updateUI();
    void functionCalled();

    std::string phrase = "Hello, world!";
    std::string title = "Title";
    std::string button1String = "Button 1";
    std::string button2String = "Button 2";

    //Buttons
    ImVec2 posButtonCancelMiddle;
    ImVec2 posButtonOkRight;
    ImVec2 posButtonCancel;
    Labbel* PhraseText;
    Labbel* TitleText;
    ImageButton* closeButton;
    Button* OkButton;
    Button* CancelButton;

    std::function<void()> dialogFunc;

};

#endif // DialogBox_H