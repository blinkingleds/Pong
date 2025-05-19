#ifndef PARAMETERS_H
#define PARAMETERS_H

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

const int COURT_OFFSET = 20;

const float BALL_WIDTH = 10;
const float BALL_INIT_VELOCITY = 2000;
const float BALL_VELOCITY_INCR = 10;

const float PADDLE_WIDTH = 10;
const float PADDLE_HEIGHT = 150;

const float PADDLE_ACCEL    = 8000;      // pixel/seconds squared
const float PADDLE_DECEL    = 12000;      // pixel/seconds squared
const float PADDLE_TERMVELO = 15000;      // pixel/seconds

#endif // !PARAMETERS_H