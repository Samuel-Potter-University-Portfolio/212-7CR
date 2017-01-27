#pragma once

#include <GLFW/glfw3.h>
#include <vec2.hpp>

class Window
{
private:
	GLFWwindow* window;
	glm::ivec2 desired_resolution;

public:
	Window(const glm::ivec2 desired_resolution);
	~Window();

	void Launch();

private:
	void LaunchMainLoop();
	void Tick(const float delta_time);
};

