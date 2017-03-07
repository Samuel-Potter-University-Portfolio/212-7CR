#include "DebugPlayer.h"
#include "Game.h"
#include "CameraComponent.h"

DebugPlayer::DebugPlayer()
{
	tags |= E_TAG_PLAYER;

	MakeComponent<CameraComponent>();
}

void DebugPlayer::LogicTick(float delta_time)
{
	Super::LogicTick(delta_time);

	Keyboard& keyboard = g_game->GetWindow()->GetKeyboard();
	const float speed = 15.0f * delta_time;

	if (keyboard.GetKeyState(GLFW_KEY_SPACE))
		transform.location += glm::vec3(0, 1, 0) * speed;
	if (keyboard.GetKeyState(GLFW_KEY_LEFT_CONTROL))
		transform.location -= glm::vec3(0, 1, 0) * speed;

	if (keyboard.GetKeyState(GLFW_KEY_W))
		transform.location += transform.GetForward() * speed;
	if (keyboard.GetKeyState(GLFW_KEY_S))
		transform.location -= transform.GetForward() * speed;

	if (keyboard.GetKeyState(GLFW_KEY_D))
		transform.location += transform.GetXZRight() * speed;
	if (keyboard.GetKeyState(GLFW_KEY_A))
		transform.location -= transform.GetXZRight() * speed;
}

void DebugPlayer::WindowTick(float delta_time)
{
	Super::WindowTick(delta_time);

	Keyboard& keyboard = g_game->GetWindow()->GetKeyboard();
	Mouse& mouse = g_game->GetWindow()->GetMouse();

	if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_LEFT))
		mouse.Lock();

	else if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_RIGHT) || keyboard.GetKeyState(GLFW_KEY_ESCAPE))
		mouse.Unlock();

	if (mouse.IsLocked())
	{
		const glm::vec2 mouse_input = mouse.GetVelocity()* 20.0f;
		transform.rotation += glm::vec3(0.0, -2.0, 0.0) * mouse_input.x;
		transform.rotation += glm::vec3(2.0, 0.0, 0.0) * mouse_input.y;

		if (transform.rotation.x > 89.0f)
			transform.rotation.x = 89.0f;

		if (transform.rotation.x < -89.0f)
			transform.rotation.x = -89.0f;
	}
}