#pragma once
#include <vector>

#include "GameWindow.h"


class WindowsCollection
{
private:
	WindowsCollection() {};

public:

	static WindowsCollection& Instance()
	{
		static WindowsCollection INSTANCE;
		return INSTANCE;
	}

	std::vector<GameWindow*> GameWindows;

	void UpdateAllGameWindows()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) // poll all events
		{
			
			cout << "SDL input: "<< e.type << endl;
			switch (e.type)
			{
			case SDL_MOUSEMOTION:
				cout << "SDL mouse" << endl;
				break;
			case SDL_KEYDOWN:
				cout << "SDL keydown" << endl;
				break;
			}

			
		}
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_RETURN]) {
			cout << "<RETURN> is pressed." << endl;
		}
		if (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) {
			printf("Right and Up Keys Pressed.\n");
		}

		for (GameWindow* win : GameWindows)
		{
			win->DrawScene();

		}
	}

	HWND CreateNewChildGameWindow(HWND handle, int width, int height)
	{
		GameWindow* gw = new GameWindow(handle, width, height);
		GameWindows.push_back(new GameWindow(handle,  width,  height));
		return gw->GetWindowHandle();
	}
	GameWindow* CreateNewGameWindow()
	{
		GameWindow* win = new GameWindow();
		GameWindows.push_back(win);
		return win;
	}
};


