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

	//Required for VAOs
	glewExperimental = GL_TRUE;

	glfwSetErrorCallback(
		[](int error, const char* desc)
		{
			//TODO - Create actual method/handle
			LOG(Error, "GLFW(%i): '%s'", error, desc);
		}
	);

	if (!glfwInit())
	{
		LOG(Fatal, "Failed to init GLFW");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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
	else
		glfwMakeContextCurrent(window);

	//Check for errors
	GLenum error_type = glewInit();

	if (error_type != GLEW_OK)
		LOG(Error, "GLEW(%i)", error_type);

	//Log vendor info
	LOG(Log, "GL Vendor:");
	LOG(Log, "\t-GL_RENDERER: \t%s", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	LOG(Log, "\t-GL_VERSION: \t%s", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	LOG(Log, "\t-GL_VENDOR: \t%s", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));


	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetVSync(true);

	keyboard.Register(this);
	mouse.Register(this);
	g_game->SetWindowReady();
	LaunchMainLoop();
}

void Window::SetVSync(const bool on) 
{
	LOG(Log, "Vsync: %s", on ? "on" : "off");
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
		glfwPollEvents();

		//Tick logic
		if(g_game->IsReady())
			Tick(delta_time);

		//Swap buffers i.e. Draw
		glfwSwapBuffers(window);

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

	//Correct aspect ratio
	glfwGetFramebufferSize(window, &width, &height);
	aspect_ratio = (float)(width) / (float)(height);
	glViewport(0, 0, width, height);

	//Tick world
	World* world = g_game->GetWorld();
	if (world)
		world->WindowTick(this, delta_time);
}

void Window::CleanUp() 
{
	model_loader.CleanUp();
	shader_loader.CleanUp();
	texture_loader.CleanUp();

	//Clean up current world
	World* world = g_game->GetWorld();
	if (world)
		world->UnloadWindowResources(this);

	LOG(Log, "Terminating GLFW");
	glfwTerminate();
}