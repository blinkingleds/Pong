
#include "Game.h"

Game::Game(SDL_Renderer* renderer) : renderer(renderer), currentState(&mainMenuState), exit(false)
{
	currentState->SetGame(this);
	currentState->Init(renderer);
    // Initialize other states
}

Game::~Game()
{
    // Cleanup
}

void Game::Run()
{
    bool running = true;
    float elapsedTime = 1.0f/200;


	const bool* keyStates = SDL_GetKeyboardState(NULL);

	SDL_PumpEvents();

	if (keyStates[SDL_SCANCODE_ESCAPE])
	{
		running = false;
	}

	currentState->HandleEvents(keyStates);
	currentState->Update(elapsedTime);
	currentState->Render(renderer);
	SDL_RenderPresent(renderer);

	// Apply pending state change if queued
	if (stateChangeQueued)
	{
		currentState->Cleanup();
		ChangeState(pendingState);
		stateChangeQueued = false;
	}
    
}

void Game::QueueStateChange(GameStateType newState)
{
    pendingState = newState;
    stateChangeQueued = true;
}

void Game::ChangeState(GameStateType newState)
{
    switch (newState)
    {
    case GameStateType::MainMenu:
        currentState = &mainMenuState;
        break;
    case GameStateType::Gameplay:
		currentState = &gamePlayState;
		break;
    default:
        break;
    }
	currentState->SetGame(this);
    currentState->Init(renderer);
}