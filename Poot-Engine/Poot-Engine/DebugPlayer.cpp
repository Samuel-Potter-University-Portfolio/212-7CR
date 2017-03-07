#include "DebugPlayer.h"
#include "Game.h"
#include "CameraComponent.h"

DebugPlayer::DebugPlayer()
{
	SetTags(OBJ_TAG_PLAYER);
	MakeComponent<CameraComponent>();
}
#include "Logger.h"
void DebugPlayer::Tick(float delta_time) 
{
	Super::Tick(delta_time);

	Keyboard& keyboard = g_game->GetWindow()->GetKeyboard();
	const float speed = 15.0f * delta_time;
	
	if (keyboard.GetKeyState(GLFW_KEY_SPACE))
		local_transform.location += glm::vec3(0, 1, 0) * speed;
	if (keyboard.GetKeyState(GLFW_KEY_LEFT_CONTROL))
		local_transform.location -= glm::vec3(0, 1, 0) * speed;

	if (keyboard.GetKeyState(GLFW_KEY_W))
		local_transform.location += local_transform.GetForward() * speed;
	if (keyboard.GetKeyState(GLFW_KEY_S))
		local_transform.location -= local_transform.GetForward() * speed;

	if (keyboard.GetKeyState(GLFW_KEY_D))
		local_transform.location += local_transform.GetXZRight() * speed;
	if (keyboard.GetKeyState(GLFW_KEY_A))
		local_transform.location -= local_transform.GetXZRight() * speed;

	Mouse& mouse = g_game->GetWindow()->GetMouse();

	if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_LEFT))
		mouse.Lock();

	else if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_RIGHT) || keyboard.GetKeyState(GLFW_KEY_ESCAPE))
		mouse.Unlock();

	if (mouse.IsLocked())
	{
		const glm::vec2 mouse_input = mouse.GetVelocity()* 20.0f;
		local_transform.rotation += glm::vec3(0.0, -2.0, 0.0) * mouse_input.x;
		local_transform.rotation += glm::vec3(2.0, 0.0, 0.0) * mouse_input.y;

		//LOG(Log, "%f, %f, %f", local_transform.rotation.x, local_transform.rotation.y, local_transform.rotation.z);

		if (local_transform.rotation.x > 89.0f)
			local_transform.rotation.x = 89.0f;

		if (local_transform.rotation.x < -89.0f)
			local_transform.rotation.x = -89.0f;
	}
}
