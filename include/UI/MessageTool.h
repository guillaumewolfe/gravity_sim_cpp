// MessageTool.h
#ifndef MESSAGETOOL_H
#define MESSAGETOOL_H

#include "UI/Buttons.h"
#include "UI/Labbel.h"
#include "UI/Icon.h"
#include "UI/ImageButton.h"

class MessageTool{
public:
    std::function<void()> onConfirm;
    explicit MessageTool(std::string phr, std::function<void()> onConfirmFunc);
    bool shouldClose = false;
    std::string phrase;
    void Draw();
    void draw_rectangles();
    void generate_labels();
    void draw_labels();
    void generate_buttons();
    void draw_buttons();

    void OkButtonPressed();
    void CancelButtonPressed();

private:
    int winWidth, winHeight;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
    float longueur, hauteur;
    Icon *icon;
    ImageButton *imageButton;

};

#endif // MESSAGETOOL_H