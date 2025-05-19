#ifndef BALL_H
#define BALL_H

#include "common.h"
#include "parameters.h"

class Ball
{
private:
	Vec2D position;
	Vec2D velocity;
	float width;

public:
	Ball(Vec2D startposition, float ballwidth)
		: position(startposition), width(ballwidth)
	{
		velocity = Vec2D(BALL_INIT_VELOCITY, 0);
	}

	Vec2D GetPosition() const
	{
		return position;
	}

	Vec2D GetVelocity() const
	{
		return velocity;
	}

	float GetWidth() const
	{
		return width;
	}

	void SetPosition(Vec2D pos)
	{
		position = pos;
	}

	void SetVelocity(Vec2D vel)
	{
		velocity = vel;
	}
};

#endif // !BALL_H