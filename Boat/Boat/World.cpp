#include "World.h"
#include "Game.h"
#include "Logger.h"

World::~World() 
{
	for (Entity* entity : entities)
		delete entity;
}

void World::LogicBegin(GameLogic* game_logic) 
{
	LoadLogicResources(game_logic);
	logic_begun = true;
	LOG(Log, "World logic ready");
}

void World::WindowBegin(Window* window)
{
	//Let logic load first to ensure all resources are ready
	if (!logic_begun)
		return;

	LoadWindowResources(window);
	window_begun = true;
	LOG(Log, "World window ready");
}

void World::LogicTick(GameLogic* game_logic, float delta_time)
{
	//Handle, if world if currently being destroyed
	if(is_being_destroyed)
	{
		if (!logic_destroyed)
		{
			UnloadLogicResources(game_logic);
			LOG(Log, "World logic destroyed");
		}
		return;
	}

	//Handle world's first tick i.e. needs to be loaded in
	if (!logic_begun) 
	{
		LogicBegin(game_logic);
		return;
	}

	//Wait until both threads have been loaded
	if (!HasLoaded())
		return;

	//Tick entities
	for (Entity* entity : entities)
		entity->HandleLogicTick(delta_time);
}

void World::WindowTick(Window* window, float delta_time)
{
	//Handle, if world if currently being destroyed
	if (is_being_destroyed)
	{
		if (!window_destroyed)
		{
			UnloadWindowResources(window);
			LOG(Log, "World window destroyed");
		}
		return;
	}

	//Handle world's first tick i.e. needs to be loaded in
	if (!window_begun)
	{
		WindowBegin(window);
		return;
	}

	//Wait until both threads have been loaded
	if (!HasLoaded())
		return;
	
	//Tick entities
	for (Entity* entity : entities)
		entity->HandleWindowTick(delta_time);

	if (renderer)
		renderer->Render();
}

void World::LoadLogicResources(GameLogic* game_logic) 
{
}

void World::LoadWindowResources(Window* window)
{
	renderer = new Renderer;

	//Added already existing entities
	for(Entity* entity : entities)
		renderer->AddEntityToQueue(entity);
}

void World::UnloadLogicResources(GameLogic* game_logic)
{
	for (Entity* entity : entities)
		entity->LogicDestroy();

	logic_destroyed = true;
}

void World::UnloadWindowResources(Window* window) 
{
	for (Entity* entity : entities)
		entity->WindowDestroy();

	if (renderer)
	{
		renderer->CleanUp();
		delete renderer;
		renderer = nullptr;
	}

	window_destroyed = true;
}

void World::AddEntity(Entity* entity) 
{
	if (!entity)
		return;

	entities.push_back(entity);

	if(renderer)
		renderer->AddEntityToQueue(entity);
}