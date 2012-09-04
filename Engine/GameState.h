#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "Engine.h"

class GameState
{
public:
	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Destroy() = 0;
};

#endif