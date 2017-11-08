#include <Windows.h>
#include <memory>
#include "DebugSystem.h"
#include "Window.h"
#include "Renderer.h"
#include "Game.h"


int CALLBACK WinMain(HINSTANCE _app_instance, HINSTANCE _prev_instance, LPSTR _cmd_line, int _cmd_count)
{
	Window window("Alloy Engine", 1024, 576);
	Renderer renderer(window);

	DebugSystem debugger;
	debugger.CreateConsoleWindow();

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
		renderer.EndFrame();
	}

	return 0;
}
