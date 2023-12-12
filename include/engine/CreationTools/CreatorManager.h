#ifndef CreatorManager_H
#define CreatorManager_H

#include <memory>
#include <map>
#include <string>

#include "StateCreator.h"
#include "../RenderTools/RenderComponent.h"
#include "engine/CreationTools/TextureCreator.h"
#include "engine/CreationTools/PositionCreator.h"
#include "engine/CreationTools/VelocityCreator.h"
#include "engine/CreationTools/ParametersCreator.h"
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
    bool isCreated = false;
    bool creationConfirmed = false;

    Vec3 cameraCreationPosition;
    Vec3 cameraCreationPositionInit = Vec3(0,125,0);
    Vec3 cameraCreationTarget;
    Vec3 cameraCreationTargetInit = Vec3(0,0,0);
    void resetCamera();
    void updateCamera();


private:
    std::map<std::string, std::unique_ptr<StateCreator>> states;
    StateCreator* currentState = nullptr;
};

#endif // CreatorManager_H