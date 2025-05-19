#include "mainmenustate.h"
#include "game.h"


void MainMenuState::Init(SDL_Renderer* renderer)
{
    m_renderer = renderer; // Assign the renderer
    MouseButtonStatus = 0;       // Initialize current status
    const char* basePath = SDL_GetBasePath();
    std::string assetsPath = std::string(basePath) + "../assets/";
    
	SDL_Surface* bgndSurf = SDL_LoadBMP((assetsPath + "StartScreenAssets/Background.bmp").c_str());
    if (!bgndSurf) {
        std::cout << "Failed to load Background.bmp: " << SDL_GetError() << std::endl;
    }
	SDL_Surface* pongLogoSurf = IMG_Load((assetsPath + "StartScreenAssets/PongLogo.png").c_str());
    if (!pongLogoSurf) {
        std::cout << "Failed to load PongLogo.png: " << SDL_GetError() << std::endl;
    }
	SDL_Surface* StartTextDefaultSurf = IMG_Load((assetsPath + "StartScreenAssets/StartTextDefault.png").c_str());
	SDL_Surface* StartTextOnClickSurf = IMG_Load((assetsPath + "StartScreenAssets/StartTextOnClick.png").c_str());
	SDL_Surface* QuitTextDefaultSurf = IMG_Load((assetsPath + "StartScreenAssets/QuitTextDefault.png").c_str());
	SDL_Surface* QuitTextOnClickSurf = IMG_Load((assetsPath + "StartScreenAssets/QuitTextOnClick.png").c_str());
	SDL_Surface* ControlTextSurf = IMG_Load((assetsPath + "StartScreenAssets/ControlsText.png").c_str());


	bgndTex = SDL_CreateTextureFromSurface(renderer, bgndSurf);
	pongLogoTex = SDL_CreateTextureFromSurface(renderer, pongLogoSurf);
	StartTextDefaultTex = SDL_CreateTextureFromSurface(renderer, StartTextDefaultSurf);
	StartTextOnClickTex = SDL_CreateTextureFromSurface(renderer, StartTextOnClickSurf);
	QuitTextDefaultTex = SDL_CreateTextureFromSurface(renderer, QuitTextDefaultSurf);
	QuitTextOnClickTex = SDL_CreateTextureFromSurface(renderer, QuitTextOnClickSurf);
	ControlsTextTex = SDL_CreateTextureFromSurface(renderer, ControlTextSurf);	

    SDL_DestroySurface(bgndSurf);
	SDL_DestroySurface(pongLogoSurf);
	SDL_DestroySurface(StartTextDefaultSurf);
	SDL_DestroySurface(StartTextOnClickSurf);
	SDL_DestroySurface(QuitTextDefaultSurf);
	SDL_DestroySurface(QuitTextOnClickSurf);
	SDL_DestroySurface(ControlTextSurf);

    SDL_AudioSpec spec;
    SDL_LoadWAV((assetsPath + "sounds/MainMenuBackGroundMusic.wav").c_str(), &spec, &sounds[0].wav_data, &sounds[0].wav_data_len);
    sounds[0].stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(sounds[0].stream);
}

void MainMenuState::HandleEvents(const bool* keyStates)
{
    float tempMouseX_window, tempMouseY_window;
    MouseButtonStatus = SDL_GetMouseState(&tempMouseX_window, &tempMouseY_window); 

    if (m_renderer) { 
        SDL_RenderCoordinatesFromWindow(m_renderer, tempMouseX_window, tempMouseY_window, &mousePos.x, &mousePos.y);
    } else {
        mousePos.x = tempMouseX_window;
        mousePos.y = tempMouseY_window;
    }
}

void MainMenuState::Update(float elapsedTime)
{
    if (SDL_GetAudioStreamAvailable(sounds[0].stream) < 20) 
    {
        SDL_PutAudioStreamData(sounds[0].stream, sounds[0].wav_data, sounds[0].wav_data_len);
    }
}

