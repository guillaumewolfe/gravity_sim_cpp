
#include "save/GameState.h"
#include "engine/RenderTools/RenderContext.h"
#include "engine/CelestialObject.h" 




GameState::GameState(){
}

GameState::~GameState(){
    for (auto& object : celestialObjects){
        delete object;
    }
}

std::string GameState::getStateNameId(){
    return stateNameID;
}

void GameState::setStateNameId(std::string stateNameID){
    this->stateNameID = stateNameID;
}

void GameState::addCelestialObject(CelestialObject* celestialObject){
    celestialObjects.push_back(celestialObject);
}

void GameState::removeCelestialObject(CelestialObject* celestialObject){
    for (int i = 0; i < celestialObjects.size(); i++){
        if (celestialObjects[i] == celestialObject){
            celestialObjects.erase(celestialObjects.begin() + i);
        }
    }
}


std::vector<CelestialObject*> GameState::getCelestialObjects(){
    return celestialObjects;
}


void GameState::setSimulationsimulationTimeStamp(std::chrono::time_point<std::chrono::system_clock> simulationTimeStamp){

}

std::string GameState::getSimulationTimeStampString(){
    //if string empty, convert simulationTimeStamp to string
    if (simulationTimeStampString.empty()){
        simulationTimeStampString = simulationTimeStampToString(simulationTimeStamp);
    }
    return simulationTimeStampString;
}

//get simulationTimeStamp
std::chrono::time_point<std::chrono::system_clock> GameState::getSimulationTimeStamp(){
    return simulationTimeStamp;
}


void GameState::GameStateStringToContext(std::string gameStateString){

}

CelestialObject* GameState::createCelestialObject(std::string celestialObjectString){
    std::vector<std::string> celestialObjectStringVector;
    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;
    while ((pos = celestialObjectString.find(delimiter)) != std::string::npos) {
        token = celestialObjectString.substr(0, pos);
        celestialObjectStringVector.push_back(token);
        celestialObjectString.erase(0, pos + delimiter.length());
    }
    celestialObjectStringVector.push_back(celestialObjectString);
    std::string name = celestialObjectStringVector[0].substr(5);
    std::string typeName = celestialObjectStringVector[1].substr(9);
    Vec3 position = stringToVec3(celestialObjectStringVector[2].substr(9));
    Vec3 velocity = stringToVec3(celestialObjectStringVector[3].substr(9));
    double mass = std::stod(celestialObjectStringVector[4].substr(5));
    double radius = std::stod(celestialObjectStringVector[5].substr(7));    

    CelestialObject* m_object = nullptr;
    bool objectCreated = true;
    if(typeName == "Sun"){m_object = new Sun();}
    else if(typeName == "Mercury"){m_object = new Mercure();}
    else if(typeName == "Venus"){m_object = new Venus();}
    else if(typeName == "Earth"){m_object = new Earth();}
    else if(typeName == "Moon"){m_object = new Moon();}
    else if(typeName == "Mars"){m_object = new Mars();}
    else if(typeName == "Jupiter"){m_object = new Jupiter();}
    else if(typeName == "Saturn"){m_object = new Saturn();}
    else if(typeName == "Uranus"){m_object = new Uranus();}
    else if(typeName == "Neptune"){m_object = new Neptune();}
    else if(typeName == "BlackHole"){m_object = new BlackHole();}
    else if(typeName == "Volcanic"){m_object = new Volcanic();}
    else if(typeName == "Ice") {m_object = new Ice();}
    else if(typeName == "Rock"){m_object = new Rock();}
    else if(typeName == "Ice"){m_object = new Ice();}
    else{objectCreated = false;}
    if(objectCreated){
        m_object->name = name;
        m_object->typeName = typeName;
        m_object->position_real = position;
        m_object->velocity = velocity;
        m_object->setWeight(mass);
        m_object->real_radius = radius;
        m_object->setDefaultValues();
    }
    //Std::cout infos about new object
    //std::cout << "Celestial Object created: " << m_object->name << " " << m_object->typeName << " " << m_object->position_real.x << " " << m_object->position_real.y << " " << m_object->position_real.z << " " << m_object->velocity.x << " " << m_object->velocity.y << " " << m_object->velocity.z << " " << m_object->getWeight() << " " << m_object->real_radius << std::endl;
    return m_object;
}

