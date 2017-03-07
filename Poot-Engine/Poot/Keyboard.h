#pragma once
#include "API.h"
#include <GL/glew.h>
#include <GLFW\glfw3.h>

class POOT_API Keyboard
{
private:
	int key_states[GLFW_KEY_LAST + 1]{GLFW_RELEASE};

public:
	void Register(class Window* window);
	void OnKey(int key, int scan_code, int action, int mods);

	inline int GetKeyState(const int key) { return key_states[key]; }
};

