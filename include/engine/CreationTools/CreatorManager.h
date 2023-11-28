#ifndef CreatorManager_H
#define CreatorManager_H

#include <memory>
#include <map>
#include <string>

#include "StateCreator.h"
#include "../RenderTools/RenderComponent.h"
#include "engine/CreationTools/TextureCreator.h"
#include "engine/CreationTools/PositionCreator.h"

#include "engine/CelestialObject.h"


class CreatorManager: public RenderComponent {
public:

    explicit CreatorManager(RenderContext* renderContext);
    void AddState(const std::string& stateName, std::unique_ptr<StateCreator> state);
    void ChangeState(const std::string& newState);
    void UpdateCurrentState();
    void Draw();
    void DrawOpenGL();
    void InitStates();
    void Exit();

    CelestialObject* newCreatedObject;

private:
    std::map<std::string, std::unique_ptr<StateCreator>> states;
    StateCreator* currentState = nullptr;
};

#endif // CreatorManager_H