CelestialObjectData GameState::getCelestialObjectDataFromString(std::string celestialObjectString){
    CelestialObjectData celestialObjectData;
    std::vector<std::string> celestialObjectStringVector;
    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;
    while ((pos = celestialObjectString.find(delimiter)) != std::string::npos) {
        token = celestialObjectString.substr(0, pos);
        celestialObjectStringVector.push_back(token);
        celestialObjectString.erase(0, pos + delimiter.length());
    }
    celestialObjectStringVector.push_back(celestialObjectString);
    std::string name = celestialObjectStringVector[0].substr(5);
    std::string typeName = celestialObjectStringVector[1].substr(9);
    Vec3 position = stringToVec3(celestialObjectStringVector[2].substr(9));
    Vec3 velocity = stringToVec3(celestialObjectStringVector[3].substr(9));
    double mass = std::stod(celestialObjectStringVector[4].substr(5));
    double radius = std::stod(celestialObjectStringVector[5].substr(7));    
    celestialObjectData.name = name;
    celestialObjectData.typeName = typeName;
    celestialObjectData.position = position;
    celestialObjectData.velocity = velocity;
    celestialObjectData.mass = mass;
    celestialObjectData.radius = radius;
    return celestialObjectData;
}


std::string GameState::ContextToGameStateString(RenderContext* renderContext){
/* String model
[GameState18]
GameStateID=save1
Time=5.540032
Name=Sun,TypeName=Sun,Position=-1152690460.362360,31071222.714849,488031611.699277,Velocity=-0.000000,0.000000,0.000000,Mass=1988500.000000,Radius=696340.000000
Name=Mercury,Position=42490291533.599174,-7561286403.934082,44409180014.736252 ,Velocity=0.000000,0.000000,0.000000,Mass=0.330000,Radius=2439.700000
Name=Venus,Position=-2604078134.534499,-1378349457.812142,109212611628.104843,Velocity=0.000000,0.000000,0.000000,Mass=4.870000,Radius=6051.800000
Name=Earth,Position=-132376366116.273148,27629617.143289,-67841012545.613029,Velocity=0.000000,0.000000,0.000000,Mass=5.970000,Radius=6371.000000
Name=Moon,Position=-132603959205.117371,61771455.354033,-68170783424.015739,Velocity=0.000000,0.000000,0.000000,Mass=0.073460,Radius=1737.100000
Name=Mars,Position=66452961073.946098,-5849129840.494658,201942514670.680817,Velocity=0.000000,0.000000,0.000000,Mass=0.642000,Radius=3389.500000
Name=Jupiter,Position=477208322763.686218,-13055746033.769842,-573507693267.766724,Velocity=0.000000,0.000000,0.000000,Mass=1898.000000,Radius=69911.000000
Name=Saturn,Position=1358179925588.154053,-45101423136.985893,516129002935.212158,Velocity=0.000000,0.000000,0.000000,Mass=568.000000,Radius=58232.000000
Name=Uranus,Position=1810338217436.339600,-14888585976.837662,-2306038808809.396973,Velocity=0.000000,0.000000,0.000000,Mass=86.800000,Radius=25362.000000
Name=Neptune,Position=4464491152431.009766,-97867445504.394760,243836349808.002563,Velocity=0.000000,0.000000,0.000000,Mass=102.000000,Radius=24622.000000
[EndGameState]
*/
    
//From current contect, help me create the string, I dont need the GameStateID and the guards
std::string data;
    std::string timeString = simulationTimeStampToString(renderContext->getCurrentTime());
data = "SimulationTime="+timeString;
std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
std::string nowString = simulationTimeStampToString(now);
data+= "CreatedTime="+nowString;

    for (auto& object : renderContext->systemeSolaire->getObjects()){
        data += CelestialObjectToString(object) + "\n";
    }
    return data;
}

