#pragma once

#include "Engine.h"
#include "Input.h"
#include "Engine.h"
#include <stack>


class Application : public Singleton<Application>
{
	
public:	
    // Create the application.
    Application();
    
    // Destroy the application.
    virtual ~Application();
    
    // Render current scene.
    virtual void Render();
    
    // Update Current Scene.
    virtual void Update(float deltatime);
            
    // Initialize the application.
    virtual int Init(const char *);
	
	virtual void Destroy();
        
protected:
   
};

