// RenderComponent.cpp
#include "engine/RenderTools/RenderComponent.h"

// Constructeur qui prend un pointeur vers RenderContext
RenderComponent::RenderComponent(RenderContext* renderContext)
    : m_renderContext(renderContext) {
}

RenderComponent::~RenderComponent() {
    // Implémentez le destructeur virtuel ici si nécessaire
}