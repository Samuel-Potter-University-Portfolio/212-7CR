#pragma once
#include "API.h"

#include <thread>


class POOT_API GameLogic
{
private:
	std::thread* thread;
	float last_tick_end_time;

	//Variables to keep track of current tick rate (debug logging)
	float second_counter = 0;
	int ticks_this_second = 0;
	int ticks_last_second = 0;

public:
	const int tick_rate;
	const float total_sleep_time;
	float last_sleep_time;

	GameLogic();
	~GameLogic();

	void Launch();

	float GetNormalizedTickTime();
	inline int GetCurrentTickRate() { return ticks_last_second; }

private:
	void LaunchMainLoop();
	void Tick(float delta_time);
};

