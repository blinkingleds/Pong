#include "Pong_Game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <chrono>
#include <filesystem>
#include <iostream>
#include "common.h"
#include "parameters.h"
#include "sound.h"
#include "gamerendering.h"
#include "game.h"
#include "playeractions.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

using namespace std;

int main(int argc, char *argv[])
{
	const int UPDATE_FREQUENCY{ 200 };
    const float CYCLE_TIME{ 1.0f / UPDATE_FREQUENCY };
    static Timer system_timer;
    float accumulated_seconds{ 0.0f };
	static int total_samples_generated = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool close = false;
	const bool *keyStates;
	SDL_Event e;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

	if (TTF_Init() < 0) {
		cout << "Error initializing SDL_ttf: " << SDL_GetError() << endl;
	}

	// Create fullscreen window with 1920x1080 resolution
	window = SDL_CreateWindow("Pong", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MAXIMIZED);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		return SDL_APP_FAILURE;
	}


	// Set logical size to maintain 1920x1080 coordinates regardless of actual window size
	SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	Game game(renderer);

	keyStates = SDL_GetKeyboardState(NULL);

	while(!close)
	{
		// Update clock
        system_timer.tick();
        accumulated_seconds += system_timer.elapsed_seconds;

		if (accumulated_seconds > CYCLE_TIME)
		{
			// Reset the accumulator
			accumulated_seconds = -CYCLE_TIME;

			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x0, 0xFF);
			SDL_RenderClear(renderer);
		    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

			SDL_PumpEvents();

			game.Run();

			SDL_PollEvent(&e);

			if (game.GetExit() || keyStates[SDL_SCANCODE_ESCAPE] || (e.type == SDL_EVENT_QUIT))
			{
				close = true;
			}
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}