#pragma once
#include "World.h"

class BoatWorld : public World
{
public:
	BoatWorld();

	virtual void LoadLogicResources(class GameLogic* game_logic);
	virtual void LoadWindowResources(class Window* window);
	virtual void UnloadLogicResources(class GameLogic* game_logic);
	virtual void UnloadWindowResources(class Window* window);
};

