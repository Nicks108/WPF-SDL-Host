#include "GameWindow.h"
#include <SDL.h>
#include <SDL_syswm.h>

#include "WindowsCollection.h"

#define ENGINE_API EXTERN_C __declspec(dllexport)


ENGINE_API
HWND CreatNewChildGameWindow(HWND handle, int width, int height)
{
	//cout << "in DLL Create child window" << endl;
	return WindowsCollection::Instance().CreateNewChildGameWindow(handle, width, height);
}

//ENGINE_API
//HWND GetWindowHandle()
//{
//	
//
//	
//
//	//SDL_SysWMinfo systemInfo;
//	//SDL_VERSION(&systemInfo.version);
//	//SDL_GetWindowWMInfo(pWindow, &systemInfo);
//
//	HWND handle;// systemInfo.info.win.window;
//
//	std::cerr << "Handel: " << handle << std::endl;
//
//	return handle;
//}


ENGINE_API
void dllRender()
{
	//cout << "in DLL render" << endl;
	WindowsCollection::Instance().UpdateAllGameWindows();
}

//ENGINE_API
//void My_WIN_WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	WIN_WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//}