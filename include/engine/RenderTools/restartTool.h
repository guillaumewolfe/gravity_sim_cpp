// RestartTool.h
#ifndef RestartTool_H
#define RestartTool_H

#include "RenderComponent.h"
#include "UI/ImageButton.h"
#include "UI/Labbel.h"
#include "UI/Buttons.h"
#include "UI/Icon.h"
#include <functional>

class RestartTool : public RenderComponent {
public:
    explicit RestartTool(RenderContext* renderContext);
    ~RestartTool() override;
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void Open();
    void Close();
    void setRestartFunction(const std::function<void(bool defaultState)>& func);
private:

    int winWidth, winHeight;
    bool loadStateActive = false;
    void drawUI();
    void initUI();

    void OpenLoad();
    void OpenDefault();

    void restartDefault();
    void restartLoadedState();

    Button* restartDefaultButton = nullptr;
    Button* restartLoadedStateButton = nullptr;
    Button* closeButton = nullptr;
    Labbel* messageLabbel= nullptr;
    Icon* notificationIcon = nullptr;
    ImageButton* closeImageButton = nullptr;

    std::function<void(bool defaultState)> restartFunction;

};

#endif // RestartTool_H