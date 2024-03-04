
#include "save/saveTool.h"
#include <fstream>
#include <string>
#include <vector>
#include "path_util.h"
#include "game/game.h"
#include "engine/CelestialObject.h"
#include "engine/RenderTools/RenderContext.h"
#include <regex>


SaveTool::SaveTool(){
    saveFilepath = getSavePath();
}

SaveTool::~SaveTool(){}

std::string SaveTool::getSavePath() {
    
    std::string path = getFullPath("assets/CurlCertif/dat.dat");

    // Check if the file exists
    if (!fs::exists(path)) {
        // Create and immediately close the file, resulting in an empty file
        std::ofstream(path).close();
    }

    return path;
}

std::string SaveTool::encryptDecrypt(const std::string& data) {
    char key = 'X'; // Encryption key. Use a more complex key for better security.
    std::string result = data;

    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key;
    }

    return result;
}


void SaveTool::saveGameSettings(const GameSettings& settings) {
    // Vérifiez si les paramètres sont déjà écrits et les supprimez si nécessaire
    checkIfSettingsAlreadyWritten(true);
    
    // Lire le contenu existant
    std::ifstream fileIn(saveFilepath, std::ios::binary);
    std::string existingContent;
    if (fileIn.is_open()) {
        std::string line;
        while (std::getline(fileIn, line)) {
            existingContent += line + "\n";
        }
        fileIn.close();
    } else {
        std::cerr << "Failed to open file for reading." << std::endl;
    }

    // Ajouter les nouvelles données de configuration
    std::string data = "[GAMESETTINGS]\n";
    data += "resolutionX=" + std::to_string(settings.resolutionX) + "\n";
    data += "resolutionY=" + std::to_string(settings.resolutionY) + "\n";
    data += "fullscreen=" + std::to_string(settings.fullscreen) + "\n";
    data += "showFPS=" + std::to_string(settings.showFPS) + "\n";
    data += "antiAliasing=" + std::to_string(settings.antiAliasing) + "\n";
    data += "textureQuality=" + std::to_string(settings.textureQuality) + "\n";
    data += "highQuality=" + std::to_string(settings.highQuality) + "\n";
    data += "movementSpeed=" + std::to_string(settings.movementSpeed) + "\n";
    data += "rotationSpeed=" + std::to_string(settings.rotationSpeed) + "\n";
    data += "mainVolume=" + std::to_string(settings.mainVolume) + "\n";
    data += "musicVolume=" + std::to_string(settings.musicVolume) + "\n";
    data += "sfxVolume=" + std::to_string(settings.sfxVolume) + "\n";
    data += "volumeChanged=" + std::to_string(settings.volumeChanged) + "\n";
    data += "[ENDGAMESETTINGS]\n";

    // Chiffrer les nouvelles données de configuration
    std::string encryptedData = encryptDecrypt(data);

    // Ajouter les nouvelles données chiffrées au contenu existant
    existingContent += encryptedData;

    // Réécrire le fichier avec le contenu combiné
    std::ofstream fileOut(saveFilepath, std::ios::binary);
    if (!fileOut.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    fileOut.write(existingContent.c_str(), existingContent.size());
    fileOut.close();
}

bool SaveTool::checkIfSettingsAlreadyWritten(bool removeExistingSettings) {
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return 0;
    }

    // Read the encrypted data from the file
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Decrypt the content
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    // Now search for the markers in the decrypted content
    size_t startPos = decryptedContent.find("[GAMESETTINGS]");
    size_t endPos = decryptedContent.find("[ENDGAMESETTINGS]");

    // Check if both markers are found
    if (startPos != std::string::npos && endPos != std::string::npos) {
        
        // Remove the existing settings
        if(removeExistingSettings){decryptedContent.erase(startPos, endPos - startPos + 17);}

        return true;
    } else {
        return false;
    }
}

