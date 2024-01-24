// SettingsTool.h
#ifndef SettingsTool_H
#define SettingsTool_H

#include "RenderComponent.h"
#include "UI/ImageButton.h"
#include "UI/Labbel.h"
#include "UI/Buttons.h"
#include "UI/Icon.h"
#include <functional>
#include "UI/Slider.h"
#include "game/game.h"
#include "UI/ToggleButton.h"

class SettingsTool : public RenderComponent {
public:
    explicit SettingsTool(RenderContext* renderContext);
    ~SettingsTool() override;
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void init();
    void draw_labbels();
    void draw_buttons();
    void draw_rect();
    void generate_buttons();
    void generate_labels();
    void setMode(int newMode);
    void Open();
    void setGameSettings(GameSettings* settings);

    void setCloseButtonFunction(std::function<void()> function);
    void setSaveButtonFunction(std::function<void()> function);
    void CloseButton();
    void SaveButton();
private:
    ToggleButton* toggleButton = nullptr;
    bool testToggle = false;
    int winWidth, winHeight;
    float longueur, hauteur;
    std::vector<Labbel*> labbels = std::vector<Labbel*>();
    std::vector<Button*> buttons = std::vector<Button*>();
    std::vector<ImageButton*> imageButtons = std::vector<ImageButton*>();

    std::vector<Labbel*> labbelsMode1 = std::vector<Labbel*>();
    std::vector<Button*> buttonsMode1 = std::vector<Button*>();
    std::vector<ImageButton*> imageButtonsMode1 = std::vector<ImageButton*>();
    std::vector<Slider*> slidersMode1 = std::vector<Slider*>();

    std::vector<Labbel*> labbelsMode2 = std::vector<Labbel*>();
    std::vector<Button*> buttonsMode2 = std::vector<Button*>();
    std::vector<ImageButton*> imageButtonsMode2 = std::vector<ImageButton*>();
    std::vector<Slider*> slidersMode2 = std::vector<Slider*>();

    std::vector<Labbel*> labbelsMode3 = std::vector<Labbel*>();
    std::vector<Button*> buttonsMode3 = std::vector<Button*>();
    std::vector<ImageButton*> imageButtonsMode3 = std::vector<ImageButton*>();
    std::vector<Slider*> slidersMode3 = std::vector<Slider*>(); 
    std::vector<ToggleButton*> ToggleButtonsMode3 = std::vector<ToggleButton*>();

    void draw_mode1();
    void draw_mode2();
    void draw_mode3();
    void generate_UImodes1();
    void generate_UImodes2();
    void generate_UImodes3();

    Icon* icon = nullptr;
    int mode = 1;

    GameSettings* gameSettings = nullptr;
    GameSettings originalSettings;



    void checkButtonState();

    std::function<void()> closeButtonFunction;
    std::function<void()> saveButtonFunction;

    float originalMainVolume;
    float originalMusicVolume;
    float originalSfxVolume;
    void checkVolumeChange();
    void setScreen(int screen);

};

#endif // SettingsTool_H