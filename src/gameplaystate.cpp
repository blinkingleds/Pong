#include "gameplaystate.h"
#include <iostream>
#include <cmath>
#include "game.h"

GamePlayState::GamePlayState():
    ball({ WINDOW_WIDTH / 2 - BALL_WIDTH/2, WINDOW_HEIGHT / 2 - BALL_WIDTH/2}, BALL_WIDTH),
    paddleLeft({ COURT_OFFSET + PADDLE_WIDTH, (WINDOW_HEIGHT / 2) - PADDLE_HEIGHT / 2 },  PADDLE_HEIGHT,  PADDLE_WIDTH),
    paddleRight( { WINDOW_WIDTH - COURT_OFFSET - (2 * PADDLE_WIDTH), (WINDOW_HEIGHT / 2) - PADDLE_HEIGHT/2}, PADDLE_HEIGHT, PADDLE_WIDTH)
{
    SDL_AudioSpec spec;
    const char* basePath = SDL_GetBasePath();
    std::string assetsPath = std::string(basePath) + "../assets/";
    
    scores.leftPlayerScore = 0;
    scores.rightPlayerScore = 0;
    reset = false;


    SDL_LoadWAV((assetsPath + "sounds/RightPaddle.wav").c_str(), &spec, &sounds[0].wav_data, &sounds[0].wav_data_len); // Right Paddle hit sound 
    sounds[0].stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    SDL_LoadWAV((assetsPath + "sounds/LeftPaddle.wav").c_str(), &spec, &sounds[1].wav_data, &sounds[1].wav_data_len);  // Left Paddle hit sound
    sounds[1].stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    SDL_LoadWAV((assetsPath + "sounds/Background.wav").c_str(), &spec, &sounds[2].wav_data, &sounds[2].wav_data_len); // Background Music
    sounds[2].stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    SDL_LoadWAV((assetsPath + "sounds/Wall.wav").c_str(), &spec, &sounds[3].wav_data, &sounds[3].wav_data_len);  // Ball past paddle sound
    sounds[3].stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    SDL_LoadWAV((assetsPath + "sounds/Wall.wav").c_str(), &spec, &sounds[4].wav_data, &sounds[4].wav_data_len);   // Wall hit sound
    sounds[4].stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    SDL_LoadWAV((assetsPath + "sounds/GameOver.wav").c_str(), &spec, &sounds[5].wav_data, &sounds[5].wav_data_len);   // Wall hit sound
    sounds[5].stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);

    SDL_ResumeAudioStreamDevice(sounds[0].stream);
    SDL_ResumeAudioStreamDevice(sounds[1].stream);
    SDL_ResumeAudioStreamDevice(sounds[2].stream);
    SDL_ResumeAudioStreamDevice(sounds[3].stream);
    SDL_ResumeAudioStreamDevice(sounds[4].stream);
    SDL_ResumeAudioStreamDevice(sounds[5].stream);

    scoreFont = TTF_OpenFont((assetsPath + "fonts/font.ttf").c_str(), 24);
    if ( !scoreFont ) {
        std::cout << "Failed to load font: " << SDL_GetError() << std::endl;
    }

}

void GamePlayState:: Init(SDL_Renderer* renderer)
{
    sdlRenderer = renderer;
    gameover = false;
    gameoveranimationcounter = 0;
    scores.leftPlayerScore = 0;
    scores.rightPlayerScore = 0;
    reset = true;
    playSound.backgroundSound = true;
    playSound.gameover = false;
    playSound.rightPaddleHit= false;
	playSound.leftPaddleHit = false;
	playSound.ballPastPaddle = false;
	playSound.wallHit = false;


}


void GamePlayState::HandleEvents(const bool* keyStates)
{
    if (keyStates[SDL_SCANCODE_UP])
    {
        playerPaddleInputs.playerRight = PA_MOVEUP;
    }
    else if (keyStates[SDL_SCANCODE_DOWN])
    {
        playerPaddleInputs.playerRight = PA_MOVEDOWN;
    }
    else
    {
        playerPaddleInputs.playerRight = PA_NONE;
    }

    if (keyStates[SDL_SCANCODE_W])
    {
        playerPaddleInputs.playerLeft = PA_MOVEUP;
    }
    else if (keyStates[SDL_SCANCODE_S])
    {
        playerPaddleInputs.playerLeft = PA_MOVEDOWN;
    }
    else
    {
        playerPaddleInputs.playerLeft = PA_NONE;
    }
    // Handle other events
}

