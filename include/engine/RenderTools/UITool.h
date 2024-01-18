// UITool.h
#ifndef UITOOL_H
#define UITOOL_H

#include "RenderComponent.h"
#include <chrono>
#include "UI/ImageButton.h"
#include "UI/Icon.h"

class UITool : public RenderComponent {
public:
    float sliderVariable;
    explicit UITool(RenderContext* renderContext);
    ~UITool() override;
    
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void draw_labbels();
    void draw_buttons();
    void draw_rect();
    GLuint backgroundTexture;
    std::chrono::system_clock::time_point start_time;
    void update_time();
    float lastScroll = 0.0f;

    GLuint textureID;

    ImageButton* pauseButton;

private:
    int winWidth, winHeight;
    ImFont* myFont;
    std::chrono::steady_clock::time_point lastFrameTime;
    int frameCount;
    float fps;
    void checkScroll();

};

#endif // UITOOL_H