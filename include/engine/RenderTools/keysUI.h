// KeysUI.h
#ifndef KeysUI_H
#define KeysUI_H

#include "RenderComponent.h"
#include <chrono>
#include "UI/ImageButton.h"
#include "UI/Icon.h"

struct Keys{
    std::string name;
    std::string controlPhrase;
    Icon* icon;
};

class KeysUI : public RenderComponent {
public:
    explicit KeysUI(RenderContext* renderContext);
    ~KeysUI() override;
    
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void draw_rect();


private:
    int winWidth, winHeight;
    ImVec2 center, topLeft;
    float longueur, hauteur;
    void initPosition();
    void initUI();
    void hideButton();
    std::vector<Labbel*> labbels;
    std::vector<Labbel*> keyLabbels;
    std::vector<Button*> buttons;
    std::vector<ImageButton*> imageButtons;

    void drawUI();



    //Keys:
    std::vector<Keys> keys;
    std::vector<Keys> keysHighlight;
    void updateAlpha();

    ImGuiKey getKeyFromName(const std::string& keyName);
    void init_key_icons();
    void draw_key_icons();
    void deactivateKeys();
    void activateKey(const std::string& keyName);
    void updateKeyPhrase(const std::string& keyName, const std::string& newControlPhrase);
    void drawRectForPressedKey(Icon* icon, bool isKeyDown);
};

#endif // KeysUI_H