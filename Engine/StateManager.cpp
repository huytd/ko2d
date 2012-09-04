#include "StateManager.h"
#include "GL2D.h"
#include "ParticleSystem.h"
#include "FileSystem.h"

StateManager::StateManager(void)
{
	isFade = false;
	fading = 0;
	currentState = NULL;
	#ifdef _ANDROID
	LOGI("\nAndroid detected\n");
	shader.Init(FileSystem::GetInstance()->GetPath("Shaders/fade.vs ").data(), FileSystem::GetInstance()->GetPath("Shaders/fade.fs ").data());
	#else	
	shader.Init(FileSystem::GetInstance()->GetPath("Shaders/fade.vs").data(), FileSystem::GetInstance()->GetPath("Shaders/fade.fs").data());
	#endif
}


StateManager::~StateManager(void)
{
	this->Destroy();
}

void StateManager::Draw()
{
	if (currentState)
	{
		currentState->Draw();
		ParticleSystem::GetInstance()->Draw();
	}
	if (isFade)
	{
		GL2D::DrawFade(shader.program, fading);
	}
}

void StateManager::Update(float deltaTime)
{
	if (isFade)
	{
		fading += 2 * deltaTime;
		//LOGI("\n%f", fading);
		if (fading >= 1.0)
		{
			fading = 0;
			isFade = false;
			if (currentState)
			{
				currentState->Destroy();
				delete currentState;
				currentState = NULL;
			}
			currentState = targetState;
			currentState->Init();
		}
	}
	else
	if (currentState)
	{
		currentState->Update(deltaTime);
		ParticleSystem::GetInstance()->Update(deltaTime);
	}
	
}

void StateManager::Destroy()
{
	currentState->Destroy();
	delete currentState;
	currentState = NULL;
	ParticleSystem::FreeInstance();
}

void StateManager::SwitchState(GameState* state)
{
	if (!isFade)
	{
		fading = 0;
		isFade = true;
		targetState = state;
	}
}