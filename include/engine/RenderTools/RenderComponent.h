// RenderComponent.h
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "RenderContext.h"

class RenderComponent {
public:
    // Constructeur qui prend un pointeur vers RenderContext
    explicit RenderComponent(RenderContext* renderContext);

    // Assurez-vous d'avoir un destructeur virtuel pour une classe de base avec des méthodes virtuelles
    virtual ~RenderComponent();

    // Méthode virtuelle pure pour le dessin, à implémenter par les sous-classes
    virtual void Draw() = 0;

protected:
    // Un pointeur vers le RenderContext pour accéder aux éléments de rendu nécessaires
    RenderContext* m_renderContext;
};

#endif // RENDERCOMPONENT_H