#pragma once
#include "..\..\..\Engine\GameState.h"
#include "..\..\..\Engine\GL2D.h"

class InGameState: public GameState
{
public:
	void Init()
	{
	}
	void Update(float deltaTime)
	{
	}
	void Draw()
	{
		GL2D::GetInstance()->DrawString("HELLO WORLD", SCREEN_W / 2 - 100, SCREEN_H / 2 - 25);
		GL2D::GetInstance()->DrawString("iAM KO2D", SCREEN_W / 2 - 75, SCREEN_H / 2);
	}
	void Destroy()
	{
	}
};