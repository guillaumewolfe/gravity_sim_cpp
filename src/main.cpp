#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/EngineIncludes.h"
#include "game/game.h"




using namespace std; 

int main()
{
    Game myGame;
    myGame.Init();
    myGame.getState();
    myGame.ChangeState(Game::MENU);
    myGame.getState();

    return 0;
}