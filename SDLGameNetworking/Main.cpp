//#define GLEW_STATIC

//#include "GL/glew.h"
#include <SDL.h>
#undef main
//#include <SDL_image.h>

#include <iostream>
#include <String>
#include <sstream>
#include <queue>
#include <filesystem>
#include <iostream>

#include "Sprite.h"
#include "UDPConnection.h"
#include "Text.h"
#include "ComponentHeaders.h"
#include "GameWindow.h"
#include "WindowsCollection.h"
#include "SceanHirarcyWindow.h"
#include "ProfilerSystem.h"

//#include "KW_gui.h"
//#include "KW_button.h"
//#include "SDL_image.h"
//#include "KW_frame.h"
//#include "KW_editbox.h"
//#include "KW_renderdriver_sdl2.h"

#include <thread>
//#include <gl/GL.h>
//#include <SDL_opengl.h>



#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "imgui_sdl.h"
#include "imgui_internal.h"



//TODO Clean up this bad code! stop being lazy!!!!
//const int SCREEN_WIDTH = 1280;
//const int SCREEN_HEIGHT = 720;
bool canQuit = false;
Sprite* TestPlayer;


void HandleEvents(ImGuiIO& io);
static void ShowExampleAppDockSpace(bool* p_open);

enum GameStates {
	SPLASH_SCREEN,
	CONNECTION_SCREEN,
	GAME_SCREEN
};
GameStates CurrentState = SPLASH_SCREEN;



struct FrameTimeStack
{
public:
	std::vector<float> FrameTimeQueue;
	int Capacity = 240;
	void push(float FrameTime)
	{
		int diff = FrameTimeQueue.size() - Capacity;
		if (diff > 0)
		{
			for (int i=0; i<= diff; i++)
			{
				FrameTimeQueue.erase(FrameTimeQueue.begin());
			}
		}

		FrameTimeQueue.push_back(FrameTime);
	}

	float AverageTime()
	{
		float average = 0;
		for (int i = 0; i < FrameTimeQueue.size(); i++)
			average += FrameTimeQueue[i];

		return average / FrameTimeQueue.size();
	}

};







