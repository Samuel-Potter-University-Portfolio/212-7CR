#pragma once
#include <World.h>

#define POBJ_TAG_BALL (OBJ_TAG_LAST << 1)
#define POBJ_TAG_TABLE (OBJ_TAG_LAST << 2)
#define POBJ_TAG_FLOOR (OBJ_TAG_LAST << 3)

#define POBJ_TAG_LAST POBJ_TAG_FLOOR

class PoolWorld : public World
{
public:
	PoolWorld();

	virtual void LoadLogicResources(class GameLogic* game_logic);
	virtual void LoadWindowResources(class Window* window);
	virtual void UnloadLogicResources(class GameLogic* game_logic);
	virtual void UnloadWindowResources(class Window* window);
};

