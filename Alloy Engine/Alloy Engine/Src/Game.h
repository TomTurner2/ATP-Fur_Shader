#pragma once
#include "GameData.h"
#include <memory>

class Renderer;
class Triangle;

class Game
{
public:
	explicit Game(Renderer& _renderer);
	~Game() = default;

	void Tick();
	void Draw(Renderer& _renderer) const;

private:
	std::unique_ptr<GameData> game_data { nullptr };
	std::unique_ptr<Triangle> test_triangle { nullptr };
};
