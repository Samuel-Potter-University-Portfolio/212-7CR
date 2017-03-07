#include "PoolWorld.h"
#include <Sun.h>
#include <SkyBox.h>
#include <DebugPlayer.h>

#include "BasicBox.h"

PoolWorld::PoolWorld()
{
}


void PoolWorld::LoadLogicResources(GameLogic* game_logic)
{
	Super::LoadLogicResources(game_logic);

	AddEntity(new DebugPlayer);
	AddEntity(new SkyBox);

	Sun* sun = new Sun;
	sun->directional_light->SetDirection(glm::vec3(0, -1, 0));
	AddEntity(sun);


	AddEntity(new BasicBox);
}

void PoolWorld::LoadWindowResources(Window* window)
{
	Super::LoadWindowResources(window);
}

void PoolWorld::UnloadLogicResources(GameLogic* game_logic)
{
	Super::UnloadLogicResources(game_logic);
}

void PoolWorld::UnloadWindowResources(Window* window)
{
	Super::UnloadWindowResources(window);
}