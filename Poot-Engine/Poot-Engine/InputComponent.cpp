#include "InputComponent.h"
#include <math.h>
#include "Game.h"

#define CLAMP(x, min, max) (x > max ? max : x < min ? min : x)


void InputAxis::OnKey(int key, int action, int mods, int scan_code)
{
	if (action == GLFW_PRESS)
	{
		if (positive[key])
			input_state++;
		else if (negative[key])
			input_state--;
	}
	else if (action == GLFW_RELEASE)
	{
		if (positive[key])
			input_state--;
		else if (negative[key])
			input_state++;
	}
}

void InputAxis::OnMouseMove(float x, float y) 
{
	if (positive[GLFW_MOUSE_LOCATION_X])
		mouse_input += x;
	else if (positive[GLFW_MOUSE_LOCATION_Y])
		mouse_input += y;

	if (negative[GLFW_MOUSE_LOCATION_X])
		mouse_input -= x;
	else if (negative[GLFW_MOUSE_LOCATION_Y])
		mouse_input -= y;
}

float InputAxis::Consume()
{
	if (input_state == 0)
	{
		axis_value *= (1.0f - decay);
	}
	else
	{
		const float consume_value = 1.0f * (input_state > 0 ? 1 : -1);
		axis_value += CLAMP(consume_value, -1.0f, 1.0f) * acceleration;
	}

	axis_value = CLAMP(axis_value, -1.0f, 1.0f) + mouse_input * mouse_sensitivity;
	mouse_input = 0.0f;
	return axis_value;
}

void InputEvent::OnKey(int key, int action, int mods, int scan_code)
{
	if(keys[key])
		current_state = (action != GLFW_RELEASE);
}

void InputEvent::OnMouseMove(float x, float y)
{
	if (keys[GLFW_MOUSE_LOCATION_X])
		current_state = x * x > 1.0f;

	if (keys[GLFW_MOUSE_LOCATION_Y])
		current_state = y * y > 1.0f;
}

bool InputEvent::Consume(bool& state) 
{
	bool value = false;

	if (current_state != last_state)
		value = true;

	state = current_state;
	last_state = current_state;
	return value;
}

InputComponent::InputComponent()
{
}


InputComponent::~InputComponent()
{
}

void InputComponent::Tick(float delta_time) 
{
	//Handle axes
	for (InputAxis& axis : axes)
		axis.func(axis.Consume());


	//Handle events
	for (InputEvent& event : events)
	{
		bool pressed = false;
		if (event.Consume(pressed) || (pressed && event.is_spammable))
			event.func(pressed);
	}
}

void InputComponent::OnMouseMove(float x, float y, bool grabbed)
{
	for (InputAxis& axis : axes)
		if(!axis.need_mouse_lock || grabbed)
			axis.OnMouseMove(x, y);

	for (InputEvent& event : events)
		if (!event.need_mouse_lock || grabbed)
			event.OnMouseMove(x, y);
}

void InputComponent::OnKey(int key, int action, int mods, int scan_code) 
{
	for (InputAxis& axis : axes)
		axis.OnKey(key, action, mods, scan_code);

	for (InputEvent& event : events)
		event.OnKey(key, action, mods, scan_code);
}

void InputComponent::GrabMouse() 
{
	Mouse& mouse = g_game->GetWindow()->GetMouse();

	if (!mouse.IsLocked())
		mouse.Lock();
}

void InputComponent::ReleaseMouse() 
{
	Mouse& mouse = g_game->GetWindow()->GetMouse();

	if (mouse.IsLocked())
		mouse.Unlock();
}