#ifndef SOUND_H
#define SOUND_H

struct PlaySound 
{
	bool rightPaddleHit;
	bool leftPaddleHit;
	bool backgroundSound;
	bool ballPastPaddle;
	bool wallHit;
	bool gameover;
};

struct Sound 
{
    Uint8 *wav_data;
    Uint32 wav_data_len;
    SDL_AudioStream *stream;
};

#endif	// SOUND_H