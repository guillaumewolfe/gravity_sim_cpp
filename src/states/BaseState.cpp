#include "states/BaseState.h"
#include "game/game.h"

BaseState::BaseState(Game* gameObjParam) : gameObj(gameObjParam) {
    window = gameObj->getWindow();
}