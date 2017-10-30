#pragma once
#include "GameData.h"
#include <memory>

class Renderer;

class Game
{
public:
	void Tick();
	void Draw(Renderer& _renderer);

private:
	std::unique_ptr<GameData> game_data { nullptr };
};