void SaveTool::loadSettings(GameSettings* settings) {
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }

    // Read the encrypted data from the file
    std::string encryptedData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Decrypt the data
    std::string decryptedData = encryptDecrypt(encryptedData);

    // Use a stringstream to process the decrypted data line by line
    std::istringstream stream(decryptedData);
    std::string line;

    while (std::getline(stream, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // Print each setting for verification
            //std::cout << key << " = " << value << std::endl;

            // Update settings based on key
            if (key == "resolutionX") settings->resolutionX = std::stoi(value);
            else if (key == "resolutionY") settings->resolutionY = std::stoi(value);
            else if (key == "fullscreen") settings->fullscreen = std::stoi(value) != 0;
            else if (key == "showFPS") settings->showFPS = std::stoi(value) != 0;
            else if (key == "antiAliasing") settings->antiAliasing = std::stoi(value);
            else if (key == "textureQuality") settings->textureQuality = std::stoi(value); 
            else if (key == "highQuality") settings->highQuality = std::stoi(value) != 0;
            else if (key == "movementSpeed") settings->movementSpeed = std::stof(value);
            else if (key == "rotationSpeed") settings->rotationSpeed = std::stof(value);
            else if (key == "mainVolume")settings->mainVolume = std::stof(value);
            else if (key == "musicVolume")settings->musicVolume = std::stof(value);
            else if (key == "sfxVolume") settings->sfxVolume = std::stof(value);
            else if (key == "volumeChanged") settings->volumeChanged = std::stoi(value) != 0;
        }
    }
}





/////GAME STATE//////////////////////////////////////////////////////

//Check if gameState already exists: 

int SaveTool::checkExistingGameState() {
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return 0;
    }

    // Read the encrypted data from the file
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Decrypt the content
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    int highestIndex = 0;
    std::string markerBase = "[GameState";
    size_t startPos = 0;

    // Now search in the decrypted content
    while ((startPos = decryptedContent.find(markerBase, startPos)) != std::string::npos) {
        startPos += markerBase.length();
        size_t endPos = decryptedContent.find("]", startPos);
        if (endPos != std::string::npos) {
            int index = std::stoi(decryptedContent.substr(startPos, endPos - startPos));
            highestIndex = std::max(highestIndex, index);
        }
    }

    return highestIndex;
}

//Save the game state

void SaveTool::saveGameState(RenderContext* Context, std::string gameStateId) {
    int gameStateIndex = checkExistingGameState() + 1;

    std::vector<CelestialObject*> objects = Context->systemeSolaire->objects;
    std::string dataStream;

    // Start writing the game state data to a string stream (to be encrypted)
    std::string gameStateMarker = "[GameState" + std::to_string(gameStateIndex) + "]\n";
    dataStream += gameStateMarker; // Start game state marker
    dataStream += "GameStateID=" + gameStateId + "\n"; // Save the game state name

    GameState gameState;
    dataStream += gameState.ContextToGameStateString(Context);

    dataStream += "[EndGameState]\n"; // End game state marker
    // Encrypt the data
    std::string encryptedData = encryptDecrypt(dataStream);

    // Write the encrypted data to file
    std::ofstream file(saveFilepath, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    file.write(encryptedData.c_str(), encryptedData.size());
    file.close();
    readState(0);
}

//ReadGameState



void SaveTool::loadGameState(RenderContext* Context, GameState* gameState) {
    int highestIndex = checkExistingGameState();
    int gameStateIndex = gameState->getGameStateId();
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }

    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Decrypt the content
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    std::string targetMarker = "[GameState" + std::to_string(gameStateIndex) + "]";
    std::string endMarker = "[EndGameState]";
    size_t start = decryptedContent.find(targetMarker);
    size_t end = decryptedContent.find(endMarker, start);

    if (start == std::string::npos || end == std::string::npos) {
        std::cerr << "Game state " << gameStateIndex << " not found." << std::endl;
        return;
    }
    std::string gameStateData = decryptedContent.substr(start, end - start + endMarker.length());
    std::istringstream stream(gameStateData);
    std::string line;
    while (std::getline(stream, line)) {
        //std::cout<<line<<std::endl;
        //If name=Sun
        if (line.find("Name=") != std::string::npos){
            CelestialObject* newObj = gameState->createCelestialObject(line);
            gameState->addCelestialObject(newObj);
        }
        else if (line.find("CreatedTime=") != std::string::npos){
            gameState->setDateCreated(line.substr(12));}
        else if(line.find("SimulationTime=") != std::string::npos){
            gameState->setSimulationTimeStampString(line.substr(15));
            gameState->setsimulationTimeStamp(gameState->stringToSimulationTimeStamp(line.substr(15)));}
        else if(line.find("GameStateID=") != std::string::npos){
            gameState->setStateNameId(line.substr(12));
        }
    //Print GameStateID, simulationTime, dateCreated and number of objects
    }
}

