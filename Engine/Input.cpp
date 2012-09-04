#include "Input.h"
#include <cstring>
#include "Engine.h"

Input::Input(void)
{
	Reset();
}


Input::~Input(void)
{
}

bool Input::KeyPressed(int keyCode, bool reset)
{
	if ( keyCode >=0 && keyCode < 256)
	{
		bool keyDown = keyState[keyCode];
		keyState[keyCode] = keyState[keyCode] && !reset;
		return keyDown;
	}
	return false;
}

bool Input::KeyPressed()
{	
	return keyPressed;
}



void Input::OnKey(unsigned char keyCode, bool keyPressed)
{	
	this->keyPressed = keyPressed;
	keyState[keyCode] = keyPressed;
	getKeyCode = keyCode;
}

void Input::OnPointer(PointerEvent e, int x, int y, int x2, int y2)
{	
	PointerX = x;
	PointerY = y;	
	PointerX2 = x2; 
	PointerY2 = y2;
	memset(pointerState, 0, sizeof(pointerState));
	pointerState[e] = true;
	if ( e == POINTER_DOWN)
	{
		pointerDown = true;
	}
	else
	{
		pointerDown = false;
	}

}

void Input::Reset()
{
	memset(keyState, 0, sizeof(keyState));
	memset(pointerState, 0, sizeof(pointerState));
	keyPressed = false;	
	pointerDown = false;
}

bool Input::HasPointer(PointerEvent e)
{
	bool p = pointerState[e];	
	pointerState[e] = 0;	
	return p;
}

bool Input::HoldPointer()
{	
	static char countDown = 0;	
	if ( pointerDown == true )
	{
		if ( countDown <= 10 )
			countDown++;		
		if ( countDown >= 10 )
			return true;
	}	
	else
	{
		countDown = 0;
	}
	return false;
}