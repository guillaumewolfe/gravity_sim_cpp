
#include "save/saveTool.h"
#include <fstream>
#include <string>
#include <vector>
#include "path_util.h"
#include "game/game.h"
#include "engine/CelestialObject.h"
#include "engine/RenderTools/RenderContext.h"
#include <regex>
#include <winsock2.h>
#include <windows.h>
#include "engine/RenderTools/SuccessTool.h"


SaveTool::SaveTool(){
    saveFilepath = getSavePath();
}

SaveTool::~SaveTool(){}

std::string SaveTool::getSavePath() {

    char* appDataPath = std::getenv("APPDATA");
    if (appDataPath == nullptr) {
        std::cerr << "Impossible de trouver le dossier AppData.\n";
    }

    //folder path
    std::filesystem::path spaceQueryPath = std::filesystem::path(appDataPath) / "SpaceQuery";
    //File path
    std::filesystem::path datFilePath = spaceQueryPath / "dat.dat";

    if (!std::filesystem::exists(spaceQueryPath)) {
        std::filesystem::create_directories(spaceQueryPath);
    }
    // Vérifier si le fichier dat.dat existe, le créer si nécessaire
    if (!std::filesystem::exists(datFilePath)) {
        std::ofstream file(datFilePath); // Créer le fichier
        if (file.is_open()) {
            file.close();
        } else {
            std::cerr << "Impossible de créer le fichier " << datFilePath << ".\n";
        }
    } 

    // Convertir le chemin en chaîne de caractères
    std::string path = datFilePath.string();
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




void SaveTool::checkGameSuccessTag(){

    //Check if the tags  [GameSuccess] and [EndGameSuccess] exist, if not create them

    std::ifstream file(saveFilepath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for reading." << std::endl;
            return;
        }

        std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Decrypt the content
        std::string decryptedContent = encryptDecrypt(encryptedContent);
        size_t start = decryptedContent.find("[GameSuccess]");
        size_t end = decryptedContent.find("[EndGameSuccess]");

        bool tagExist = start != std::string::npos && end != std::string::npos;

        if (!tagExist) {
            std::string successSection = decryptedContent + "[GameSuccess]\n[EndGameSuccess]\n";
            std::string encryptedData = encryptDecrypt(successSection);
            std::ofstream fileOut(saveFilepath, std::ios::binary);
            if (!fileOut.is_open()) {
                std::cerr << "Failed to open file for writing." << std::endl;
                return;
            }
            fileOut.write(encryptedData.c_str(), encryptedData.size());
            fileOut.close();
        }
}

void SaveTool::checkSuccessList(std::vector<Success*> successList){
    checkGameSuccessTag(); // Assurez-vous que cette fonction crée correctement les tags si absents
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    // Mapping des IDs de succès à leur progression dans le fichier
    std::map<std::string, std::string> fileSuccessProgress;

    size_t start = decryptedContent.find("[GameSuccess]");
    size_t end = decryptedContent.find("[EndGameSuccess]");

    if (start != std::string::npos && end != std::string::npos) {
        std::string successSection = decryptedContent.substr(start + 13, end - start - 13);
        std::istringstream ss(successSection);
        std::string line;
        while (std::getline(ss, line)) {
            if (line.find("SUCCESS") != std::string::npos) {
                auto pos = line.find("=");
                if (pos != std::string::npos) {
                    std::string id = line.substr(0, pos);
                    fileSuccessProgress[id] = line.substr(pos + 1);
                }
            }
        }
    }

    std::string newSuccessSection = "[GameSuccess]\n";
    for (const auto& success : successList) {
        std::string successId = "SUCCESS" + std::to_string(success->id);
        // Utiliser la progression du fichier si disponible, sinon utiliser la progression de successList
        auto it = fileSuccessProgress.find(successId);
        if (it != fileSuccessProgress.end()) {
            newSuccessSection += successId + "=" + it->second + "\n";
        } else {
            newSuccessSection += successId + "=" + std::to_string(success->stepsDone) + "/" + std::to_string(success->totalSteps) + "\n";
        }
    }
    newSuccessSection += "[EndGameSuccess]\n";

    // Reconstruction du contenu final
    std::string before = start > 0 ? decryptedContent.substr(0, start) : "";
    std::string after = end > 0 ? decryptedContent.substr(end + strlen("[EndGameSuccess]")) : decryptedContent;
    std::string finalContent = before + newSuccessSection + after;

    // Réécriture du fichier avec le contenu modifié
    std::string encryptedData = encryptDecrypt(finalContent);
    std::ofstream fileOut(saveFilepath, std::ios::binary);
    if (!fileOut.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    fileOut.write(encryptedData.c_str(), encryptedData.size());
    fileOut.close();
    // Après avoir écrit dans le fichier, réouvrez-le pour le débogage
    std::ifstream fileDebug(saveFilepath, std::ios::binary);
    if (!fileDebug.is_open()) {
        std::cerr << "Failed to open file for reading (debug)." << std::endl;
        return;
    }

    std::string encryptedContentDebug((std::istreambuf_iterator<char>(fileDebug)), std::istreambuf_iterator<char>());
    fileDebug.close();

    // Decrypt the content for debugging
    std::string decryptedContentDebug = encryptDecrypt(encryptedContentDebug);

    // Print the whole content for debugging
    //std::cout << "File content after adding successes:" << std::endl;
    //std::cout << decryptedContentDebug << std::endl;
}

void SaveTool::SyncSuccessList(std::vector<Success*> successList) {
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    // Trouver la section des succès
    size_t start = decryptedContent.find("[GameSuccess]");
    size_t end = decryptedContent.find("[EndGameSuccess]");
    
    if (start == std::string::npos || end == std::string::npos) {
        std::cerr << "GameSuccess section not found." << std::endl;
        return;
    }

    // Extraire et parcourir la section des succès
    std::string successSection = decryptedContent.substr(start + 13, end - start - 13);
    std::istringstream ss(successSection);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.find("SUCCESS") != std::string::npos) {
            auto pos = line.find("=");
            auto slashPos = line.find("/");
            if (pos != std::string::npos && slashPos != std::string::npos) {
                int id = std::stoi(line.substr(7, pos - 7)); // Extrait l'ID après "SUCCESS"
                int stepsDone = std::stoi(line.substr(pos + 1, slashPos - pos - 1)); // Extrait stepsDone

                // Trouver et mettre à jour le succès correspondant dans la liste
                for (auto& success : successList) {
                    if (success->id == id) {
                        success->stepsDone = stepsDone;
                        if(success->stepsDone == success->totalSteps){
                            success->isDone = true;
                        }
                        break; // Sortir de la boucle une fois le succès mis à jour
                    }
                }
            }
        }
    }
}