std::string GameState::CelestialObjectToString(CelestialObject* celestialObject){
    //Vec3 such as position and velocity are converted to string manually, accessing x, y and z that are doubles
    std::string celestialObjectString = "Name=" + celestialObject->name +";TypeName="+celestialObject->typeName + ";Position=" + std::to_string(celestialObject->position_real.x) + "," + std::to_string(celestialObject->position_real.y) + "," + std::to_string(celestialObject->position_real.z) + ";Velocity=" + std::to_string(celestialObject->velocity.x) + "," + std::to_string(celestialObject->velocity.y) + "," + std::to_string(celestialObject->velocity.z) + ";Mass=" + std::to_string(celestialObject->getWeight()) + ";Radius=" + std::to_string(celestialObject->real_radius);
    return celestialObjectString;
}


std::string GameState::getDateCreated(){
    return dateCreated;
}

std::string GameState::simulationTimeStampToString(std::chrono::time_point<std::chrono::system_clock> simulationTimeStamp){
    std::time_t time = std::chrono::system_clock::to_time_t(simulationTimeStamp);
    std::string timeString = std::ctime(&time);
    return timeString;
}

std::chrono::time_point<std::chrono::system_clock> GameState::stringToSimulationTimeStamp(std::string simulationTimeStampString){
    std::tm tm = {};
    std::istringstream ss(simulationTimeStampString);
    ss >> std::get_time(&tm, "%c");
    std::time_t time = std::mktime(&tm);
    std::chrono::time_point<std::chrono::system_clock> timePoint = std::chrono::system_clock::from_time_t(time);
    return timePoint;
}

Vec3 GameState::stringToVec3(std::string vec3String){
    std::vector<std::string> vec3StringVector;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = vec3String.find(delimiter)) != std::string::npos) {
        token = vec3String.substr(0, pos);
        vec3StringVector.push_back(token);
        vec3String.erase(0, pos + delimiter.length());
    }
    vec3StringVector.push_back(vec3String);
    Vec3 vec3 = Vec3(std::stod(vec3StringVector[0]), std::stod(vec3StringVector[1]), std::stod(vec3StringVector[2]));
    return vec3;
}

void GameState::setDateCreated(std::string dateCreated){
    this->dateCreated = dateCreated;
}

void GameState::setSimulationTimeStampString(std::string simulationTimeStampString){
    this->simulationTimeStampString = simulationTimeStampString;
}

void GameState::setsimulationTimeStamp(std::chrono::time_point<std::chrono::system_clock> simulationTimeStamp){
    this->simulationTimeStamp = simulationTimeStamp;
}

void GameState::setstateNameID(std::string stateNameID){
    this->stateNameID = stateNameID;
}

void GameState::setGameStateId(int gameStateId){
    this->gameStateId = gameStateId;
}

void GameState::addCelestialObjectsData(CelestialObjectData celestialObjectData){
    celestialObjectsDataList.push_back(celestialObjectData);
}

std::vector<CelestialObjectData> GameState::getCelestialObjectsData(){
    return celestialObjectsDataList;
}

int GameState::getGameStateId(){
    return gameStateId;
}


CelestialObjectData GameState::getSunData(CelestialObject* excludeObject, std::vector<CelestialObjectData> celestialObjectsList) {
    if (celestialObjectsList.empty()) {
        return CelestialObjectData(); // Return an empty object if the list is empty
    }

    CelestialObjectData mostMassive;
    for (auto& object : celestialObjectsList) {
        if (excludeObject!=nullptr && object.name == excludeObject->name) {
            continue; // Skip the excluded object
        }

        // If it's the first valid object or a sun, select it
        if (mostMassive.name=="" || object.typeName == "Sun") {
            mostMassive = object;
            if (object.typeName == "Sun") {
                break; // Stop searching if a sun is found
            }
        } else if (object.mass > mostMassive.mass) {
            // Otherwise, keep looking for the most massive object
            mostMassive = object;
        }
    }

    // Convert the most massive or sun object found to CelestialObjectData
    if (mostMassive.name != "") {
        return CelestialObjectData{
            mostMassive.name,
            mostMassive.typeName,
            mostMassive.position, // Assuming this is a Vec3
            mostMassive.velocity, // Assuming this is a Vec3
            mostMassive.mass,
            mostMassive.radius
        };
    } else {
        return CelestialObjectData(); // Return an empty object if none found/mostMassive is nullptr
    }
}