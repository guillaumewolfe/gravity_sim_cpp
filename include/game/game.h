#include <states/BaseState.h>

#ifndef GAME_H
#define GAME_H

class Game {
private:
    BaseState* currentState; 

public:
    Game();
    ~Game();
    void getState();

    void Init();
    void Update();
    void Draw();
    void ChangeState(BaseState* newState);
};

#endif