void SaveTool::SaveSuccessList(const std::vector<Success*> successList) {
    // Lecture du contenu existant
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    // Trouver la section des succès
    size_t start = decryptedContent.find("[GameSuccess]");
    size_t end = decryptedContent.find("[EndGameSuccess]");

    // Préparation de la nouvelle section des succès
    std::string newSuccessSection = "[GameSuccess]\n";
    for (const auto& success : successList) {
        newSuccessSection += "SUCCESS" + std::to_string(success->id) + "=" + std::to_string(success->stepsDone) + "/" + std::to_string(success->totalSteps) + "\n";
    }
    newSuccessSection += "[EndGameSuccess]\n";

    std::string finalContent;
    if (start != std::string::npos && end != std::string::npos) {
        // Remplacer la vieille section par la nouvelle
        std::string before = decryptedContent.substr(0, start);
        std::string after = decryptedContent.substr(end + strlen("[EndGameSuccess]"));
        finalContent = before + newSuccessSection + after;
    } else {
        // Ajouter la nouvelle section des succès si elle n'existait pas
        finalContent = decryptedContent + newSuccessSection;
    }

    // Réécrire le fichier avec le contenu mis à jour
    std::string encryptedData = encryptDecrypt(finalContent);
    std::ofstream fileOut(saveFilepath, std::ios::binary);
    if (!fileOut.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    fileOut.write(encryptedData.c_str(), encryptedData.size());
    fileOut.close();
}

void SaveTool::saveElementsToVerify(const std::vector<std::string>* elementsToVerify, int ID) {
    if (!elementsToVerify) return;

    // Lecture du contenu existant et décryptage
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string decryptedContent = encryptDecrypt(encryptedContent);
    
    // Construction des gardes avec l'ID
    std::string startGuard = "[ElementsToVerify" + std::to_string(ID) + "]";
    std::string endGuard = "[EndElementsToVerify" + std::to_string(ID) + "]";
    
    // Recherche des positions des gardes dans le contenu
    size_t startPos = decryptedContent.find(startGuard);
    size_t endPos = decryptedContent.find(endGuard, startPos);
    
    // Construction de la nouvelle section avec les éléments à vérifier
    std::string updatedSection = startGuard + "\n";
    for (const auto& element : *elementsToVerify) {
        updatedSection += element + "\n";
    }
    updatedSection += endGuard + "\n";
    
    std::string updatedContent;
    if (startPos != std::string::npos && endPos != std::string::npos) {
        // Si les gardes existent, remplacer la section existante
        std::string before = decryptedContent.substr(0, startPos);
        std::string after = decryptedContent.substr(endPos + endGuard.length());
        updatedContent = before + updatedSection + after;
    } else {
        // Si les gardes n'existent pas, ajouter la nouvelle section à la fin
        updatedContent = decryptedContent + updatedSection;
    }

    // Chiffrer et écrire le contenu mis à jour dans le fichier
    std::string encryptedData = encryptDecrypt(updatedContent);
    std::ofstream fileOut(saveFilepath, std::ios::binary);
    if (!fileOut.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    fileOut.write(encryptedData.c_str(), encryptedData.size());
    fileOut.close();

    // Section de débogage pour vérifier le contenu mis à jour
    // Réouverture du fichier pour lire et afficher le contenu mis à jour pour le débogage
    std::ifstream fileDebug(saveFilepath, std::ios::binary);
    if (!fileDebug.is_open()) {
        std::cerr << "Failed to open file for reading (debug)." << std::endl;
        return;
    }
    std::string encryptedContentDebug((std::istreambuf_iterator<char>(fileDebug)), std::istreambuf_iterator<char>());
    fileDebug.close();
    std::string decryptedContentDebug = encryptDecrypt(encryptedContentDebug);
    //std::cout << "Updated file content for debug:" << std::endl;
    //std::cout << decryptedContentDebug << std::endl;
}


void SaveTool::loadElementsToVerify(std::vector<std::string>& elementsToVerify, int ID) {
    // Ouvrir le fichier et lire son contenu
    std::ifstream file(saveFilepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return; // Terminer si le fichier ne peut pas être ouvert
    }
    std::string encryptedContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Décrypter le contenu du fichier
    std::string decryptedContent = encryptDecrypt(encryptedContent);

    // Construire les noms des gardes basés sur l'ID
    std::string startGuard = "[ElementsToVerify" + std::to_string(ID) + "]";
    std::string endGuard = "[EndElementsToVerify" + std::to_string(ID) + "]";

    // Trouver les positions des gardes dans le contenu décrypté
    size_t startPos = decryptedContent.find(startGuard);
    size_t endPos = decryptedContent.find(endGuard, startPos);

    // Vérifier si les gardes ont été trouvés
    if (startPos == std::string::npos || endPos == std::string::npos) {
        return; // Terminer la fonction si les gardes ne sont pas trouvés
    }

    // Extraire la section entre les gardes
    std::string elementsSection = decryptedContent.substr(startPos + startGuard.length(), endPos - (startPos + startGuard.length()));

    // Nettoyer le vecteur existant
    elementsToVerify.clear();

    // Parcourir la section extraite et peupler le vecteur
    std::istringstream iss(elementsSection);
    std::string line;
    while (std::getline(iss, line)) {
        // Ignorer les lignes vides ou les lignes qui ne contiennent que des espaces
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (!line.empty()) {
            elementsToVerify.push_back(line);
        }
    }


}