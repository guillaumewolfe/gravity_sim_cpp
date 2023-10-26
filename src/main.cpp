#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/EngineIncludes.h"
#include "states/statesIncludes.h"
#include <game/game.h>

using namespace std; 



int main()
{
    Game myGame;

    myGame.ChangeState(new LoadingState());
    myGame.getState();
    myGame.ChangeState(new MenuState());
    myGame.getState();
    return 0;
}