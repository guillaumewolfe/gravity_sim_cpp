#ifndef GAME_H
#define GAME_H

class Game{
public:
    enum GameState{
        LOADING,
        MENU,
        PLAYING,
        END
    };
    Game();
    ~Game();
    void getState();

    void Init();
    void Update();
    void Draw();
    void ChangeState(GameState newState);
private:
    GameState currentState;

};
#endif