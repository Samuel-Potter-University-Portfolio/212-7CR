#pragma once

#include <thread>


class GameLogic
{
private:
	std::thread* thread;
	const int tick_rate;

	//Variables to keep track of current tick rate (debug logging)
	float second_counter = 0;
	int ticks_this_second = 0;
	int ticks_last_second = 0;

public:
	GameLogic();
	~GameLogic();

	void Launch();

	inline int GetCurrentTickRate() { return ticks_last_second; }

private:
	void LaunchMainLoop();
	void Tick(float delta_time);
};

