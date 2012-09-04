#pragma once
#include "Engine.h"
#include "Singleton.h"
#include "GameState.h"
#include "Shaders.h"

class StateManager: public Singleton<StateManager>
{
public:
	StateManager(void);
	~StateManager(void);
	GameState* currentState;

	float fading; 
	bool isFade;
	GameState* targetState;
	Shaders shader;
	
	void Draw();
	void Update(float deltaTime);
	void Destroy();
	void SwitchState(GameState* state);
};

