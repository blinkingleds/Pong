#ifndef GAMESTATES_H
#define GAMESTATES_H

#include <SDL3/SDL.h>

class Game;

enum class GameStateType
{
    MainMenu,
    Gameplay,
    Pause,
    GameOver
};

class GameState
{
public:
    virtual ~GameState() {}
    virtual void Init(SDL_Renderer* renderer) = 0;
    virtual void HandleEvents(const bool* keyStates) = 0;
    virtual void Update(float elapsedTime) = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
    virtual void Cleanup() = 0;

    void SetGame(Game* game) { this->game = game; };

protected:
    Game* game;
};

#endif // GAMESTATES_H