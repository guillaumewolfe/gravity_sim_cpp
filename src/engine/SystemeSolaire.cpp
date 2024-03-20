#include "engine/SystemeSolaire.h"
#include <iostream>
#include "engine/RenderTools/objectsTool.h"
#include "engine/RenderTools/RenderContext.h"
#include "engine/RenderTools/uranusRingTool.h"

SystemeSolaire::SystemeSolaire(){
    maxSize = 1000;
    radiusScale = 2.0f;
    apiTool = new ApiTool();
    initialStateObjects = initSystem();
    objects = initialStateObjects;
    scale = getScale();
    syncWithNasa();
    setRayonInit();

    for (auto& object : objects) {
        if(object->getName()=="Pluto"){
            std::cout<<"Pluto radius"<<object->getRayon()<<std::endl;
        }
    }
    }

// Destructeur
SystemeSolaire::~SystemeSolaire() {
    for (auto& object : objects) {
        delete object;
    }
    objects.clear();
    deletedObjects.clear();
    delete apiTool;
}


std::vector<CelestialObject*> SystemeSolaire::initSystem(){
    std::vector<CelestialObject*> celestialObjects;
    
    Sun* sun = new Sun();
    Mercure* mercure = new Mercure();
    Venus* venus = new Venus();
    Earth* earth = new Earth();
    Moon* moon = new Moon();
    Mars* mars = new Mars();
    Jupiter* jupiter = new Jupiter();
    Saturn* saturn = new Saturn();
    Uranus* uranus = new Uranus();
    Neptune* neptune = new Neptune();
    //Pluto* pluto = new Pluto();
    


    celestialObjects.push_back(sun);
    celestialObjects.push_back(mercure);
    celestialObjects.push_back(venus);
    celestialObjects.push_back(earth);
    celestialObjects.push_back(moon);
    celestialObjects.push_back(mars);
    celestialObjects.push_back(jupiter);
    celestialObjects.push_back(saturn);
    celestialObjects.push_back(uranus);
    celestialObjects.push_back(neptune);
    //celestialObjects.push_back(pluto);
    return celestialObjects;
}

void SystemeSolaire::updatePhysics(double dt){}

double SystemeSolaire::getScale(){
    double maxLength = 0;
    CelestialObject* maxDistanceObject = nullptr;
    for (auto& object : objects){
        if(object->position_real.norm()>maxLength){maxLength = object->position_real.norm(); maxDistanceObject = object;}
    }
    scale = maxSize/maxLength;
    std::cout << "Max object distance: " << maxDistanceObject->getName() << " " << maxDistanceObject->position_real.norm() << std::endl;
    return scale;
}

void SystemeSolaire::setRayonInit(){
    for (auto& object : objects){
        object->setRayonSim(radiusScale*scale);
        object->distanceScale = scale;
        object->setInitialSettings();
    }   
}
void SystemeSolaire::setRayon(CelestialObject* obj){
    obj->setRayonSim(radiusScale*scale);
    ObjectsTool::initSphere(*obj, 150, 150);
    updateEffects(obj);
}

void SystemeSolaire::syncWithNasa(){
    if(objects.empty()){return;}
    std::string sunString = apiTool->getBodyData("Sun");
    bool connexionFailed = false;
    std::pair<Vec3, Vec3> sunPositionAndVelocity = apiTool->extractBodyData(sunString, "Sun", &connexionFailed);
    if(connexionFailed){return;}
    Vec3 sunPosition = sunPositionAndVelocity.first;
    Vec3 sunVelocity = sunPositionAndVelocity.second;
    for(auto& object : objects){
        std::string buffer = apiTool->getBodyData(object->getName());
        std::pair<Vec3, Vec3> positionAndVelocity = apiTool->extractBodyData(buffer,object->getName(), &connexionFailed);
        // Multipliez les valeurs par 1000
        Vec3 newPosition = positionAndVelocity.first * 1000;
        Vec3 newVelocity = positionAndVelocity.second * 1000;
        object->nasaPosition = newPosition;
        object->nasaVelocity = newVelocity;
    }    
    bool connectionSuccess = apiTool->connectionSuccess;
    
}


