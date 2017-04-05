#include "Mouse.h"
#include "InputComponent.h"
#include "Window.h"
#include "Logger.h"


Mouse* main_mouse = nullptr;


void Mouse::Register(class Window* window)
{
	this->window = window;
	glfw_window = window->GetGLFWwindow();

	if (!glfw_window)
	{
		LOG(Error, "Cannot setup mouse with invalid glfw window");
		return;
	}

	if (main_mouse)
	{
		LOG(Warning, "Cannot setup multiple mouses at the same time");
		return;
	}

	main_mouse = this;
	is_locked = false;

	glfwSetCursorPosCallback(glfw_window,
		[](GLFWwindow* window, double x, double y)
		{
			main_mouse->OnMouseMove(x, y);
		}
	);

	glfwSetMouseButtonCallback(glfw_window,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			main_mouse->OnButton(button, action, mods);
		}
	);

	LOG(Log, "Mouse setup");
}

void Mouse::HandleNewComponent(Component* component) 
{
	InputComponent* input = Cast<InputComponent>(component);

	if (input)
		input_listeners.push_back(input);
}

void Mouse::OnMouseMove(float x, float y) 
{
	if (is_locked)
	{
		float width = window->GetWidth();
		float height = window->GetHeight();
		width = (width != 0 ? width : 1.0f);
		height = (height != 0 ? height : 1.0f);

		x -= width / 2.0f;
		y -= height / 2.0f;


		glfwSetCursorPos(glfw_window,
			width / 2.0f,
			height / 2.0f
		);
	}

	mouse_location.x = x;
	mouse_location.y = y;

	for (InputComponent* listener : input_listeners)
		listener->OnMouseMove(x, y, is_locked);
}

void Mouse::OnButton(int button, int action, int mods) 
{
	for (InputComponent* listener : input_listeners)
		listener->OnKey(button, action, mods);
}

void Mouse::Lock() 
{
	is_locked = true;
	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Mouse::Unlock() 
{
	is_locked = false;
	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}