void GamePlayState::Update(float elapsedTime)
{
        
		Vec2D rightPadCurrPos = paddleRight.GetPosition();
		Vec2D rightPadCurrVel = paddleRight.GetVelocity();
		Vec2D leftPadCurrPos = paddleLeft.GetPosition();
		Vec2D leftPadCurrVel = paddleLeft.GetVelocity();
		Vec2D ballCurrPos = ball.GetPosition();
		Vec2D ballCurrVel = ball.GetVelocity();


		Vec2D balltargetpos = ballCurrPos;
		Vec2D balltargetvel = ballCurrVel;


        playSound.rightPaddleHit= false;
	    playSound.leftPaddleHit = false;
	    playSound.ballPastPaddle = false;
	    playSound.wallHit = false;
		if(ballCurrPos.x < (WINDOW_WIDTH) && ballCurrVel.x < 0)
		{
			while (balltargetpos.x > leftPadCurrPos.x)
			{
				float timetowall;

				if (balltargetvel.y < 0 )
				{
					timetowall = (COURT_OFFSET - balltargetpos.y) / balltargetvel.y;
				}
				else
				{
					timetowall = (WINDOW_HEIGHT - COURT_OFFSET - balltargetpos.y) / balltargetvel.y;
				}

				float timetopaddle = (leftPadCurrPos.x - balltargetpos.x) / balltargetvel.x;

				if(timetowall < timetopaddle)
				{
					balltargetpos.x += balltargetvel.x * timetowall;
					balltargetpos.y += balltargetvel.y * timetowall;

					balltargetvel.y = -balltargetvel.y;
				}
				else
				{
					balltargetpos.y += balltargetvel.y * timetopaddle;
					break;
				}
			}

			float targetPosY = balltargetpos.y - PADDLE_HEIGHT / 2;
			float distance = targetPosY - leftPadCurrPos.y;
			float threshold = PADDLE_HEIGHT/5;

			if (std::abs(distance) > threshold)
			{
				if (distance > 0)
				{
					playerPaddleInputs.playerLeft = PA_MOVEDOWN;
				}
				else
				{
					playerPaddleInputs.playerLeft = PA_MOVEUP;
				}
			}
			else
			{
				playerPaddleInputs.playerLeft = PA_NONE;
			}
		}
		else
		{
			float targetPosY = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
			float distance = targetPosY - leftPadCurrPos.y;
			float threshold = PADDLE_HEIGHT / 4;

			if (std::abs(distance) > threshold)
			{
				if (distance > 0)
				{
					playerPaddleInputs.playerLeft = PA_MOVEDOWN;
				}
				else
				{
					playerPaddleInputs.playerLeft = PA_MOVEUP;
				}
			}
			else
			{
				playerPaddleInputs.playerLeft = PA_NONE;
			}
		}
		

		if (playerPaddleInputs.playerRight == PA_MOVEUP) // Move up
		{
			rightPadCurrVel.y -= (rightPadCurrVel.y > 0 ? PADDLE_DECEL : PADDLE_ACCEL) * (elapsedTime);
			rightPadCurrVel.y = std::max(rightPadCurrVel.y, -PADDLE_TERMVELO);
		}
		else if (playerPaddleInputs.playerRight == PA_MOVEDOWN) // Move down
		{
			rightPadCurrVel.y += (rightPadCurrVel.y > 0 ? PADDLE_ACCEL : PADDLE_DECEL) * (elapsedTime);
			rightPadCurrVel.y = std::min(rightPadCurrVel.y, PADDLE_TERMVELO);
		}
		else // Decelerate
		{
			if (rightPadCurrVel.y > 0)
			{
				rightPadCurrVel.y = std::max(rightPadCurrVel.y - PADDLE_DECEL * (elapsedTime), 0.0f);
			}
			else if (rightPadCurrVel.y < 0)
			{
				rightPadCurrVel.y = std::min(rightPadCurrVel.y + PADDLE_DECEL * (elapsedTime), 0.0f);
			}
		}

		rightPadCurrPos.y += rightPadCurrVel.y * (elapsedTime);


		// limit the paddle position to the court boundaries

		if (rightPadCurrPos.y < COURT_OFFSET)
		{
			rightPadCurrPos.y = COURT_OFFSET;
			rightPadCurrVel.y = 0;
		}
		else if (rightPadCurrPos.y > (WINDOW_HEIGHT - COURT_OFFSET - PADDLE_HEIGHT))
		{
			rightPadCurrPos.y = WINDOW_HEIGHT - COURT_OFFSET - PADDLE_HEIGHT;
			rightPadCurrVel.y = 0;
		}


		// Do the same as above for left paddle

		if (playerPaddleInputs.playerLeft == PA_MOVEUP) // Move up
		{
			leftPadCurrVel.y -= (leftPadCurrVel.y > 0 ? PADDLE_DECEL : PADDLE_ACCEL) * (elapsedTime);
			leftPadCurrVel.y = std::max(leftPadCurrVel.y, -PADDLE_TERMVELO);
		}
		else if (playerPaddleInputs.playerLeft == PA_MOVEDOWN) // Move down
		{
			leftPadCurrVel.y += (leftPadCurrVel.y > 0 ? PADDLE_ACCEL : PADDLE_DECEL) * (elapsedTime);
			leftPadCurrVel.y = std::min(leftPadCurrVel.y, PADDLE_TERMVELO);
		}
		else // Decelerate
		{
			if (leftPadCurrVel.y > 0)
			{
				leftPadCurrVel.y = std::max(leftPadCurrVel.y - PADDLE_DECEL * (elapsedTime), 0.0f);
			}
			else if (leftPadCurrVel.y < 0)
			{
				leftPadCurrVel.y = std::min(leftPadCurrVel.y + PADDLE_DECEL * (elapsedTime), 0.0f);
			}
		}

		leftPadCurrPos.y += leftPadCurrVel.y * (elapsedTime);

		// limit the paddle position to the court boundaries
		if (leftPadCurrPos.y < COURT_OFFSET)
		{
			leftPadCurrPos.y = COURT_OFFSET;
			leftPadCurrVel.y = 0;
		}
		else if (leftPadCurrPos.y > (WINDOW_HEIGHT - COURT_OFFSET - PADDLE_HEIGHT))
		{
			leftPadCurrPos.y = WINDOW_HEIGHT - COURT_OFFSET - PADDLE_HEIGHT;
			leftPadCurrVel.y = 0;
		}

		if (reset == false && !gameover)
		{
			// Update the ball position
		
			ballCurrPos.x += ballCurrVel.x * (elapsedTime);
			ballCurrPos.y += ballCurrVel.y * (elapsedTime);
			
			if (ballCurrPos.y < COURT_OFFSET)
			{
				playSound.wallHit = true;
				ballCurrPos.y = COURT_OFFSET;
				ballCurrVel.y = -ballCurrVel.y;
			}
			else if (ballCurrPos.y > (WINDOW_HEIGHT - COURT_OFFSET - BALL_WIDTH))
			{
				playSound.wallHit = true;
				ballCurrPos.y = WINDOW_HEIGHT - COURT_OFFSET - BALL_WIDTH;
				ballCurrVel.y = -ballCurrVel.y;
			}
			
			if(ballCurrPos.x > rightPadCurrPos.x)
			{
				if ((ballCurrPos.y+BALL_WIDTH) > rightPadCurrPos.y && (ballCurrPos.y) < rightPadCurrPos.y + PADDLE_HEIGHT)
				{
					float padHitPos;
					Vec2D padnNewVel;
					
					padHitPos = (rightPadCurrPos.y + PADDLE_HEIGHT / 2) - (ballCurrPos.y + BALL_WIDTH/2);
					padHitPos = padHitPos / (PADDLE_HEIGHT / 2);
					if (padHitPos >  0.8) {padHitPos = 0.8;};
					if (padHitPos < -0.8) {padHitPos = -0.8;};
					//cout << padHitPos << endl;
					//padnNewVel.x = -(SDL_sqrt(1-(padHitPos*padHitPos)));
					padnNewVel.x = -(SDL_sqrt(1-(padHitPos*padHitPos)));
					padnNewVel.y = padHitPos;
					//cout << padnNewVel.x << " " << padnNewVel.y << endl;



					ballCurrPos.x = rightPadCurrPos.x;
					float magnitude = sqrt((ballCurrVel.x/1000)*(ballCurrVel.x/1000) + (ballCurrVel.y/1000)*(ballCurrVel.y/1000));
					magnitude = std::min(magnitude, 5.0f);
					
					ballCurrVel.x = (magnitude+0.1)*padnNewVel.x*1000; //2000*padnNewVel.x;
					ballCurrVel.y = -(magnitude+0.1)*padnNewVel.y*1000; //-2000*padnNewVel.y;
					//std::cout << ballCurrVel.x << " " << ballCurrVel.y << std::endl;
					//cout << SDL_sqrt((ballCurrVel.x*ballCurrVel.x) + (ballCurrVel.y*ballCurrVel.y)) << endl;

					playSound.rightPaddleHit = true;
				}
				else
				{
					reset = true;
					reseStartTimePoint = std::chrono::steady_clock::now();
					scores.leftPlayerScore++;
					rightPadCurrPos.y = ((WINDOW_HEIGHT / 2) - PADDLE_HEIGHT / 2 );
					leftPadCurrPos.y = ( (WINDOW_HEIGHT / 2) - PADDLE_HEIGHT/2 );
					playSound.ballPastPaddle = true;
					ballCurrPos.x = WINDOW_WIDTH / 2 - BALL_WIDTH / 2;
					ballCurrPos.y = WINDOW_HEIGHT / 2 - BALL_WIDTH / 2;
					ballCurrVel.x = BALL_INIT_VELOCITY;
					ballCurrVel.y = 0;
				}
			}
			else if (ballCurrPos.x < leftPadCurrPos.x)
			{
				if ((ballCurrPos.y+BALL_WIDTH)  > leftPadCurrPos.y && (ballCurrPos.y)  < leftPadCurrPos.y + PADDLE_HEIGHT)
				{
					float padHitPos;
					Vec2D padnNewVel;

					padHitPos = (leftPadCurrPos.y + PADDLE_HEIGHT / 2) - (ballCurrPos.y + BALL_WIDTH/2);
					padHitPos = padHitPos / (PADDLE_HEIGHT / 2);
					if (padHitPos >  0.8) {padHitPos = 0.8;};
					if (padHitPos < -0.8) {padHitPos = -0.8;};
					//cout << padHitPos << endl;
					padnNewVel.x = SDL_sqrt(1 - (padHitPos * padHitPos));
					padnNewVel.y = -padHitPos;
					//cout << padnNewVel.x << " " << padnNewVel.y << endl;

					ballCurrPos.x = leftPadCurrPos.x;


					float magnitude = sqrt((ballCurrVel.x/1000)*(ballCurrVel.x/1000) + (ballCurrVel.y/1000)*(ballCurrVel.y/1000));
					magnitude = std::min(magnitude, 5.0f);
					ballCurrVel.x = (magnitude+0.1)*padnNewVel.x*1000;//2000*padnNewVel.x;
					ballCurrVel.y = (magnitude+0.1)*padnNewVel.y*1000;//2000*padnNewVel.y;
					//std::cout << ballCurrVel.x << " " << ballCurrVel.y << std::endl;
					//cout << SDL_sqrt((ballCurrVel.x*ballCurrVel.x) + (ballCurrVel.y*ballCurrVel.y)) << endl;

					playSound.leftPaddleHit = true;
				}
				else
				{
					reset = true;
					reseStartTimePoint = std::chrono::steady_clock::now();
					scores.rightPlayerScore++;
					rightPadCurrPos.y = ((WINDOW_HEIGHT / 2) - PADDLE_HEIGHT / 2 );
					leftPadCurrPos.y = ( (WINDOW_HEIGHT / 2) - PADDLE_HEIGHT/2 );
					playSound.ballPastPaddle = true;
					ballCurrPos.x = WINDOW_WIDTH / 2 - BALL_WIDTH / 2;
					ballCurrPos.y = WINDOW_HEIGHT / 2 - BALL_WIDTH / 2;
					ballCurrVel.x = -BALL_INIT_VELOCITY;
					ballCurrVel.y = 0;
				}
			}

			ball.SetPosition(ballCurrPos);
			ball.SetVelocity(ballCurrVel);
		}
		else
		{
			if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - reseStartTimePoint).count() > 1)
			{
				reset = false;
			}
		}
		



		paddleRight.SetVelocity(rightPadCurrVel);
		paddleRight.SetPosition(rightPadCurrPos);
		paddleLeft.SetVelocity(leftPadCurrVel);
		paddleLeft.SetPosition(leftPadCurrPos);

		if(playSound.rightPaddleHit)
		{
			if (SDL_GetAudioStreamAvailable(sounds[0].stream) < 20) 
			{
				// feed the new data to the stream. It will queue at the end, and trickle out as the hardware needs more data.
				SDL_PutAudioStreamData(sounds[0].stream, sounds[0].wav_data, sounds[0].wav_data_len);
			}
		}
		if(playSound.leftPaddleHit)
		{
			if (SDL_GetAudioStreamAvailable(sounds[1].stream) < 20) 
			{
				//feed the new data to the stream. It will queue at the end, and trickle out as the hardware needs more data.
				SDL_PutAudioStreamData(sounds[1].stream, sounds[1].wav_data, sounds[1].wav_data_len);
			}
		}
		if (playSound.ballPastPaddle)
		{
			if (SDL_GetAudioStreamAvailable(sounds[3].stream) < 20) 
			{
				// feed the new data to the stream. It will queue at the end, and trickle out as the hardware needs more data. 
				SDL_PutAudioStreamData(sounds[3].stream, sounds[3].wav_data, sounds[3].wav_data_len);
			}
		}
		if (playSound.wallHit)
		{
			if (SDL_GetAudioStreamAvailable(sounds[4].stream) < 20) 
			{
				// feed the new data to the stream. It will queue at the end, and trickle out as the hardware needs more data.
				SDL_PutAudioStreamData(sounds[4].stream, sounds[4].wav_data, sounds[4].wav_data_len);
			}
		}


        if (playSound.backgroundSound)
        {
            if (SDL_GetAudioStreamAvailable(sounds[2].stream) < 200) 
            {
                // feed the new data to the stream. It will queue at the end, and trickle out as the hardware needs more data. 
                SDL_PutAudioStreamData(sounds[2].stream, sounds[2].wav_data, sounds[2].wav_data_len);
            }
        }

        if ((scores.leftPlayerScore == 5 || scores.rightPlayerScore == 5) && (gameover == false))
        {
            playSound.backgroundSound = false;
            playSound.gameover = true;
            gameover = true;
        }

        if (playSound.gameover)
        {
            SDL_ClearAudioStream(sounds[2].stream);
            if (SDL_GetAudioStreamAvailable(sounds[5].stream) < 20) 
            {
                // feed the new data to the stream. It will queue at the end, and trickle out as the hardware needs more data. 
                SDL_PutAudioStreamData(sounds[5].stream, sounds[5].wav_data, sounds[5].wav_data_len);
            }
            playSound.gameover = false;
        }

}

