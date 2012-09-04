// This file will run on windows 32
#ifdef _WIN32

#include <windows.h>
#include "Engine.h"
#include <cstdio>
#include <EGL\egl.h>
#include "Application.h"


// EGL variables
EGLDisplay			eglDisplay	= 0;
EGLConfig			eglConfig	= 0;
EGLSurface			eglSurface	= 0;
EGLContext			eglContext	= 0;
EGLNativeWindowType	eglWindow	= 0;
// Windows variables
HWND				hWnd	= 0;
HDC					hDC		= 0;
void CleanUp();
bool TestEGLError(HWND hWnd, char* pszLocation);
bool Init(GLuint width, GLuint height);
//---------------------------------------------------------------------------



//-----------------------
int main(int n, char **args)
{
	if ( Init( SCREEN_W, SCREEN_H ) == false )
		return -1;
	
	if ( Application::GetInstance()->Init( "../../../../Resource" ) == false )
		return -1;

	MSG msg = {0};
	int done = 0;
	DWORD lastTime = GetTickCount();

	while (!done)
	{
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		DWORD curTime = GetTickCount();
		float deltaTime = (float)( curTime - lastTime ) / 1000.0f;		
		lastTime = curTime;

		if ( gotMsg )
		{
			if (msg.message==WM_QUIT)
			{
				done = 1; 
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
		else
		{			
			Application::GetInstance()->Render();
			eglSwapBuffers(eglDisplay, eglSurface);			
		}						
		Application::GetInstance()->Update(deltaTime);			
	}	

	Application::GetInstance()->Destroy();
		
	return 0;
}

LRESULT CALLBACK WndProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{

	LRESULT  lRet = 1; 
	int x = LOWORD(lParam); 
	int y = HIWORD(lParam);
	static bool mouseDown = false;

	switch (Msg) 
	{ 
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break; 

	case WM_KEYDOWN:
		Input::GetInstance()->OnKey((unsigned char) wParam, true);
		break;
	case WM_KEYUP:
		Input::GetInstance()->OnKey((unsigned char) wParam, false);
		break;
	case WM_LBUTTONDOWN:		
	case WM_RBUTTONDOWN:		
		mouseDown = true;		
		Input::GetInstance()->OnPointer(POINTER_DOWN, x, y, -1, -1);
		break;

	case WM_LBUTTONUP:	
	case WM_RBUTTONUP:
		mouseDown = false;
		Input::GetInstance()->OnPointer(POINTER_UP, x, y, -1, -1);
		break;
	case WM_MOUSEMOVE:		
		if (mouseDown)
		{			
			Input::GetInstance()->OnPointer(POINTER_DRAGGED, x, y, -1, -1);
		}		
		break;
	default: 
		lRet = DefWindowProc (hWnd, Msg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}

bool TestEGLError(HWND hWnd, char* pszLocation)
{	
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		TCHAR pszStr[256];
		sprintf_s(pszStr, ("%s failed (%d).\n"), pszLocation, iErr);
		MessageBox(hWnd, pszStr, ("Error"), MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

void CleanUp()
{
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	eglTerminate(eglDisplay);

	if (hDC) 
		ReleaseDC(hWnd, hDC);

	if (hWnd) 
		DestroyWindow(hWnd);
}


bool Init(GLuint width, GLuint height)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	
	// Register the windows class
	WNDCLASS sWC;
	sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProcedure; //Procedute Callback Event
	sWC.cbClsExtra = 0;
	sWC.cbWndExtra = 0;
	sWC.hInstance = hInstance;
	sWC.hIcon = 0;
	sWC.hCursor = 0;
	sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	sWC.lpszClassName = "OpenGLES2.0";


	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, ("Failed to register the window class"), ("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
	// Create the eglWindow
	RECT	sRect;
	SetRect(&sRect, 0, 0, SCREEN_W, SCREEN_H);
	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
	hWnd = CreateWindow( "OpenGLES2.0", "HEngine", WS_VISIBLE | WS_SYSMENU,
		SCREEN_W / 3, SCREEN_H / 2, sRect.right - sRect.left, sRect.bottom - sRect.top, NULL, NULL, hInstance, NULL);
	eglWindow = hWnd;

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, "Failed to create the device context", "Error", MB_OK|MB_ICONEXCLAMATION);
		CleanUp();
		return false;
	}

	eglDisplay = eglGetDisplay(hDC);

	if(eglDisplay == EGL_NO_DISPLAY)
		eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		MessageBox(0, ("eglInitialize() failed."), ("Error"), MB_OK|MB_ICONEXCLAMATION);
		CleanUp();
		return false;
	}
	printf("\nVersion: %d %d\n", iMajorVersion, iMinorVersion);

	const EGLint pi32ConfigAttribs[] =
	{
		EGL_LEVEL,				0,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,		
		EGL_DEPTH_SIZE,			8,
		EGL_STENCIL_SIZE, 8,
		EGL_SAMPLE_BUFFERS, 1,
		EGL_NONE
	};


	int iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		MessageBox(0, ("eglChooseConfig() failed."), ("Error"), MB_OK|MB_ICONEXCLAMATION);
		CleanUp();
		return false;
	}


	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

	if(eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
	{
		CleanUp();
		return false;
	}


	// Bind the API (It could be OpenGLES or OpenVG)
	eglBindAPI(EGL_OPENGL_ES_API);
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

	if (!TestEGLError(hWnd, "eglCreateContext"))
	{
		CleanUp();
		return false;
	}

	
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

	if (!TestEGLError(hWnd, "eglMakeCurrent"))
	{
		CleanUp();
		return false;
	}

	return true;
}

#endif