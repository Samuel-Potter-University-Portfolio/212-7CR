#include "GameLogic.h"
#include "Logger.h"
#include "Misc.h"
#include "Game.h"

#include <chrono>
#include <time.h>


GameLogic::GameLogic()
	: tick_rate(60)
{
}


GameLogic::~GameLogic()
{
	if (thread)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}
}

void GameLogic::Launch() 
{
	LOG(Log, "Launching GameLogic thread");
	thread = new std::thread(&GameLogic::LaunchMainLoop, this);
}

inline float GetTime()
{
	return System::GetSysTimeSeconds() + System::GetSysTimeMilli() / 1000.0f;
}

void GameLogic::LaunchMainLoop()
{
	const float total_sleep_time = 1.0f / (float)(tick_rate);
	float sleep_time = 0;
	g_game->SetLogicReady();

	float last_tick_time = GetTime();
	float delta_time = 0;

	while (!g_game->IsClosedRequested())
	{
		//Tick logic
		if(g_game->IsReady())
			Tick(delta_time);


		//Calculate corrected delta time
		float current_time = GetTime();

		delta_time = current_time - last_tick_time;
		if (delta_time < 0.0f) //Minute ticked over
			delta_time += 60.0f;

		last_tick_time = current_time;


		//Sleep for desired tick_rate
		sleep_time += total_sleep_time * 2.0f - delta_time;
		
		if (sleep_time > 0)
		{
			int actual_sleep_time = (int)(sleep_time);
			std::this_thread::sleep_for(std::chrono::milliseconds(actual_sleep_time * 1000));
			sleep_time -= actual_sleep_time;
		}
		

	}

	//Ensure other threads have registered close request
	g_game->Close();

	LOG(Log, "Terminating GameLogic");
}

void GameLogic::Tick(float delta_time)
{
	//Track ticks this second, for debug
	second_counter += delta_time;
	ticks_this_second++;

	if (second_counter > 1.0f)
	{
		second_counter -= 1.0f;
		ticks_last_second = ticks_this_second;
		ticks_this_second = 0;
	}
}