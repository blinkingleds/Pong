#ifndef PLAYERACTIONS_H
#define PLAYERACTIONS_H

enum PlayerAction
{
	PA_NONE,
	PA_MOVEUP,
	PA_MOVEDOWN
};

struct PlayerInput
{
	PlayerAction playerRight;
	PlayerAction playerLeft;
};


#endif	// PLAYERACTIONS_H