#include "GameWindow.h"

#include "Sprite.h"


GameWindow::GameWindow()
{
	

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << "SDL couldnt not be initialized! ERROR: " << SDL_GetError() << std::endl;
	}
	else
	{

		/*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);*/

		pWindow = SDL_CreateWindow("SDL Network Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI| SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
		if (pWindow == NULL)
		{
			std::cerr << "Window could not be created! ERROR: " << SDL_GetError() << std::endl;
		}
		else
		{

			gl_context = SDL_GL_CreateContext(pWindow);
			SDL_GL_MakeCurrent(pWindow, gl_context);
			SDL_GL_SetSwapInterval(1); // Enable vsync

			pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
			if (pRenderer == NULL)
			{
				std::cerr << "Craeting Renderer Failed! SDL error: " << SDL_GetError() << std::endl;
			}

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!IMG_Init(imgFlags) & imgFlags)
			{
				std::cerr << "SDL image could not Initilized! SDL_image error: " << IMG_GetError() << std::endl;
			}
			//Initialize SDL_ttf
			if (TTF_Init() == -1)
			{
				std::cerr << "SDL_TTF could not be Initilized! SDL_TTF error: " << TTF_GetError() << std::endl;
			}

			/*pScreenSurface = SDL_GetWindowSurface(pWindow);
			SDL_FillRect(pScreenSurface, NULL, SDL_MapRGB(pScreenSurface->format, 0, 0, 128));*/

			SDL_SetRenderDrawColor(pRenderer, 0, 0, 128, 0);
		}
	}
}


GameWindow::GameWindow(HWND hwnd, int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << "SDL couldnt not be initialized! ERROR: " << SDL_GetError() << std::endl;
	}
	SDL_GetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT);

	pWindow = SDL_CreateWindowFrom((void*)hwnd);
    
	//pWindow = SDL_CreateWindow("SDL Network Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		//SCREEN_WIDTH, SCREEN_HEIGHT,
		//SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

	if (pWindow == NULL)
	{
		std::cerr << "Window could not be created! ERROR: " << SDL_GetError() << std::endl;
	}
	SDL_SetWindowSize(pWindow, width, height);

	gl_context = SDL_GL_CreateContext(pWindow);
	SDL_GL_MakeCurrent(pWindow, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (pRenderer == NULL)
	{
		std::cerr << "Craeting Renderer Failed! SDL error: " << SDL_GetError() << std::endl;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!IMG_Init(imgFlags) & imgFlags)
	{
		std::cerr << "SDL image could not Initilized! SDL_image error: " << IMG_GetError() << std::endl;
	}
	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cerr << "SDL_TTF could not be Initilized! SDL_TTF error: " << TTF_GetError() << std::endl;
	}

	/*pScreenSurface = SDL_GetWindowSurface(pWindow);
	SDL_FillRect(pScreenSurface, NULL, SDL_MapRGB(pScreenSurface->format, 0, 0, 128));*/

	SDL_SetRenderDrawColor(pRenderer, 0, 0, 128, 0);

	//D:\One Drive - UOS\OneDrive - University of Suffolk\UOS\Units\Tools For Game Development\06 - im gui 1\SDLGameNetworking - C# editor test\Assets
	Sprite* TestPlayer = new Sprite(pRenderer, "../../../Assets/megaMan.png", 50, 50, true, "player");
	TestPlayer->Transfrom.ParentSet(GetHirarcy());


}


void GameWindow::SetFullscreen(bool setFullScreen) {
	//Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	//isFullScreen = SDL_GetWindowFlags(pWindow) & FullscreenFlag;
	SDL_SetWindowFullscreen(pWindow, setFullScreen ? 0 : SDL_WINDOW_FULLSCREEN);
	SDL_ShowCursor(true);
}

void GameWindow::SetWindowMaximized(bool setMaximized) {
	//Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	//isFullScreen = SDL_GetWindowFlags(pWindow) & FullscreenFlag;
	SDL_SetWindowFullscreen(pWindow, setMaximized ? 0 : SDL_WINDOW_MAXIMIZED);
	SDL_ShowCursor(true);
}