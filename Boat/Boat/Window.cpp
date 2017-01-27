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


Window::~Window()
{
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

	glfwSwapInterval(0); //Vsync
	glfwMakeContextCurrent(window);
	LaunchMainLoop();
}

float GetTime() 
{
	return System::GetSysTimeSeconds() + System::GetSysTimeMilli() / 1000.0f;
}


void Window::LaunchMainLoop()
{
	float last_tick_time = GetTime();
	float delta_time = 0;

	while (!glfwWindowShouldClose(window))
	{
		Tick(delta_time);
		glfwSwapBuffers(window);
		glfwPollEvents();

		//Time ticks
		float current_time = GetTime();

		delta_time = current_time - last_tick_time;
		if(delta_time < 0.0f) //Minute ticked over
			delta_time += 60.0f;

		last_tick_time = current_time;
	}

	LOG(Log, "Terminating GLFW");
	glfwTerminate();
}

void Window::Tick(const float delta_time) 
{
}