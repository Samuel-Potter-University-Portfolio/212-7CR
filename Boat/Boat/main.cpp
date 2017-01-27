#include "Game.h"
#include "Logger.h"

int main(void)
{
	Game game("Boat Game", glm::ivec2(640, 480));
	game.Launch();

	LOG(Log, "Exiting(0)");
	while (true) {};
	return 0;
}