GameState* SaveTool::readState(int gameStateIndex) {
    int highestIndex = checkExistingGameState();
    if (gameStateIndex > highestIndex || gameStateIndex < 1) {
        if (highestIndex == 0) {
            std::cerr << "No game state available to load." << std::endl;
            return nullptr;
        }
        gameStateIndex = highestIndex;
    }
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return nullptr;
    }

    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Decrypt the content
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    std::string targetMarker = "[GameState" + std::to_string(gameStateIndex) + "]";
    std::string endMarker = "[EndGameState]";
    size_t start = decryptedContent.find(targetMarker);
    size_t end = decryptedContent.find(endMarker, start);

    if (start == std::string::npos || end == std::string::npos) {
        std::cerr << "Game state " << gameStateIndex << " not found." << std::endl;
        return nullptr;
    }
    //Check integrity
    std::string gameStateData = decryptedContent.substr(start, end - start + endMarker.length());
    bool isValidGameState = checkGameStateIntegrity(gameStateData);
    if (!isValidGameState) {
        std::cerr << "Game state " << gameStateIndex << " is invalid." << std::endl;
        return nullptr;
    }
    std::istringstream stream(gameStateData);
    std::string line;
    GameState* gameState = new GameState();
    int numberOfObjects = 0;
    while (std::getline(stream, line)) {
        //std::cout<<line<<std::endl;
        //If name=Sun
        if (line.find("Name=") != std::string::npos){
            numberOfObjects++;
            gameState->addCelestialObjectsData(gameState->getCelestialObjectDataFromString(line));
        }
        else if (line.find("CreatedTime=") != std::string::npos){
            gameState->setDateCreated(line.substr(12));}
        else if(line.find("SimulationTime=") != std::string::npos){
            gameState->setSimulationTimeStampString(line.substr(15));
            gameState->setsimulationTimeStamp(gameState->stringToSimulationTimeStamp(line.substr(15)));}
        else if(line.find("GameStateID=") != std::string::npos){
            gameState->setStateNameId(line.substr(12));
        }
    //Print GameStateID, simulationTime, dateCreated and number of objects
    }
    printGameStatesIds();
    gameState->setGameStateId(gameStateIndex);
    return gameState;
}

std::vector<GameState*> SaveTool::readAllStates() {
    std::vector<GameState*> gameStates;
    std::vector<int> gameStatesIds = getGameStatesIdsInt(); // Récupère les IDs des états de jeu.
    for (int i = 0; i < gameStatesIds.size(); i++) {
        // Appelle readState pour chaque ID et ajoute le pointeur retourné au vecteur.
        GameState* gameState = readState(gameStatesIds[i]);
        if (gameState != nullptr) {
            gameStates.push_back(gameState);
        }
    }
    return gameStates;
}

std::vector<int> SaveTool::getGameStatesIdsInt() {
    //Parcourir tout le fichier et retourner les indices des gameStates
    std::vector<int> gameStatesIds;
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return gameStatesIds;
    }

    // Read the encrypted data from the file
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Decrypt the content
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    std::string searchPattern = "[GameState";
    size_t startPos = 0;

    // Search for game state IDs in the decrypted content
    while ((startPos = decryptedContent.find(searchPattern, startPos)) != std::string::npos) {
        startPos += searchPattern.length(); // Move past the "[GameState" part
        size_t endPos = decryptedContent.find("]", startPos);
        if (endPos != std::string::npos) {
            std::string id = decryptedContent.substr(startPos, endPos - startPos);
            gameStatesIds.push_back(std::stoi(id));
            startPos = endPos + 1; // Move past the end of the current line for the next search
        }
    }

    return gameStatesIds;
}

void SaveTool::printGameStatesIds() {
    std::vector<int> gameStatesIds = getGameStatesIdsInt();
    for (int i = 0; i < gameStatesIds.size(); i++) {
    }
}

