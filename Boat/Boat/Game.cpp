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
	game_logic = new GameLogic;
	window = new Window(desired_resolution);
}


Game::~Game()
{
	CleanUp();
}

void Game::SetLogicReady() 
{ 
	logic_ready = true; 
	LOG(Log, "Logic thread ready");
}

void Game::SetWindowReady() 
{ 
	window_ready = true; 
	LOG(Log, "Window thread ready");
}


void Game::Launch() 
{
	game_logic->Launch();
	window->Launch();

	//When execution returns here, must be exiting, so clean up
	CleanUp();
}

void Game::CleanUp() 
{
	if (game_logic)
	{
		delete game_logic;
		game_logic = nullptr;
	}
	if (window)
	{
		delete window;
		window = nullptr;
	}
}