#include <Game.h>
#include <Logger.h>

#include "PoolWorld.h"

void Launch() 
{
	Game* game = new Game("Pool Game", glm::ivec2(640, 480), new PoolWorld());
	game->Launch();
	delete game;
	LOG(Log, "Exiting(0)");
}

int main(int arg_c, char** arg_v)
{
	Launch();

#ifdef _DEBUG
	//Do not close log manually in debug
	while (true) {};
#endif
	return 0;
}