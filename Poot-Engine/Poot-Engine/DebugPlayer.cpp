#include "DebugPlayer.h"
#include "CameraComponent.h"

DebugPlayer::DebugPlayer()
{
	SetTags(OBJ_TAG_PLAYER);
	MakeComponent<CameraComponent>();

	input_component = MakeComponent<InputComponent>();
}

void DebugPlayer::BuildComponents() 
{
	//Register forward movement
	{
		InputAxis axis;
		axis.AddPositive(GLFW_KEY_W);
		axis.AddPositive(GLFW_KEY_UP);
		axis.AddNegative(GLFW_KEY_S);
		axis.AddNegative(GLFW_KEY_DOWN);
		axis.func = [this](float a) { OnForward(a); };

		input_component->AddAxis(axis);
	}
	//Register strafe movement
	{
		InputAxis axis;
		axis.AddPositive(GLFW_KEY_D);
		axis.AddPositive(GLFW_KEY_RIGHT);
		axis.AddNegative(GLFW_KEY_A);
		axis.AddNegative(GLFW_KEY_LEFT);
		axis.func = [this](float a) { OnStrafe(a); };

		input_component->AddAxis(axis);
	}
	//Register up down
	{
		InputEvent jump_event;
		jump_event.AddInput(GLFW_KEY_SPACE);
		jump_event.is_spammable = true;
		jump_event.func = [this](bool pressed) { OnJump(pressed); };
		input_component->AddEvent(jump_event);

		InputEvent crouch_event;
		crouch_event.AddInput(GLFW_KEY_LEFT_CONTROL);
		crouch_event.is_spammable = true;
		crouch_event.func = [this](bool pressed) { OnCrouch(pressed); };
		input_component->AddEvent(crouch_event);
	}
	//Register mouse grab
	{
		InputEvent grab_event;
		grab_event.AddInput(GLFW_MOUSE_BUTTON_LEFT);
		grab_event.func = [this](bool pressed) { OnGrabMouse(pressed); };
		input_component->AddEvent(grab_event);

		InputEvent release_event;
		release_event.AddInput(GLFW_MOUSE_BUTTON_RIGHT);
		release_event.AddInput(GLFW_KEY_ESCAPE);
		release_event.func = [this](bool pressed) { OnReleaseMouse(pressed); };
		input_component->AddEvent(release_event);
	}
	//Register mouse look 
	{
		InputAxis up_axis;
		up_axis.AddNegative(GLFW_MOUSE_LOCATION_Y);
		up_axis.decay = 1.0f;
		up_axis.mouse_sensitivity = 0.08f;
		up_axis.func = [this](float a) { OnLookUp(a); };
		input_component->AddAxis(up_axis);

		InputAxis side_axis;
		side_axis.AddNegative(GLFW_MOUSE_LOCATION_X);
		side_axis.decay = 1.0f;
		side_axis.mouse_sensitivity = 0.08f;
		side_axis.func = [this](float a) { OnLookSideways(a); };
		input_component->AddAxis(side_axis);
	}
}

void DebugPlayer::OnForward(float amount) 
{
	current_input += local_transform.GetForward() * amount;
}

void DebugPlayer::OnStrafe(float amount) 
{
	current_input += local_transform.GetXZRight() * amount;
}

void DebugPlayer::OnJump(bool pressed) 
{
	if (pressed)
		upward_input += 1.0f;
}

void DebugPlayer::OnCrouch(bool pressed) 
{
	if (pressed)
		upward_input += -1.0f;
}

void DebugPlayer::OnGrabMouse(bool pressed) 
{
	if(pressed)
		input_component->GrabMouse();
}

void DebugPlayer::OnReleaseMouse(bool pressed) 
{
	if(pressed)
		input_component->ReleaseMouse();
}

void DebugPlayer::OnLookUp(float amount) 
{
	local_transform.rotation += glm::vec3(amount, 0, 0);
}

void DebugPlayer::OnLookSideways(float amount) 
{
	local_transform.rotation += glm::vec3(0, amount, 0);
}

void DebugPlayer::Tick(float delta_time) 
{
	Super::Tick(delta_time);

	glm::vec3 input = glm::clamp(current_input, -1.0f, 1.0f);
	current_input = glm::vec3(0);

	const float speed = 5.0f * delta_time;
	local_transform.location += input * speed + upward_input * speed * glm::vec3(0,1,0);
	upward_input = 0.0f;

	/*
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
	*/
}
