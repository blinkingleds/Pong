#ifndef GAME_H
#define GAME_H

#include "gamestates.h"
#include "mainmenustate.h"
#include "gameplaystate.h"

class Game
{
public:
    Game(SDL_Renderer* renderer);
    ~Game();
    void Run();
	void QueueStateChange(GameStateType newState);
	void SetExit(bool exit) { this->exit = exit; }
	bool GetExit() const { return exit; }

private:
    void ChangeState(GameStateType newState);

    SDL_Renderer* renderer;
    GameState* currentState;
	GameStateType pendingState;
    bool stateChangeQueued;
    MainMenuState mainMenuState;
	GamePlayState gamePlayState;
	bool exit;

};

#endif // GAME_H