

#ifndef saveTool_H
#define saveTool_H
#include <iostream>
#include <filesystem>
#include <vector>   
#include "save/gameState.h"
namespace fs = std::filesystem;


class GameSettings;
class RenderContext;
struct CelestialObjectData;

class SaveTool {

public:
    SaveTool();
    ~SaveTool();

    //Settings
    void loadSettings(GameSettings* settings);
    void saveGameSettings(const GameSettings& settings);
    bool checkIfSettingsAlreadyWritten(bool removeExistingSettings = false);
    int checkExistingGameState();

    //GameState
    void saveGameState(RenderContext* Context, std::string gameStateId);
    void loadGameState(RenderContext* Context, GameState* gameState = nullptr);
    void removeGameState(int gameStateIndex);
    bool checkGameStateIntegrity(std::string gameStateString);

    std::vector<std::string> getGameStatesIds();
    GameState* readState(int gameStateIndex);
    std::vector<GameState*> readAllStates();
    std::vector<int> getGameStatesIdsInt();
    void printGameStatesIds();



private:

    std::string encryptDecrypt(const std::string& data);
    std::string getSavePath();
    std::string saveFilepath;



};

#endif
