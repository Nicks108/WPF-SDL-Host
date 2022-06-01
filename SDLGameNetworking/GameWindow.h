#pragma once
#include "SDL.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_syswm.h>
#include <SDL_ttf.h>

#include "Bitmap.h"
#include "Transfrom2D.h"

class GameWindow
{
private:
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	Transfrom2D HirarcyRoot; //do i want use this across all game windows?

public:

	GameWindow();
	GameWindow(HWND hwnd, int width, int height);
	SDL_Window* pWindow = NULL;
	SDL_Renderer* pRenderer = NULL;
	SDL_GLContext gl_context = NULL;

    

    Transfrom2D& const GetHirarcy()
    {
		return HirarcyRoot;
    }
	void UpdateScene()
	{
		for (Transfrom2D* child : HirarcyRoot.Children)
		{
			//this only supports sprites
			//need to allow for other game object types
			Bitmap *ChiledAsBitmap= static_cast<Bitmap*>(child->Owner);
			ChiledAsBitmap->Update();
			
		}
	}
	void DrawScene()
	{
		SDL_RenderClear(pRenderer);
		//cout << "rendering window" << endl;
		//cout << "children in hirarcy "<<HirarcyRoot.Children.size() << endl;
		for (Transfrom2D* child : HirarcyRoot.Children)
		{
			//this only supports sprites
			//need to allow for other game object types
			Bitmap* ChiledAsBitmap = static_cast<Bitmap*>(child->Owner);
			ChiledAsBitmap->Draw();

		}
		SDL_RenderPresent(pRenderer);
		SDL_Delay(16);
	}

	inline bool isFullScreen()
	{
		return SDL_GetWindowFlags(pWindow) &
			SDL_WINDOW_FULLSCREEN;
	}
	void SetFullscreen(bool setFullScreen);

	inline bool isWindowMaximized()
	{
		return SDL_GetWindowFlags(pWindow) &
			SDL_WINDOW_MAXIMIZED;
	}

	HWND GetWindowHandle ()
	{
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(pWindow, &wmInfo);
		return wmInfo.info.win.window;
	}
	void SetWindowMaximized(bool setMaximized);


	
};

