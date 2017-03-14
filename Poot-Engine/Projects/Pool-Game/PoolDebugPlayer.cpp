#include "PoolDebugPlayer.h"
#include <CameraComponent.h>
#include <SphereCollider.h>


PoolDebugPlayer::PoolDebugPlayer()
{
	SetTags(OBJ_TAG_PLAYER);
	CameraComponent* camera = MakeComponent<CameraComponent>();
	camera->use_latest_rotation = true;

	body = MakeComponent<Body>();
	body->gravity_enabled = true;
	body->drag = 0.1f;
	input_component = MakeComponent<InputComponent>();

	model_component = MakeComponent<ModelComponent>();
	SphereCollider* sphere = MakeComponent<SphereCollider>();
	sphere->local_transform.location = glm::vec3(0,0,0);
	sphere->radius = 1.0f;

	model_component->local_transform.location = sphere->local_transform.location;
	model_component->local_transform.scale *= sphere->radius;
}

void PoolDebugPlayer::BuildComponents()
{
	model_component->model = LoadModelAsset("Resources/unit_sphere.obj");
	model_component->shader = LoadShaderAsset("default");
	model_component->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));

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

void PoolDebugPlayer::OnForward(float amount)
{
	current_input += local_transform.GetForward() * amount;
}

void PoolDebugPlayer::OnStrafe(float amount)
{
	current_input += local_transform.GetXZRight() * amount;
}

void PoolDebugPlayer::OnJump(bool pressed)
{
	if (pressed)
		upward_input += 1.0f;
}

void PoolDebugPlayer::OnCrouch(bool pressed)
{
	if (pressed)
		upward_input += -1.0f;
}

void PoolDebugPlayer::OnGrabMouse(bool pressed)
{
	if (pressed)
		input_component->GrabMouse();
}

void PoolDebugPlayer::OnReleaseMouse(bool pressed)
{
	if (pressed)
		input_component->ReleaseMouse();
}

void PoolDebugPlayer::OnLookUp(float amount)
{
	local_transform.rotation += glm::vec3(amount, 0, 0);

	if (local_transform.rotation.x > 89.9f)
		local_transform.rotation.x = 89.9f;

	if (local_transform.rotation.x < -89.9f)
		local_transform.rotation.x = -89.9f;
}

void PoolDebugPlayer::OnLookSideways(float amount)
{
	local_transform.rotation += glm::vec3(0, amount, 0);
}

void PoolDebugPlayer::Tick(float delta_time)
{
	Super::Tick(delta_time);

	glm::vec3 input = glm::clamp(current_input, -1.0f, 1.0f);
	current_input = glm::vec3(0);

	const float speed = 1.0f * delta_time;
	body->velocity += input * speed + upward_input * speed * glm::vec3(0, 1, 0);
	upward_input = 0.0f;
}

#include <Logger.h>
void PoolDebugPlayer::OnCollide(Collider* collider, HitInfo& hit_info) 
{
	//LOG(Log, "Hit");
}