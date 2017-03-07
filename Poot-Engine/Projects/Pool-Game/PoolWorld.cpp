#include "PoolWorld.h"
#include <Logger.h>
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

	AddObject(new DebugPlayer);
	AddObject(new SkyBox);

	Sun* sun = new Sun(glm::vec3(0, -1, 0));
	LOG(Log, "C");
	//sun->directional_light->SetDirection(glm::vec3(0, -1, 0));
	AddObject(sun);


	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			for (int z = -1; z <= 1; z++)
			{
				BasicBox* box = new BasicBox;
				box->local_transform.location = glm::vec3(x, y, z) * 5.0f;
				AddObject(box);
			}
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