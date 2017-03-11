#pragma once
#include "Component.h"
#include "Mouse.h"

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <functional>
#include <vector>


#define GLFW_MOUSE_LOCATION_X	GLFW_MOUSE_BUTTON_LAST + 1
#define GLFW_MOUSE_LOCATION_Y	GLFW_MOUSE_LOCATION_X + 1
#define INPUT_KEY_LAST			GLFW_KEY_LAST


struct POOT_API InputAxis
{
private:
	bool positive[INPUT_KEY_LAST + 1]{ false };
	bool negative[INPUT_KEY_LAST + 1]{ false };

	int input_state = 0;
	float mouse_input = 0.0f;

	float axis_value = 0.0f;

public:
	std::function<void(float)> func;

	bool need_mouse_lock = true;
	float mouse_sensitivity = 1.0f;
	float acceleration = 1.0f;
	float decay = 0.35f;
	float Consume();


	void OnKey(int key, int action, int mods, int scan_code);
	void OnMouseMove(float x, float y);

	inline void AddPositive(int key) { positive[key] = true; }
	inline void AddNegative(int key) { negative[key] = true; }
};


struct POOT_API InputEvent
{
private:
	bool last_state = false;
	bool current_state = false;
	bool keys[INPUT_KEY_LAST + 1]{ false };

public:
	bool need_mouse_lock = true;
	bool is_spammable = false;

	inline void AddInput(int key) { keys[key] = true; }
	bool Consume(bool& state);

	void OnKey(int key, int action, int mods, int scan_code);
	void OnMouseMove(float x, float y);

	std::function<void(bool)> func;
};


class POOT_API InputComponent : public Component
{
private:
	std::vector<InputAxis> axes;
	std::vector<InputEvent> events;

public:
	InputComponent();
	~InputComponent();

	void OnMouseMove(float x, float y, bool grabbed);
	void OnKey(int key, int action, int mods, int scan_code = 0);

	inline void AddAxis(InputAxis& axis) { axes.push_back(axis); }
	inline void AddEvent(InputEvent& event) { events.push_back(event); }

	void GrabMouse();
	void ReleaseMouse();

protected:
	virtual void Tick(float delta_time) override;
};

