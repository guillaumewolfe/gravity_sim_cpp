#ifndef StateCreator_H
#define StateCreator_H
#include "engine/RenderTools/RenderContext.h"


class CreatorManager;

class StateCreator {
public:
    virtual ~StateCreator() = default;

    explicit StateCreator(RenderContext* renderContext, CreatorManager* manager);
    virtual void Enter() = 0; // Appelé lors de l'entrée dans l'état
    virtual void Draw() = 0; // Appelé à chaque frame pour mettre à jour l'état
    virtual void DrawOpenGL() = 0;
    virtual void Exit() = 0; // Appelé lors de la sortie de l'état


protected:

    RenderContext* m_renderContext;
    CreatorManager* m_manager;
};

#endif // StateCreator_H