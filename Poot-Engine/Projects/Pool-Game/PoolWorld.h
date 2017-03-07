#pragma once
#include <World.h>

class PoolWorld : public World
{
public:
	PoolWorld();

	virtual void LoadLogicResources(class GameLogic* game_logic);
	virtual void LoadWindowResources(class Window* window);
	virtual void UnloadLogicResources(class GameLogic* game_logic);
	virtual void UnloadWindowResources(class Window* window);
};

