#include "Window.h"

#include "Game.h"
#include "Logger.h"
#include "Misc.h"


Window::Window(const glm::ivec2 desired_resolution)
	: desired_resolution(desired_resolution)
{
	LOG(Log, "Setting up Window");
	LOG(Log, "\t-Desired Resolution:\t(%i,%i)", desired_resolution.x, desired_resolution.y);
}

void Window::Launch() 
{
	LOG(Log, "Launching GLFW Window");

	if (!glfwInit())
	{
		LOG(Fatal, "Failed to init GLFW");
		return;
	}

	window = glfwCreateWindow(
		desired_resolution.x, desired_resolution.y,
		g_game ? g_game->GetName().c_str() : "Untitled", 
		nullptr, nullptr
	);

	if (!window)
	{
		glfwTerminate();
		LOG(Fatal, "Failed to create GLFW Window");
		return;
	}

	SetVSync(false);
	glfwMakeContextCurrent(window);
	g_game->SetWindowReady();
	LaunchMainLoop();
}

void Window::SetVSync(const bool on) 
{
	glfwSwapInterval(on);
}

inline float GetTime()
{
	return System::GetSysTimeSeconds() + System::GetSysTimeMilli() / 1000.0f;
}

void Window::LaunchMainLoop()
{
	float last_tick_time = GetTime();
	float delta_time = 0;

	while (!glfwWindowShouldClose(window))
	{
		//Tick logic
		if(g_game->IsReady())
			Tick(delta_time);

		//Swap buffers i.e. Draw
		glfwSwapBuffers(window);
		glfwPollEvents();

		//Calculate corrected delta time
		float current_time = GetTime();

		delta_time = current_time - last_tick_time;
		if(delta_time < 0.0f) //Minute ticked over
			delta_time += 60.0f;

		last_tick_time = current_time;


		//Close window, if requested internally by game
		if (g_game->IsClosedRequested())
			glfwSetWindowShouldClose(window, true);
	}

	//Ensure game has internally registered close request
	g_game->Close();

	LOG(Log, "Terminating Window");
	CleanUp();
}

void Window::Tick(float delta_time)
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

void Window::CleanUp() 
{
	model_loader.CleanUp();

	LOG(Log, "Terminating GLFW");
	glfwTerminate();
}