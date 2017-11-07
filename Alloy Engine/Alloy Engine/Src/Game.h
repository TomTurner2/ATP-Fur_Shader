#pragma once
#include "GameData.h"
#include <memory>
#include "Camera.h"

class Renderer;
class Triangle;
//class Camera;

class Game
{
public:
	explicit Game(Renderer& _renderer, InputManager& _input);
	~Game() = default;

	void Tick();
	void Draw(Renderer& _renderer) const;

private:
	std::unique_ptr<GameData> m_game_data { nullptr };
	std::unique_ptr<Triangle> m_test_triangle { nullptr };
	std::unique_ptr<Camera> m_camera{ nullptr };
};
