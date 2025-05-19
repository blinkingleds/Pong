#include <SDL3/SDL.h>
#include "ball.h"
#include "paddle.h"
#include <SDL3_ttf/SDL_ttf.h>

class GameRendering
{
private:

public:
	GameRendering();
	static void Render(SDL_Renderer* renderer, Ball &ball,Paddle &paddleRight, Paddle &paddleLeft, const playerScores &scores, TTF_Font *scoreFont);
};