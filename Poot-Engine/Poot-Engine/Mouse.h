#pragma once
#include "API.h"

#include <glm.hpp>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <vector>


class POOT_API Mouse 
{
private:
	class Window* window;
	GLFWwindow* glfw_window;

	bool is_locked;
	glm::vec2 mouse_location;

	std::vector<class InputComponent*> input_listeners;

public:
	void Register(class Window* window);
	void HandleNewComponent(class Component* component);

	void OnMouseMove(float x, float y);
	void OnButton(int button, int action, int mods);

	void Lock();
	void Unlock();
	inline bool IsLocked() { return is_locked; }

	inline glm::vec2 GetLocation() { return mouse_location; }
};