int main(int argc, char *argv[])
{
	CurrentState = GAME_SCREEN;

	GameWindow* gameWindow=  WindowsCollection::Instance().CreateNewGameWindow();
	gameWindow->SetFullscreen(true); //<<SDL INIT code Here


	//imGUI Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	SDL_DisplayMode DisplayMode;
	SDL_GetCurrentDisplayMode(0, &DisplayMode);
	ImGuiSDL::Initialize(gameWindow->pRenderer, DisplayMode.w, DisplayMode.h);
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(gameWindow->pWindow, gameWindow->gl_context);










	//scene Hirarcy should probably be a hirarcy obj and game objects should inherit from that
	//hirarcy obj contains vector of childeren
	//vector<Bitmap*> SceanHierarchy;
	TestPlayer = new Sprite(gameWindow->pRenderer, "../Assets/megaMan.png", 50, 50, true, "player");
	//SceanHierarchy.push_back(TestPlayer);
	TestPlayer->Transfrom.ParentSet(gameWindow->GetHirarcy());
	//TestPlayer->AddComponet(new PrintToConsole());
	//SceanHierarchy.push_back(new Sprite(pRenderer, "../Assets/megaMan.png", 100, 50, true, ""));
	//SceanHierarchy.push_back(new Sprite(pRenderer, "../Assets/megaMan.png", 100, 100, true,  ""));

	Sprite* child = new Sprite(gameWindow->pRenderer, "../Assets/megaMan.png", 100, 100, true, "Child");
	child->Transfrom.ParentSet(TestPlayer->Transfrom);
	//SceanHierarchy.push_back(child);

	Sprite* child2 = new Sprite(gameWindow->pRenderer, "../Assets/megaMan.png", 200, 200, true, "New Item");
	child2->Transfrom.ParentSet(gameWindow->GetHirarcy());










	SDL_Rect textrext;
	textrext.w = DisplayMode.w;
	textrext.h = 100;
	textrext.x = 0;
	textrext.y = (DisplayMode.h /2) - (textrext.h/2);
	
	Text* SplashText = new Text(gameWindow->pRenderer, "A Splash Screen GAME!", textrext, 48);
	int FrameCount = 0;


	int OldTime = SDL_GetTicks();
	FrameTimeStack frameTimes;

	SceanHirarcyWindow sceneHirarcy(gameWindow);




	///////////////////////////
	///read files in assets folder
	//////////////////////////



	vector<Bitmap*> content;
	std::string path = "../Assets";
	for (const auto& entry : std::filesystem::directory_iterator(path)) //directory_iterator(path) //recursive_
	{
		if (entry.path().extension() == ".bmp" || entry.path().extension() == ".jpg" || entry.path().extension() == ".png")
		{
			Bitmap* Asset = new Bitmap(gameWindow->pRenderer, entry.path().u8string().c_str(), 0, 0, true);
			content.push_back(Asset);

		}
		else if(entry.is_directory())
		{
			std::cout << "dir " << entry << std::endl;
		}
		std::cout << entry.path() << std::endl;
	}



	Bitmap* AssetMousDrag = nullptr;


	

	int x, y;
	SDL_GetRendererOutputSize(gameWindow->pRenderer, &x, &y);
	SDL_Texture* RendTarget = SDL_CreateTexture(gameWindow->pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		x, y);


	//selected element of histogram
	int selectedFrame = -1;

	bool Open = true;

	while (!canQuit)
	{
		ProfilerSystem::Instance().StartFrame();

		{
			PROFILE("Render To Texture pass");
			SDL_SetRenderTarget(gameWindow->pRenderer, RendTarget);
			SDL_RenderClear(gameWindow->pRenderer);
			gameWindow->DrawScene();
			SDL_SetRenderTarget(gameWindow->pRenderer, NULL);

			//SDL_UpdateWindowSurface(pWindow);

			SDL_RenderClear(gameWindow->pRenderer);
			//Draw stuff!!
		}


		const int menuHeight = 20;


		HandleEvents(io);


		ImGui::NewFrame();
		ImGui_ImplSDL2_NewFrame(gameWindow->pWindow);
		bool show = true;
		ShowExampleAppDockSpace(&show);
		ImGui::ShowDemoWindow(nullptr);


		ImGui::Begin("Profiler");
		
		static FrameMap* Snapshot;
		static vector<float>* FrameTimes;
		//take snapshot of all current frame data.
		if (ImGui::Button("take snapshot"))
		{
			//mem copy?
			Snapshot = &(ProfilerSystem::Instance().GetFrameDate());
			FrameTimes = &(ProfilerSystem::Instance().GetFrameTimes());
		}
		ImGui::SameLine();
		static bool LiveFlameGraph = true;
		ImGui::Checkbox("Live Flame Graph", &LiveFlameGraph);
		if(LiveFlameGraph)
		{
			selectedFrame = -1;
		}
		static int range[2] = {0, 100};

		if (FrameTimes && FrameTimes->size() > 100)
		{
			ImGui::SliderInt2("Frame Range", range, 0, FrameTimes->size());
			if (range[0] >= range[1])
				range[0] = range[1] - 1;
			/*ImGui::SliderInt("Frame Range", &range[0], 0, FrameTimes->size());
			if (range[0] <10)
				range[0] =10;*/
			vector<float> subData(FrameTimes->cbegin() + range[0], FrameTimes->cbegin() + range[1]);
			//vector<float> subData(FrameTimes->cbegin() + range[0]-10, FrameTimes->cbegin() + range[0]);

			int tempHistSelection =ImGui::MyPlotHistogram("Frame data", subData.data(), subData.size());
			if (tempHistSelection != -1)
			{
				LiveFlameGraph = false;
				selectedFrame = tempHistSelection;
			}

			//cout << t << endl;
			//get mouse in hostogram
			ImRect rect = { ImGui::GetItemRectMin(), ImGui::GetItemRectMax() };
			if(rect.Contains(io.MousePos))
			{
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					cout << selectedFrame << endl;
				}
			}

		}


		//float GraphWindowWidth = ImGui::CalcItemWidth();
		FrameMap& previousFrame = ProfilerSystem::Instance().GetLastFrameData();
		if(!LiveFlameGraph && selectedFrame != -1)
		{
			previousFrame.clear();
			for(auto const [SampleName, samples]: *Snapshot)
			{
				previousFrame[SampleName].push_back(samples[range[0]+selectedFrame]);
			}
        }
		else
		{
			LiveFlameGraph = false;
		}
		//ImGui::PlotHistogram("FrameTimes")
		ImGui::LabelText("Frame Date Count", std::to_string(previousFrame.size()).c_str());
		//ImGui::LabelText("Graph Window Width", std::to_string(GraphWindowWidth).c_str());
		//ImGui::RenderFrame({ 10,20 }, { 100,50 }, ImGui::GetColorU32(ImGuiCol_FrameBg), true, GImGui->Style.FrameRounding);
		ImDrawList* drawlist = ImGui::GetCurrentWindow()->DrawList;
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();//pos of screen top left
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
		drawlist->PushClipRect(canvas_p0, canvas_p1, true);

		
		uint64_t totalframeTime =0;
		vector<uint64_t> SampleTimes;
		vector<float> SampleWidths;
		vector<string> SampleNames;
		for(auto const& [SampleName, samples] : previousFrame)
		{
			totalframeTime += samples[0]->frameTime+1;
			SampleTimes.push_back(samples[0]->frameTime+1);
			SampleNames.push_back(SampleName);
		}
		float MinBlockWith = canvas_sz.x / totalframeTime;// GraphWindowWidth / totalframeTime;
		for (int i = 0; i < SampleTimes.size(); i++)
		{
			SampleWidths.push_back(SampleTimes[i] * MinBlockWith);
		}

		//ImGui::LabelText("Total window width", std::to_string(GraphWindowWidth).c_str());
		ImGui::LabelText("Total Frame Time", std::to_string(totalframeTime).c_str());
		ImGui::LabelText("Window Width / total frame Time", std::to_string(MinBlockWith).c_str());
		float TotalBlockWidthSoFar = 0;

		int sampleCount = previousFrame.size();
		//ImGui::SliderInt("Iterations", &sampleCount, 1, 10);
		//sampleCount = 2;
		//MinBlockWith = GraphWindowWidth / sampleCount;

		const ImU32 col_outline_base = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF;
		const ImU32 col_base = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF;

		float f = 5.0f + 5.0f;

		for (int i = 0; i < sampleCount; i++)
		{
			float ThisBlockWidth = SampleWidths[i];
			//if (ThisBlockWidth == 0)
				//ThisBlockWidth = MinBlockWith;

			const ImVec2 minPos = ImVec2(canvas_p0.x + TotalBlockWidthSoFar, canvas_p0.y + 100);
			//float a = canvas_sz.x - (canvas_p1.x - (canvas_p1.x*i))
			const ImVec2 maxPos = ImVec2(canvas_p0.x + TotalBlockWidthSoFar + ThisBlockWidth, canvas_p0.y + 200);
			drawlist->AddRectFilled(
				minPos,
				maxPos,
				col_base,
				GImGui->Style.FrameRounding);

			drawlist->AddRect(minPos, maxPos, col_outline_base);

			ImGui::RenderText(ImVec2(minPos.x+ 10, minPos.y+10), SampleNames[i].c_str());
			ImGui::RenderText(ImVec2(minPos.x+ 10, minPos.y+20), std::to_string(SampleTimes[i]-1).c_str());


			TotalBlockWidthSoFar += ThisBlockWidth;
		}
		drawlist->PopClipRect();

		ImGui::End();









		switch (CurrentState)
		{
		case SPLASH_SCREEN:
			FrameCount++;
			SplashText->Draw();
			if (FrameCount >= 180)
				CurrentState = GAME_SCREEN;
			break;

		case GAME_SCREEN:
		{

			{
				PROFILE("Scene Update");
				gameWindow->UpdateScene();
			}
			{
				PROFILE("Scene Draw");
				gameWindow-> DrawScene();
			}

			sceneHirarcy.Draw();
			sceneHirarcy.GUIUpdateScene(io);



				//hirarcy drag
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && AssetMousDrag != nullptr)
			{
				cout << "Test" << endl;
				int x, y;
				SDL_GetMouseState(&x, &y);
				Sprite* s = new Sprite(gameWindow->pRenderer, AssetMousDrag->FileName, x, y, true, AssetMousDrag->ObjectName);
				s->Transfrom.ParentSet(gameWindow->GetHirarcy());
				AssetMousDrag = nullptr;
			}

			/*ImGui::Begin("Hirarcy");
			static bool DrawDebug;
			ImGui::Checkbox("Draw Debug", &DrawDebug);
			ImGui::End();*/

			/* for (Bitmap* bitmap : SceanHierarchy)
			 {
				 bitmap->Update();
				 bitmap->Draw();*/
				 //for (int i = 0; i < bitmap->Transfrom.Children.size(); i++)
				 //{
				 //	Bitmap* child = (Bitmap*)bitmap->Transfrom.Children[i]->Owner;
				 //	child->Draw();
				 //}
				 //
				 ////static_cast<Sprite*>(bitmap)->GUIDraw();
				 //static_cast<Sprite*>(bitmap)->DrawDebug(DrawDebug);
				 //static_cast<Sprite*>(bitmap)->MouseUpdate(io);
				 //if( static_cast<Sprite*>(bitmap)->CheckBounds(io))
				 //{
				 //	I_GUIWindow::SelectedObject = bitmap;
				 //}



				 //ImGui::Begin("Hirarcy");
				 //

				 //if (ImGui::TreeNode(bitmap->ObjectName.c_str()))
				 //{
				 //	//cout << bitmap->ObjectName.c_str() << endl;
				 //	I_GUIWindow::SelectedObject = bitmap;

				 //	for (int i = 0; i < bitmap->Transfrom.Children.size(); i++)
				 //	{
				 //		Bitmap* child = (Bitmap*) bitmap->Transfrom.Children[i]->Owner;
				 //		child->GUIDrawChildGUI();
				 //		
				 //	}
				 //	ImGui::TreePop();
				 //}
				 //ImGui::End();
			 //}
			if (I_GUIWindow::SelectedObject != nullptr)
			{
				static_cast<Sprite*>(I_GUIWindow::SelectedObject)->GUIDraw();
				static_cast<Sprite*>(I_GUIWindow::SelectedObject)->DrawDebug(true);
			}

			




			//ImGui::ShowDemoWindow(&Open);
			frameTimes.push(io.DeltaTime * 1000);

			ImGui::Begin("Frame Time");
			ImGui::PlotLines(" Frames", frameTimes.FrameTimeQueue.data(), frameTimes.Capacity);
			char buffer[64];
			snprintf(buffer, sizeof buffer, "%f ms", frameTimes.AverageTime());
			ImGui::Text(buffer);
			ImGui::End();



			ImGui::Begin("Editor");
			ImGui::BeginChild("Content Window", ImVec2(), true);
			//ImGui::BeginTable("Content browser", 3);

			;
			for (int i = 0; i < content.size(); i++)
			{
				ImGui::PushID(i);

				ImGui::ImageButton((ImTextureID)content[i]->GetTextureRef(), { 100,100 });

				if (ImGui::BeginDragDropSource())
				{
					AssetMousDrag = content[i];
					ImGui::Image((ImTextureID)content[i]->GetTextureRef(), { 100,100 });
					ImGui::EndDragDropSource();
				}
				ImGui::PopID();
				ImGui::SameLine();
			}

			//ImGui::EndTabItem();

			ImGui::EndChild();


			ImGui::BeginChild("sub Window", ImVec2(200, 200), true);
			if(ImGui::IsWindowHovered())
			{
				ImVec2 mousePos = ImGui::GetMousePos();
				ImVec2 WinPos = ImGui::GetWindowPos();
				float RelativeX = mousePos.x / WinPos.x;
				float RelativeY = mousePos.y/ WinPos.y ;

				cout << "X,Y "<<RelativeX << " "<<RelativeY << endl;
				
			}
			ImVec2 size = ImGui::GetWindowSize();

			ImGui::PushID("test");
			ImGui::Image((ImTextureID)RendTarget, { size.x,size.y });

			
			ImGui::PopID();
			ImGui::EndChild();
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("window");
				cout << "sub test" << endl;

				ImGui::EndDragDropTarget();
			}

			ImGui::End();










		}
		break;
		default:
			break;
		}

		{
			PROFILE("ImGui Render");
			ImGui::Render();
			ImGuiSDL::Render(ImGui::GetDrawData());
			// Update and Render additional Platform Windows
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
		}


		//SDL_GL_SwapWindow(pWindow);

		
		{
			PROFILE("SDL Render");
			SDL_RenderPresent(gameWindow->pRenderer);
		}



		
		//SDL_Delay(16);

		ProfilerSystem::Instance().EndFrame();
	}//end while loop

	ProfilerSystem::Instance().WriteDataToCSV();
			
		
		
	//destroy bitmap objects
	delete TestPlayer;
	TestPlayer = nullptr;

	delete SplashText;
	SplashText = nullptr;

	//ImGui_ImplOpenGL2_Shutdown();
	//ImGui_ImplSDL2_Shutdown();



		



	SDL_DestroyRenderer(gameWindow->pRenderer);
	SDL_DestroyWindow(gameWindow->pWindow);
	TTF_Quit();
	SDL_Quit();


	return 0;
}