void MainMenuState::Render(SDL_Renderer* renderer)
{
    SDL_SetTextureAlphaMod(pongLogoTex, 200);
    SDL_RenderTexture(renderer, bgndTex, NULL, NULL);

    // Logo positioning - centered, at 10% from top
    const float logoWidth = 646.0f * (static_cast<float>(WINDOW_WIDTH) / 1920.0f);  // Scale based on screen width
    const float logoHeight = 311.0f * (static_cast<float>(WINDOW_HEIGHT) / 1080.0f); // Scale based on screen height
    SDL_FRect rect = {
        (WINDOW_WIDTH - logoWidth) / 2.0f,  // Center horizontally
        WINDOW_HEIGHT * 0.1f,               // 10% from top
        logoWidth,
        logoHeight
    };
    SDL_RenderTexture(renderer, pongLogoTex, NULL, &rect);

    // Start button - centered, at 60% of screen height
    const float startBtnWidth = 514.0f * (static_cast<float>(WINDOW_WIDTH) / 1920.0f);
    const float startBtnHeight = 44.0f * (static_cast<float>(WINDOW_HEIGHT) / 1080.0f);
    SDL_FRect rect1 = {
        (WINDOW_WIDTH - startBtnWidth) / 2.0f,
        WINDOW_HEIGHT * 0.6f,
        startBtnWidth,
        startBtnHeight
    };

    // Update hit detection for start button
    if (mousePos.x > rect1.x && mousePos.x < rect1.x + rect1.w &&
        mousePos.y > rect1.y && mousePos.y < rect1.y + rect1.h)
    {
        SDL_RenderTexture(renderer, StartTextOnClickTex, NULL, &rect1);

        if (MouseButtonStatus == SDL_BUTTON_LEFT)
        {
            game->QueueStateChange(GameStateType::Gameplay);
        }
    }
    else
    {
        SDL_RenderTexture(renderer, StartTextDefaultTex, NULL, &rect1);
    }

    // Quit button - centered, at 70% of screen height
    const float quitBtnWidth = 268.0f * (static_cast<float>(WINDOW_WIDTH) / 1920.0f);
    const float quitBtnHeight = 54.0f * (static_cast<float>(WINDOW_HEIGHT) / 1080.0f);
    SDL_FRect rect2 = {
        (WINDOW_WIDTH - quitBtnWidth) / 2.0f,
        WINDOW_HEIGHT * 0.7f,
        quitBtnWidth,
        quitBtnHeight
    };

    // Update hit detection for quit button
    if (mousePos.x > rect2.x && mousePos.x < rect2.x + rect2.w &&
        mousePos.y > rect2.y && mousePos.y < rect2.y + rect2.h)
    {
        SDL_RenderTexture(renderer, QuitTextOnClickTex, NULL, &rect2);

        // --- TEMPORARY DIAGNOSTIC ---
        // Check if the left button is physically down according to the most current raw state
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) { 
            std::cout << "Quit Hover + Click Attempt: MenuState.MouseButtonStatus = " << MouseButtonStatus 
                      << ", Raw State Now = " << SDL_GetMouseState(NULL,NULL) << std::endl;
        }
        // --- END DIAGNOSTIC ---

        if (MouseButtonStatus == SDL_BUTTON_LEFT)
        {
            game->SetExit(true);
        }
    }
    else
    {
        SDL_RenderTexture(renderer, QuitTextDefaultTex, NULL, &rect2);
    }

    // Controls text - centered, at 90% of screen height
    const float controlsWidth = 578.0f * (static_cast<float>(WINDOW_WIDTH) / 1920.0f);
    const float controlsHeight = 25.0f * (static_cast<float>(WINDOW_HEIGHT) / 1080.0f);
    SDL_FRect rect3 = {
        (WINDOW_WIDTH - controlsWidth) / 2.0f,
        WINDOW_HEIGHT * 0.9f,
        controlsWidth,
        controlsHeight
    };
    SDL_RenderTexture(renderer, ControlsTextTex, NULL, &rect3);
}


void MainMenuState::Cleanup()
{
    // Stop and free the background music
    SDL_free(sounds[0].wav_data);
    SDL_DestroyAudioStream(sounds[0].stream);

    // Free textures
    SDL_DestroyTexture(bgndTex);
    SDL_DestroyTexture(pongLogoTex);
    SDL_DestroyTexture(StartTextDefaultTex);
    SDL_DestroyTexture(StartTextOnClickTex);
    SDL_DestroyTexture(QuitTextDefaultTex);
    SDL_DestroyTexture(QuitTextOnClickTex);
    SDL_DestroyTexture(ControlsTextTex);
}