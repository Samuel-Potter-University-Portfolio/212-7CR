#include "Keyboard.h"
#include "Window.h"
#include "InputComponent.h"
#include "Logger.h"


Keyboard* main_keyboard = nullptr;


void Keyboard::Register(Window* window)
{
	GLFWwindow* glfw_window = window->GetGLFWwindow();

	if (!glfw_window)
	{
		LOG(Error, "Cannot setup mouse with invalid glfw window");
		return;
	}

	if (main_keyboard)
	{
		LOG(Warning, "Cannot setup multiple keyboards at the same time");
		return;
	}

	main_keyboard = this;

	glfwSetKeyCallback(glfw_window,
		[](GLFWwindow* window, int key, int scan_code, int action, int mods) 
		{
			main_keyboard->OnKey(key, action, mods, scan_code);
		}
	);

	LOG(Log, "Keyboard setup");
}

void Keyboard::HandleNewComponent(Component* component)
{
	InputComponent* input = Cast<InputComponent>(component);

	if (input)
		input_listeners.push_back(input);
}

void Keyboard::OnKey(int key, int action, int mods, int scan_code)
{
	for (InputComponent* listener : input_listeners)
		listener->OnKey(key, action, mods, scan_code);
}