//Create a function that returns a vector of string with all the game states "GameStateID"
std::vector<std::string> SaveTool::getGameStatesIds() {
    std::vector<std::string> gameStatesIds;

    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return gameStatesIds;
    }

    // Read the encrypted data from the file
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Decrypt the content
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    std::string searchPattern = "GameStateID=";
    size_t startPos = 0;

    // Search for game state IDs in the decrypted content
    while ((startPos = decryptedContent.find(searchPattern, startPos)) != std::string::npos) {
        startPos += searchPattern.length(); // Move past the "GameStateID=" part
        size_t endPos = decryptedContent.find("\n", startPos);
        if (endPos != std::string::npos) {
            std::string id = decryptedContent.substr(startPos, endPos - startPos);
            gameStatesIds.push_back(id);
            startPos = endPos + 1; // Move past the end of the current line for the next search
        }
    }


    return gameStatesIds;
}

void SaveTool::removeGameState(int gameStateIndex) {
    std::vector<int> gameStatesIds = getGameStatesIdsInt();
    if (gameStateIndex > gameStatesIds.size() || gameStateIndex < 1) {
        std::cerr << "No game state with index " << gameStateIndex << " to remove." << std::endl;
        return;
    }
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }

    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Decrypt the content
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    std::string targetMarker = "[GameState" + std::to_string(gameStateIndex) + "]";
    std::string endMarker = "[EndGameState]";
    size_t start = decryptedContent.find(targetMarker);
    size_t end = decryptedContent.find(endMarker, start);

    if (start == std::string::npos || end == std::string::npos) {
        std::cerr << "Game state " << gameStateIndex << " not found." << std::endl;
        return;
    }
    std::string gameStateData = decryptedContent.substr(start, end - start + endMarker.length());
    decryptedContent.erase(start, end - start + endMarker.length());

    // Encrypt the data
    std::string encryptedData = encryptDecrypt(decryptedContent);

    // Write the encrypted data to file
    std::ofstream fileOut(saveFilepath, std::ios::binary);
    if (!fileOut.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    fileOut.write(encryptedData.c_str(), encryptedData.size());
    fileOut.close();
}




#include <regex>

bool SaveTool::checkGameStateIntegrity(std::string gameStateString) {
    std::regex gameStateStartPattern("\\[GameState\\d+\\]"); // Recognize the starts of GameState
    std::regex gameStateIDPattern("^GameStateID=.*"); // Accept any name and any number of any length
    std::regex timePattern("^(SimulationTime|CreatedTime)=.+");
    std::regex objectPattern("^Name=.+;TypeName=.+;Position=-?[0-9]+\\.?[0-9]*,-?[0-9]+\\.?[0-9]*,-?[0-9]+\\.?[0-9]*;Velocity=-?[0-9]+\\.?[0-9]*,-?[0-9]+\\.?[0-9]*,-?[0-9]+\\.?[0-9]*;Mass=[0-9]+\\.?[0-9]*;Radius=[0-9]+\\.?[0-9]*$");
    std::regex endGameStatePattern("\\[EndGameState\\]"); // Added to recognize the ends of GameState

    std::istringstream stream(gameStateString);
    std::string line;
    int numberOfObjects = 0;
    bool foundEndGameState = false; // Flag pour vérifier si [EndGameState] a été trouvé

    while (std::getline(stream, line)) {
        if (std::regex_match(line, gameStateStartPattern) || 
            std::regex_match(line, gameStateIDPattern) ||
            std::regex_match(line, endGameStatePattern)) {
            if (std::regex_match(line, endGameStatePattern)) {
                foundEndGameState = true; // Marquer la fin de GameState comme trouvée
            }
            continue; // Les lignes correspondant à un début ou à une fin de GameState, ou à un identifiant de GameState sont valides
        } else if (std::regex_match(line, timePattern)) {
            continue; // La ligne est valide pour un timestamp
        } else if (std::regex_match(line, objectPattern)) {
            numberOfObjects++; // Incrémenter le compteur d'objets valides
        } else {
            std::cerr << "Invalid line in game state data: " << line << std::endl;
            return false;
        }
    }

    return true;
}
