#pragma once

#include "Singleton.h"

#ifdef _WIN32

#define VK_KEY_LEFT 37
#define VK_KEY_RIGHT 39
#define VK_KEY_DOWN 40
#define VK_KEY_UP 38
#define VK_KEY_A 'A'
#define VK_KEY_S 'S'
#define VK_KEY_D 'D'
#define VK_KEY_W 'W'
#define VK_KEY_SPACE 32
#define VK_KEY_TAB 9
#define VK_KEY_ESC 27

#else

#ifdef _ANDROID

#define VK_KEY_LEFT 21
#define VK_KEY_RIGHT 22
#define VK_KEY_DOWN 20
#define VK_KEY_UP 19
#define VK_KEY_A 'A'
#define VK_KEY_S 'S'
#define VK_KEY_D 'D'
#define VK_KEY_W 'W'
#define VK_KEY_SPACE 23
#define VK_KEY_TAB 82
#define VK_KEY_ESC 4

#endif
#endif

enum PointerEvent
{
    POINTER_NONE,
    POINTER_DOWN,
    POINTER_UP,
    POINTER_DRAGGED,
	POINTER_MAX
};


class Input : public Singleton<Input>
{
private:
	bool keyState[256];
	bool pointerState[POINTER_MAX];	
	bool keyPressed;	
	bool pointerDown;
public:
	int PointerX, PointerY;
	int PointerX2, PointerY2;
	unsigned char getKeyCode;
	Input(void);
	//if bool = true, reset after get key
	bool KeyPressed(int keyCode, bool = false);
	bool KeyPressed();	
	void OnKey(unsigned char keyCode, bool keyPressed);
	
	bool HasPointer(PointerEvent e);
	bool HoldPointer();
	void OnPointer(PointerEvent e, int x, int y, int x2, int y2);
	void Reset();

	virtual ~Input(void);
};


