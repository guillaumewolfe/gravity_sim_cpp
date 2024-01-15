// OptionsTool.h
#ifndef OpionsTool_H
#define OpionsTool_H

#include "RenderComponent.h"
#include "UI/ImageButton.h"
#include "UI/Labbel.h"
#include "UI/Buttons.h"
#include "UI/Icon.h"
#include <functional>

class OptionsTool : public RenderComponent {
public:
    explicit OptionsTool(RenderContext* renderContext);
    ~OptionsTool() override;
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void draw_labbels();
    void draw_buttons();
    void draw_rect();
    void generate_buttons();
    void generate_labels();

    void setMenuButtonFunction(const std::function<void()>& func);
    void setSettingsButtonFunction(const std::function<void()>& func);
    void setResumeButtonFunction(const std::function<void()>& func);
private:
    int winWidth, winHeight;
    std::vector<Labbel*> labbels;
    std::vector<Button*> buttons;
    ImageButton* imageButton;
    Icon* icon;


    void CloseButton();
    void MenuButton();
    void OptionsButton();
    std::function<void()> menuButtonFunction;
    std::function<void()> settingsButtonFunction;
    std::function<void()> resumeButtonFunction;

};

#endif // OptionsTool_H