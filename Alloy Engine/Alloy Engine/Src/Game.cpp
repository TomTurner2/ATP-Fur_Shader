#include "Game.h"
#include "Triangle.h"


Game::Game(Renderer& _renderer)
{
	m_test_triangle = std::make_unique<Triangle>(_renderer);
	m_game_data = std::make_unique<GameData>();
}


void Game::Tick()
{
	//update objects here
}


void Game::Draw(Renderer& _renderer) const
{
	m_test_triangle->Draw(_renderer);
}
