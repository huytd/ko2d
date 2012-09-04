#include "Application.h"
#include "FileSystem.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Input.h"
#include "StateManager.h"
#include <time.h>
#include "Camera.h"
#include "ResourcesManager.h"
#include "GL2D.h"
#include "..\Build\Windows\Ko2D\InGameState.h"

// create the application here.
Application::Application()
{	
		
}

// Initialize current game.
int Application::Init(const char *resPath)
{		
	LOGI("Init File System");
	FileSystem::GetInstance()->Init(resPath);	
	Camera::GetInstance()->setPosition(0, 0, 5);
	LOGI("Load resources");
	ResourcesManager::GetInstance()->LoadResouces("Data/RM.txt");
	GL2D::GetInstance()->LoadFont("Fonts/font.fnt");
	StateManager::GetInstance()->SwitchState(new InGameState());
	return true;
}

// Update current state
void Application::Update(float deltatime)
{
	StateManager::GetInstance()->Update(deltatime);

	Camera::GetInstance()->Update(deltatime);

	Input * input = Input::GetInstance();

	if ( input->HasPointer(POINTER_DRAGGED) )
	{
		//LOGI("\nPOINTER_DRAGGED");
	}
	if ( input->HasPointer(POINTER_DOWN) )
	{
		//LOGI("\nPOINTER_DOWN - Position %d : %d", input->PointerX, input->PointerY);
	}
	if ( input->HasPointer(POINTER_UP) )
	{
		//LOGI("\nPOINTER_UP");
	}
	if ( input->HoldPointer() )
	{
		//LOGI("\nPOINTER_HOLD");
	}
}

// Render application

void Application::Render()
{	
	glClear(GL_COLOR_BUFFER_BIT);
	StateManager::GetInstance()->Draw();
}

// destroy the application here.
void Application::Destroy()
{
	Application::FreeInstance();
};

Application::~Application()
{	
	StateManager::FreeInstance();
	Input::FreeInstance();	
	FileSystem::FreeInstance();
}

