#include "BoatWorld.h"
#include "TestEnt.h"
#include "Logger.h"

BoatWorld::BoatWorld()
{
}

void BoatWorld::LoadLogicResources(GameLogic* game_logic) 
{
	World::LoadLogicResources(game_logic);
	AddEntity(new TestEnt());

	TestEnt* e = new TestEnt();
	e->transform.location = glm::vec3(0, -15, 0);
	AddEntity(e);
}

void BoatWorld::LoadWindowResources(Window* window) 
{
	World::LoadWindowResources(window);
}

void BoatWorld::UnloadLogicResources(GameLogic* game_logic) 
{
	World::UnloadLogicResources(game_logic);
}

void BoatWorld::UnloadWindowResources(Window* window) 
{
	World::UnloadWindowResources(window);
}