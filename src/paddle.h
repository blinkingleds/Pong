#ifndef PADDLE_H
#define PADDLE_H


#include "common.h"
#include "parameters.h"


class Paddle
{
private:
	Vec2D position;
	float height;
	float width;
	Vec2D velocity;

public:
	Paddle(Vec2D startposition, float paddleheight, float paddlewidth)
	{
		position = startposition;
		height   = paddleheight;
		width    = paddlewidth;
		velocity = {0,0};
	}

	Vec2D GetPosition() const
	{
		return position;
	}

	Vec2D GetVelocity() const
	{
		return velocity;
	}

	float GetHeigth() const
	{
		return height;
	}

	float GetWidth() const
	{
		return width;
	}

	void SetPosition(Vec2D newposition)
	{
		position = newposition;
	}

	void SetVelocity(Vec2D newvelocity)
	{
		velocity = newvelocity;
	}


};

#endif // PADDLE_H