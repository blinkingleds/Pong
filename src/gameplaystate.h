#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include "gamestates.h"
#include "ball.h"
#include "paddle.h"
#include "sound.h"
#include "playeractions.h"
#include <chrono>
#include <SDL3_ttf/SDL_ttf.h>
#include "parameters.h"

class GamePlayState : public GameState
{
public:
    GamePlayState();  
    void Init(SDL_Renderer* renderer) override;
    void HandleEvents(const bool* keyStates) override;
    void Update(float elapsedTime) override;
    void Render(SDL_Renderer* renderer) override;
    void Cleanup() override;
private:
    Ball ball;
	Paddle paddleLeft;
	Paddle paddleRight;
	SDL_Renderer* sdlRenderer;
	PlaySound playSound;
	playerScores scores;
	bool reset;
	std::chrono::steady_clock::time_point reseStartTimePoint;
	Sound sounds[6];
    PlayerInput playerPaddleInputs;
    TTF_Font*  scoreFont;
    bool gameover;
    int gameoveranimationcounter;
};

#endif // GAMEPLAYSTATE_H