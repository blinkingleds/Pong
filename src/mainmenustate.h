#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "gamestates.h"
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include "parameters.h"
#include "common.h"
#include "sound.h"

class MainMenuState : public GameState
{
public:
    void Init(SDL_Renderer* renderer) override;
    void HandleEvents(const bool* keyStates) override;
    void Update(float elapsedTime) override;
    void Render(SDL_Renderer* renderer) override;
    void Cleanup() override;
private:
    SDL_Renderer* m_renderer;
    SDL_Texture* bgndTex;
    SDL_Texture* pongLogoTex;
    SDL_Texture* StartTextDefaultTex;
    SDL_Texture* StartTextOnClickTex;
    SDL_Texture* QuitTextDefaultTex;
    SDL_Texture* QuitTextOnClickTex;
    SDL_Texture* ControlsTextTex;
    Vec2D mousePos;
	Uint32 MouseButtonStatus;

    Sound sounds[3];
};

#endif // MAINMENUSTATE_H