#ifndef CreatorManager_H
#define CreatorManager_H

#include <memory>
#include <map>
#include <string>
#include <functional>

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

    CelestialObject* newCreatedObject = nullptr;
    bool isCreated = false;
    bool creationConfirmed = false;

    Vec3 cameraCreationPosition = Vec3(0,125,0);
    Vec3 cameraCreationPositionInit = Vec3(0,125,0);
    Vec3 cameraCreationTarget = Vec3(0,0,0);
    Vec3 cameraCreationTargetInit = Vec3(0,0,0);
    void resetCamera();
    void updateCamera();
    void setUpdateNamesFunction(const std::function<void()>& func);
    bool isEditing = false;
    bool isEditingConfirmed = false;
    bool showPath = false;
    void editObject(CelestialObject* object);
    Vec3 positionInitialeEditing = Vec3(0,0,0);
    Vec3 vitesseInitialeEditing = Vec3(0,0,0);
     
    std::string getStateName();
    std::map<std::string, std::unique_ptr<StateCreator>> states;
    std::function<void()> updateNamesFunction;
    void createNewObject();
    

private:

    StateCreator* currentState = nullptr;

    void updateNames();
};

#endif // CreatorManager_H