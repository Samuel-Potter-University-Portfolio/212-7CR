#pragma once
#include "API.h"

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <vector>


class POOT_API Keyboard
{
private:
	std::vector<class InputComponent*> input_listeners;

public:
	void Register(class Window* window);
	void HandleNewComponent(class Component* component);

	void OnKey(int key, int action, int mods, int scan_code);
};