void SystemeSolaire::resetPosition() {
    for (auto& object : objects) {
        if (!object) continue; // Sautez les pointeurs nuls

        // Votre logique existante ici...
    }
    // Réinitialiser la position et la vitesse des objets originaux
    for (auto& object : objects) {
        object->resetPlanetEaten();
        bool rayonChanged = false;
        if (!object->isCreated) {
            if(isLoadedState){
                object->distanceScale = scale;
                object->updatePositionReal(object->defaultPosition);
                object->updatePositionSimulation();
                object->updateVelocity(object->defaultVelocity);
                
            }else{
                object->updatePositionReal(object->nasaPosition);
                object->updateVelocity(object->nasaVelocity);
            }

            object->clearPositionHistory();
            if (object->real_radius != object->initialRadius) {
                rayonChanged = true;
            }
            object->resetToDefaultValues(); //Reset des caractéristiques initiales
            if(rayonChanged){
                setRayon(object);
            }
            object->isDeleted = false;
            object->showName = true;
            object->showOrbit = false;
            object->showPath = true;
        }
    }
}

void SystemeSolaire::addObject(CelestialObject* newObj){
    if(newObj->real_radius){
    setRayon(newObj);
    }else{
    }
    objects.push_back(newObj);
}

void SystemeSolaire::removeObject(CelestialObject* objToRemove) {

    auto it = std::find(objects.begin(), objects.end(), objToRemove);
    if (it != objects.end()) {
        if (!objToRemove->isCreated) {
            size_t position = std::distance(objects.begin(), it);
            deletedObjects.emplace_back(objToRemove, position);
        }
        objects.erase(it); // Enlever l'objet de la liste
        objToRemove->isDeleted = true;
    } else {
        std::cout << "Objet non trouvé." << std::endl;
    }
}

void SystemeSolaire::updateEffects(CelestialObject* newObj){
    if(newObj->getTypeName()=="Saturn"){
        if (newObj->saturnRingTool != nullptr) {
            delete newObj->saturnRingTool;
            newObj->saturnRingTool = nullptr;
        }
        newObj->saturnRingTool = new SaturnRingTool(newObj, m_renderContext);
    } else if(newObj->getTypeName()=="Earth"){
        if (newObj->athmosphereTool != nullptr) {
            delete newObj->athmosphereTool;
            newObj->athmosphereTool = nullptr;
        }
        newObj->athmosphereTool = new AthmosphereTool(newObj, m_renderContext);
    } else if(newObj->getTypeName()=="Sun"){
        if (newObj->glowTool != nullptr) {
            delete newObj->glowTool;
            newObj->glowTool = nullptr;
        }
        newObj->glowTool = new GlowTool(newObj, m_renderContext);
    } else if(newObj->getTypeName()=="Uranus"){
        if (newObj->uranusRingTool != nullptr) {
            delete newObj->uranusRingTool;
            newObj->uranusRingTool = nullptr;
        }
        newObj->uranusRingTool = new UranusRingTool(newObj, m_renderContext);
    }
}


void SystemeSolaire::setContext(RenderContext* renderContext){
    m_renderContext = renderContext;
}

CelestialObject* SystemeSolaire::getSun(CelestialObject* excludeObject) {
    if (objects.empty()) {
        return nullptr;
    }

    CelestialObject* mostMassive = nullptr;
    for (auto& object : objects) {
        // Vérifiez si cet objet est celui à exclure
        if (object == excludeObject) {
            continue; // Passez à l'objet suivant si c'est celui à exclure
        }

        // Si c'est le premier objet valide ou si l'objet est un "sun", traitez-le en conséquence
        if (mostMassive == nullptr || object->type == 1) {
            mostMassive = object;
            if (object->type == 1) {
                // Si l'objet est un "sun", retournez-le immédiatement
                return object;
            }
        }

        // Sinon, continuez à chercher l'objet le plus massif
        else if (object->getWeight() > mostMassive->getWeight()) {
            mostMassive = object;
        }
    }

    // Si aucun "sun" n'a été trouvé, retournez l'objet le plus massif
    return mostMassive;
}

std::vector<CelestialObject*> SystemeSolaire::getObjects() {
    return objects;
}


void SystemeSolaire::loadState(std::vector<CelestialObject*> loadedObjects) {
    // Nettoyez l'état actuel
    for (auto& object : objects) {
        if(object->isCreated) delete object; // Assurez-vous de libérer la mémoire si nécessaire
    }
    objects.clear();

    // Chargez le nouvel état
    objects = loadedObjects;
    if(isLoadedState){
        loadedStateObjects = loadedObjects;
    }

    resetPosition();
}

void SystemeSolaire::reset(bool toInitialState) {

    isLoadedState = !toInitialState;
    if (toInitialState) {
        loadState(initialStateObjects); // Réinitialisez à l'état initial
    } else {
        loadState(loadedStateObjects); // Réinitialisez à l'état chargé
    }
}