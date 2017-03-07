#pragma once
#include "API.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vec2.hpp>

#include "ModelLoader.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "Keyboard.h"
#include "Mouse.h"

class POOT_API Window
{
private:
	GLFWwindow* window;

	ModelLoader model_loader;
	ShaderLoader shader_loader;
	TextureLoader texture_loader;

	Keyboard keyboard;
	Mouse mouse;

	//Variables to keep track of current tick rate (debug logging)
	float second_counter = 0;
	int ticks_this_second = 0;
	int ticks_last_second = 0;

	//Window settings
	glm::ivec2 desired_resolution;
	int width = 0;
	int height = 0;
	float aspect_ratio = 1.0f;

public:
	Window(const glm::ivec2 desired_resolution);

	void Launch();
	void SetVSync(const bool on);

	inline int GetCurrentTickRate() { return ticks_last_second; }

	inline ModelLoader& GetModelLoader() { return model_loader; }
	inline ShaderLoader& GetShaderLoader() { return shader_loader; }
	inline TextureLoader& GetTextureLoader() { return texture_loader; }

	inline Keyboard& GetKeyboard() { return keyboard; }
	inline Mouse& GetMouse() { return mouse; }

	inline GLFWwindow* GetGLFWwindow() { return window; }
	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
	inline float GetAspectRatio() { return aspect_ratio; }

private:
	void LaunchMainLoop();
	void Tick(float delta_time);

	void CleanUp();

};