void HandleEvents(ImGuiIO& io)
{
	int wheel = 0;
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) // poll all events
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch (e.type)
		{
		case SDL_QUIT:
			//User requets quit
			canQuit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
		
			
			break;
		case SDL_MOUSEBUTTONUP:
			break;
		case SDL_MOUSEMOTION:
			//int x, y;
			//SDL_GetMouseState(&x, &y);
			//bool leftButtonClick = (e.button.button == SDL_BUTTON_LEFT);
			break;

		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				io.DisplaySize.x = static_cast<float>(e.window.data1);
				io.DisplaySize.y = static_cast<float>(e.window.data2);
			}
			
			break;
		case SDL_MOUSEWHEEL:
			
			wheel = e.wheel.y;
			
			break;
		}
	}


	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);




	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_ESCAPE])
	{
		canQuit = true;
	}
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		TestPlayer->Transfrom.Position().Y += -1;
	}
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		TestPlayer->Transfrom.Position().Y += 1;
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		TestPlayer->Transfrom.Position().X += -1;
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		TestPlayer->Transfrom.Position().X += 1;
	}
}






void ShowExampleAppDockSpace(bool* p_open)
{
	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
	// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
	// In this specific demo, we are not using DockSpaceOverViewport() because:
	// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
	// - we allow the host window to have padding (when opt_padding == true)
	// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
	// TL;DR; this demo is more complicated than what you would normally use.
	// If we removed all the options we are showcasing, this demo would become:
	//     void ShowExampleAppDockSpace()
	//     {
	//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	//     }

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	
		dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
	

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	
	
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);


	if (ImGui::BeginDragDropTarget())
	{
		cout << "dropping in main window" << endl;
		ImGui::EndDragDropTarget();
	}


	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		
	}


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();

		
	}

	


	ImGui::End();
}