void GamePlayState::Render(SDL_Renderer* renderer)
{
    // Draw the court boundaries
    SDL_FRect rect{};
    rect.h = static_cast<int>(WINDOW_HEIGHT - (2* COURT_OFFSET));
    rect.w = static_cast<int>(WINDOW_WIDTH - (2* COURT_OFFSET));
    rect.x = COURT_OFFSET;
    rect.y = COURT_OFFSET;
    SDL_RenderRect(renderer, &rect);

    // Draw the middle line
    for (int i = 0; i < WINDOW_HEIGHT - 2*COURT_OFFSET; ++i)
    {
        if ((i % 5) == true)
        {
            SDL_RenderPoint(renderer, WINDOW_WIDTH/2, COURT_OFFSET+i);
        }

    }

    //Draw the ball
    rect.h = static_cast<int>(BALL_WIDTH);
    rect.w = static_cast<int>(BALL_WIDTH);
    rect.x = static_cast<int>(ball.GetPosition().x);
    rect.y = static_cast<int>(ball.GetPosition().y);
    SDL_RenderFillRect(renderer, &rect);

    //Draw right side paddle
    rect.h = static_cast<int>(PADDLE_HEIGHT);
    rect.w = static_cast<int>(PADDLE_WIDTH);
    rect.x = static_cast<int>(paddleRight.GetPosition().x);
    rect.y = static_cast<int>(paddleRight.GetPosition().y);
    SDL_RenderFillRect(renderer, &rect);

    //Draw left side paddle
    rect.h = static_cast<int>(PADDLE_HEIGHT);
    rect.w = static_cast<int>(PADDLE_WIDTH);
    rect.x = static_cast<int>(paddleLeft.GetPosition().x);
    rect.y = static_cast<int>(paddleLeft.GetPosition().y);
    SDL_RenderFillRect(renderer, &rect);


    SDL_Surface* leftPlayerScoreSurface;
    SDL_Surface* rightPlayerScoreSurface;
    SDL_Texture* leftPlayerScoreTexture;
    SDL_Texture* rightPlayerScoreTexture;


    // Set color to black
    SDL_Color color = { 252, 215, 0, 80 };

    leftPlayerScoreSurface = TTF_RenderText_Solid( scoreFont, std::to_string(scores.leftPlayerScore).c_str(),0, color );
    rightPlayerScoreSurface = TTF_RenderText_Solid( scoreFont, std::to_string(scores.rightPlayerScore).c_str(),0, color );



    leftPlayerScoreTexture = SDL_CreateTextureFromSurface( renderer, leftPlayerScoreSurface );
    rightPlayerScoreTexture = SDL_CreateTextureFromSurface( renderer, rightPlayerScoreSurface );

    SDL_FRect leftPlayerScoreDest = {
        static_cast<float>(WINDOW_WIDTH/2 + 30),
        static_cast<float>(COURT_OFFSET + 10),
        30.0f,
        30.0f
    };
    SDL_FRect rightPlayerScoreDest = {
        static_cast<float>(WINDOW_WIDTH/2 - 60),
        static_cast<float>(COURT_OFFSET + 10),
        30.0f,
        30.0f
    };
    SDL_RenderTexture( renderer, leftPlayerScoreTexture, NULL, &leftPlayerScoreDest );
    SDL_RenderTexture( renderer, rightPlayerScoreTexture, NULL, &rightPlayerScoreDest );


    if (gameover)
    {
        SDL_Surface* gameOverSurface;
        SDL_Texture* gameOverTexture;
        gameOverSurface = TTF_RenderText_Solid( scoreFont, "Game Over",0, color );
        gameOverTexture = SDL_CreateTextureFromSurface( renderer, gameOverSurface );
        if (gameoveranimationcounter < 500)
        {
            gameoveranimationcounter++;
        }
        else
        {
            game->QueueStateChange(GameStateType::MainMenu);
            SDL_ClearAudioStream(sounds[2].stream);
        }

		SDL_FRect gameOverDest;
		if (gameoveranimationcounter < 200)
		{
			gameOverDest = {
				static_cast<float>(WINDOW_WIDTH/2 - 600/2),
				static_cast<float>(WINDOW_HEIGHT - gameoveranimationcounter),
				600.0f,
				100.0f
			};

		}
		else
		{
			gameOverDest = {
				static_cast<float>(WINDOW_WIDTH/2 - 600/2),
				static_cast<float>(WINDOW_HEIGHT - 200),
				600.0f,
				100.0f
			};
		}
		
		SDL_RenderTexture( renderer, gameOverTexture, NULL, &gameOverDest );

		SDL_DestroySurface(gameOverSurface);
		SDL_DestroyTexture(gameOverTexture);
    }


SDL_DestroySurface(leftPlayerScoreSurface);
SDL_DestroySurface(rightPlayerScoreSurface);
SDL_DestroyTexture(leftPlayerScoreTexture);
SDL_DestroyTexture(rightPlayerScoreTexture);

}


void GamePlayState::Cleanup()
{
}