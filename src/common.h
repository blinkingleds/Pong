#ifndef COMMON_H
#define COMMON_H

struct Vec2D
{
	float x;
	float y;
	
	Vec2D() : x(0), y(0) {}
	Vec2D(float x_, float y_) : x(x_), y(y_) {}
};

struct playerScores
{
    int leftPlayerScore;
    int rightPlayerScore;
};

struct Timer
{
    Uint64 previous_ticks{};
    float elapsed_seconds{};

    void tick()
    {
        const Uint64 current_ticks{ SDL_GetPerformanceCounter() };
        const Uint64 delta{ current_ticks - previous_ticks };
        previous_ticks = current_ticks;
        static const Uint64 TICKS_PER_SECOND{ SDL_GetPerformanceFrequency() };
        elapsed_seconds = delta / static_cast<float>(TICKS_PER_SECOND);
    }
};


enum GameState1
{
    GS_MAINMENU,
    GS_PAUSE,
    GS_PLAYING,
    GS_GAMEOVER
};

#endif // !COMMON_H
