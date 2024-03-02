

#ifndef GameState_H
#define GameState_H
#include <iostream>
#include <filesystem>
#include <vector>  
#include <chrono>
#include "engine/Vec3.h"


class GameSettings;
class CelestialObject;
class RenderContext;
class RenderContext;

struct CelestialObjectData {
    std::string name;
    std::string typeName;
    Vec3 position;
    Vec3 velocity;
    double mass;
    double radius;
};


class GameState {

public:
    GameState();
    ~GameState();

    std::string getStateNameId();
    void setStateNameId(std::string stateNameID);
    int getGameStateId();

    void addCelestialObject(CelestialObject* celestialObject);
    void removeCelestialObject(CelestialObject* celestialObject);
    std::vector<CelestialObject*> getCelestialObjects();

    void setSimulationsimulationTimeStamp(std::chrono::time_point<std::chrono::system_clock> simulationTimeStamp);
    std::string getSimulationTimeStampString();

    CelestialObjectData getCelestialObjectDataFromString(std::string celestialObjectString);
    std::vector<CelestialObjectData> getCelestialObjectsData();
    void createCelestialObject(std::string name, Vec3 realPosition, Vec3 realVelocity, double mass, double radius);
    void addCelestialObjectsData(CelestialObjectData celestialObjectData);
    void GameStateStringToContext(std::string gameStateString); //Analyse le string et crée les objets correspondants

    std::string ContextToGameStateString(RenderContext* renderContext); //Analyse le contexte et crée le gameStateString correspondant

    std::string getDateCreated();

    CelestialObject* createCelestialObject(std::string celestialObjectString);

    void setDateCreated(std::string dateCreated);
    void setSimulationTimeStampString(std::string simulationTimeStampString);
    void setsimulationTimeStamp(std::chrono::time_point<std::chrono::system_clock> simulationTimeStamp);
    void setstateNameID(std::string stateNameID);
    void setGameStateId(int gameStateId);
    std::string simulationTimeStampToString(std::chrono::time_point<std::chrono::system_clock> simulationTimeStamp);
    std::chrono::time_point<std::chrono::system_clock> stringToSimulationTimeStamp(std::string simulationTimeStampString);
    CelestialObjectData getSunData(CelestialObject* excludeObject, std::vector<CelestialObjectData> celestialObjectsList);
    std::chrono::time_point<std::chrono::system_clock> getSimulationTimeStamp();

private:

    std::string stateNameID;
    std::vector<CelestialObject*> celestialObjects;
    std::string simulationDate;
    double simulationTime;
    std::vector<CelestialObjectData> celestialObjectsDataList;

    std::chrono::time_point<std::chrono::system_clock> simulationTimeStamp;
    std::string simulationTimeStampString;


    std::string dateCreated;
    int gameStateId;

    std::string CelestialObjectToString(CelestialObject* celestialObject);

    Vec3 stringToVec3(std::string vec3String);

};

#endif
