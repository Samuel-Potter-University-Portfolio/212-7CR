#include "Game.h"

#include "Logger.h"


class Game* g_game = nullptr;


Game::Game(const std::string game_name, const glm::ivec2 desired_resolution)
	: game_name(game_name)
{
	if (!g_game)
		g_game = this;

	else
	{
		LOG(Fatal, "Cannot setup two Game instances at a time");
	}


	LOG(Log, "Setting up game '%s'", game_name.c_str());
	window = new Window(desired_resolution);
}


Game::~Game()
{
	if (window)
	{
		delete window;
		window = nullptr;
	}
}


void Game::Launch() 
{
	window->Launch();
}