// UITool.h
#ifndef OBJECTSTOOL_H
#define OBJECTSTOOL_H

#include "RenderComponent.h"


class ObjectsTool : public RenderComponent {
public:

    explicit ObjectsTool(RenderContext* renderContext);
    // Override de la méthode Draw pour implémenter le dessin spécifique de l'UI
    void Draw() override;
    void drawSphere(float radius, GLuint texture, int slices, int stacks);
private:

};

#endif // OBJECTSTOOL_H