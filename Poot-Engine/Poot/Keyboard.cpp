#include "Keyboard.h"
#include "Window.h"
#include "Logger.h"


Keyboard* active_keyboard;


void Keyboard::Register(Window* window)
{
	GLFWwindow* glfw_window = window->GetGLFWwindow();
	
	if (!active_keyboard)
		active_keyboard = this;

	else
	{
		LOG(Warning, "Cannot have 2 active keyboards at once")
		return;
	}

	glfwSetKeyCallback(glfw_window, 
		[](GLFWwindow* window, int key, int scan_code, int action, int mods){
			if (active_keyboard)
				active_keyboard->OnKey(key, scan_code, action, mods);
		}
	);

	LOG(Log, "Keyboard setup");
}

void Keyboard::OnKey(int key, int scan_code, int action, int mods) 
{
	if (key >= 0 && key <= GLFW_KEY_LAST)
		key_states[key] = action;
}