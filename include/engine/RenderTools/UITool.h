// UITool.h
#ifndef UITOOL_H
#define UITOOL_H

#include "RenderComponent.h"

class UITool : public RenderComponent {
public:

    explicit UITool(RenderContext* renderContext);
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void draw_labbels();
    void draw_buttons();
};

#endif // UITOOL_H