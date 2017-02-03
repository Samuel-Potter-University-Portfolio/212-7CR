#pragma once

#include <glm.hpp>
#include <GL/glew.h>
#include <GLFW\glfw3.h>


class Mouse
{
private:
	int button_states[GLFW_MOUSE_BUTTON_LAST + 1]{ GLFW_RELEASE };

	class Window* window;
	GLFWwindow* glfw_window;

	glm::vec2 location;
	glm::vec2 velocity;
	glm::vec2 current_velocity;
	bool locked = false;

public:

	void Register(class Window* window);
	void Update();

	void OnMouseMove(float x, float y);
	void OnButton(int button, int action, int mods);

	void Lock();
	void Unlock();

	inline bool IsLocked() { return locked; }
	inline int GetButtonState(const int button) { return button_states[button]; }

	inline glm::vec2 GetLocation() { return location; }
	inline glm::vec2 GetVelocity() { return velocity; }
	glm::vec2 GetScaledVelocity();
};

