// UITool.h
#ifndef BackgroundImageTool_H
#define BackgroundImageTool_H

#include "RenderComponent.h"
#include "engine/Vec3.h"


class BackgroundImageTool : public RenderComponent {
public:

    explicit BackgroundImageTool(RenderContext* renderContext);
    ~BackgroundImageTool() override;
    void Draw() override;
    Camera* m_camera;



private:

    int winWidth, winHeight;
    GLuint textureID;
    void loadImage();
};

#endif // BackgroundImageTool_H