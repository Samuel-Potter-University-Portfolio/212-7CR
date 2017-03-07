#include "Game.h"
#include "Logger.h"


int main(int arg_c, char** arg_v)
{
	Game* game = new Game("Boat Game", glm::ivec2(640, 480), new World());
	game->Launch();
	delete game;

	LOG(Log, "Exiting(0)");

#ifdef _DEBUG
	//Do not close log manually in debug
	while (true) {};
#endif
	return 0;
}