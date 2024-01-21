#include "engine/CreationTools/CreatorManager.h"


CreatorManager::CreatorManager(RenderContext* renderContext) : RenderComponent(renderContext){
InitStates();
}

void CreatorManager::InitStates(){
std::unique_ptr<StateCreator> textureCreator(new TextureCreator(m_renderContext,this));
std::unique_ptr<StateCreator> positionCreator(new PositionCreator(m_renderContext,this));
std::unique_ptr<StateCreator> velocityCreator(new VelocityCreator(m_renderContext,this));
std::unique_ptr<StateCreator> parametersCreator(new ParametersCreator(m_renderContext,this));

AddState("TextureCreator", std::move(textureCreator));
AddState("PositionCreator", std::move(positionCreator));
AddState("VelocityCreator", std::move(velocityCreator));
AddState("ParametersCreator", std::move(parametersCreator));
}



void CreatorManager::Draw(){
    if (currentState) {
        currentState->Draw(); // Dessine selon l'état actuel
    }
}



void CreatorManager::DrawOpenGL(){
    if (currentState) {
        currentState->DrawOpenGL(); // Dessine OpenGL selon l'état actuel
    }
}

void CreatorManager::editObject(CelestialObject* object){
    isEditing = true;
    newCreatedObject = object;
    if (newCreatedObject->showPath){
        newCreatedObject->showPath = false;
        showPath = true;
    }
    *(m_renderContext->isCreating)=true;
    *(m_renderContext->showInfo) = false;
    ChangeState("PositionCreator");
    updateNames();
    positionInitialeEditing = object->getRealPosition();
    vitesseInitialeEditing = object->velocity;
}

std::string CreatorManager::getStateName(){
// Fonction pour obtenir le nom de l'état actuel
    for (const auto& pair : states) {
        if (pair.second.get() == currentState) {
            return pair.first;
        }
    }
    return ""; // Retourner une chaîne vide si aucun état correspondant n'est trouvé
}

void CreatorManager::ChangeState(const std::string& newStateName) {
    auto stateIter = states.find(newStateName);
    if (stateIter != states.end()) {
        // Si l'état actuel existe, appeler sa méthode Exit
        if (currentState) {
            currentState->Exit();
        }

        // Mettre à jour l'état actuel et appeler sa méthode Enter
        currentState = stateIter->second.get();
        if (currentState) {
            currentState->Enter();
        }
    } else {
        std::cout<<"L'état n'existe pas"<<std::endl;
    }
}


void CreatorManager::AddState(const std::string& stateName, std::unique_ptr<StateCreator> state) {
    // Ajoute l'état dans le map des états, en utilisant stateName comme clé
    states[stateName] = std::move(state);

    // Si c'est le premier état ajouté, définissez-le comme état courant
    if (currentState == nullptr && !states.empty()) {
        currentState = states.begin()->second.get();
        currentState->Enter(); // Appel de Enter pour le premier état
    }


}



void CreatorManager::Exit(){
    currentState->Exit();
    *(m_renderContext->isCreating)=false;
    if(creationConfirmed && !isEditing){
        m_renderContext->currentCamera->newFollowObject(newCreatedObject);
        m_renderContext->currentCamera->selectedObject = newCreatedObject;
        *(m_renderContext->showInfo) = true;
    }
    if(!creationConfirmed && !isEditing){//Si la création n'a pas été confirmée, on supprime l'objet
        if(newCreatedObject!=nullptr){
            m_renderContext->systemeSolaire->removeObject(newCreatedObject);
            newCreatedObject = nullptr;
            m_renderContext->currentCamera->resetPosition();
            *(m_renderContext->showInfo) = true;
            m_renderContext->currentCamera->resetPosition();
        }
    }
    if(isEditing){
        newCreatedObject->showPath = showPath;
        newCreatedObject->clearPositionHistory();
        if(!isEditingConfirmed){
            newCreatedObject->position_real = positionInitialeEditing;
            newCreatedObject->velocity = vitesseInitialeEditing;
        }
        m_renderContext->currentCamera->newFollowObjectGlobal(newCreatedObject);
    }
    updateNames();
    creationConfirmed = false;
    isEditingConfirmed = false;
    isEditing = false;
    ChangeState("TextureCreator");
}


void CreatorManager::resetCamera(){
    m_renderContext->currentCamera->creationMode();
    m_renderContext->currentCamera->setPosition(cameraCreationPositionInit);
    m_renderContext->currentCamera->setTarget(cameraCreationTargetInit);
    cameraCreationTarget = cameraCreationTargetInit;
    cameraCreationPosition = cameraCreationPositionInit;
}

void CreatorManager::updateCamera(){
    m_renderContext->currentCamera->setPosition(cameraCreationPosition);
    m_renderContext->currentCamera->setTarget(cameraCreationTarget);
}

void CreatorManager::setUpdateNamesFunction(const std::function<void()>& func) {
    updateNamesFunction = func;
}

void CreatorManager::updateNames(){
    updateNamesFunction();
}