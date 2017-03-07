#include "Mouse.h"
#include "Window.h"
#include "Logger.h"


Mouse* active_mouse;


void Mouse::Register(Window* window) 
{
	this->window = window;
	glfw_window = window->GetGLFWwindow();

	if (!active_mouse)
		active_mouse = this;

	else
	{
		LOG(Warning, "Cannot have 2 active mouses at once")
		return;
	}

	glfwSetCursorPosCallback(glfw_window, 
		[](GLFWwindow* window, double x, double y)
		{
			if (active_mouse)
				active_mouse->OnMouseMove(x, y);
		}
	);

	glfwSetMouseButtonCallback(glfw_window,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			if (active_mouse)
				active_mouse->OnButton(button, action, mods);
		}
	);

	LOG(Log, "Mouse setup");
}

void Mouse::OnButton(int button, int action, int mods) 
{
	if (button >= 0 && button <= GLFW_KEY_LAST)
		button_states[button] = action;
}

void Mouse::OnMouseMove(float x, float y) 
{
	float width = window->GetWidth();
	float height = window->GetHeight();

	width = width ? width : 1;
	height = height ? height : 1;

	location.x = (x / width - 0.5f) * 2.0f;
	location.y = (0.5f - (y / height)) * 2.0f;
	current_velocity += location;

	if (locked)
		glfwSetCursorPos(glfw_window,
			width / 2.0f,
			height / 2.0f
	);
}

glm::vec2 Mouse::GetScaledVelocity() 
{ 
	return velocity * glm::vec2(window->GetWidth(), window->GetHeight()); 
}

void Mouse::Update()
{
	velocity = current_velocity;
	current_velocity *= 0.0f;
}

void Mouse::Lock() 
{
	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	locked = true;
}

void Mouse::Unlock() 
{
	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	locked = false;
}