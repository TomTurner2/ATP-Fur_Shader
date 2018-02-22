#include <Windows.h>
#include <memory>
#include "DebugSystem.h"
#include "Window.h"
#include "Renderer.h"
#include "Game.h"
#include "WICTextureLoader.h"
#include <ctime>


int CALLBACK WinMain(HINSTANCE _app_instance, HINSTANCE _prev_instance, LPSTR _cmd_line, int _cmd_count)
{
	CoInitialize(nullptr);//initialise WIC texture loading
	srand(time(nullptr));//seed rand

	Window window("Alloy Engine", 1366, 768);
	Renderer renderer(window);

	DebugSystem debugger;
	//debugger.CreateConsoleWindow();
	debugger.InitAntTweakBar(renderer);

	std::unique_ptr<InputManager> input_manager = std::make_unique<InputManager>();
	std::unique_ptr<Game> game = std::make_unique<Game>(renderer, *input_manager);

	MSG msg { nullptr };

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			input_manager->ProcessMessage(msg.message, msg.wParam);//polls for input

			if (msg.message == WM_QUIT)
				break;
		}

		game->Tick();
		renderer.BeginFrame();
		game->Draw(renderer);
		debugger.DrawDebugUI();
		renderer.EndFrame();	
	}

	return 0;
}
