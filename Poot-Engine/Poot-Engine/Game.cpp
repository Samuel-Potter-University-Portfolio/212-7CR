#include "Game.h"

#include "Logger.h"


class Game* g_game = nullptr;


Game::Game(const std::string game_name, const glm::ivec2 desired_resolution, World* world)
	: game_name(game_name), world(world)
{
	//Set default values
	logic_ready = false;
	window_ready = false;
	closed_requested = false;

	//Check that only one Game exists at one time
	if (!g_game)
		g_game = this;
	else
		LOG(Fatal, "Cannot setup two Game instances at a time");


	if (world == nullptr)
		LOG(Warning, "Initializing game with null world");


	LOG(Log, "Setting up game '%s'", game_name.c_str());
	LOG(Log, "Using Poot Engine %i.%i.%i", POOT_VERSION_MAJOR, POOT_VERSION_MINOR, POOT_VERSION_PATCH);
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
	if (world)
	{
		if (!world->IsDestroyed())
			LOG(Error, "World was not destroyed correctly");

		delete world;
		world = nullptr;
		LOG(Log, "World cleaned up")
	}
}

