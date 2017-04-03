#include "PoolPlayer.h"
#include <World.h>
#include <DefaultShader.h>


#define CLAMP(x, min, max) (x > max ? max : x < min ? min : x)


PoolPlayer::PoolPlayer()
{
	SetTags(OBJ_TAG_PLAYER);

	camera = MakeComponent<CameraComponent>();
	camera->local_transform.location = glm::vec3(0, 20, 17);
	input_component = MakeComponent<InputComponent>();

	model = MakeComponent<ModelComponent>();
	model->SetIntUnit(SHADER_UNITI_USING_PHONG_MAP, 1);
	model->SetFloatUnit(SHADER_UNITF_ROUGHNESS, 0.8f);
	model->SetFloatUnit(SHADER_UNITF_SHININESS, 50.0f);
	model->SetFloatUnit(SHADER_UNITF_REFLECTIVENESS, 0.25f);
}

PoolPlayer::~PoolPlayer()
{
}

void PoolPlayer::BuildComponents()
{
	//Register mouse grab
	{
		InputEvent grab_event;
		grab_event.AddInput(GLFW_MOUSE_BUTTON_RIGHT);
		grab_event.AddInput(GLFW_MOUSE_BUTTON_MIDDLE);
		grab_event.func = [this](bool pressed) { OnGrabMouse(pressed); };
		input_component->AddEvent(grab_event);

		InputEvent release_event;
		release_event.AddInput(GLFW_MOUSE_BUTTON_RIGHT);
		release_event.AddInput(GLFW_KEY_ESCAPE);
		release_event.func = [this](bool pressed) { OnReleaseMouse(pressed); };
		input_component->AddEvent(release_event);
	} 
	//Register mouse hit
	{
		InputEvent event;
		event.AddInput(GLFW_MOUSE_BUTTON_LEFT);
		event.AddInput(GLFW_KEY_SPACE);
		event.func = [this](bool pressed) { OnHit(pressed); };
		input_component->AddEvent(event);
	}
	//Register mouse look 
	{
		InputAxis side_axis;
		side_axis.AddNegative(GLFW_MOUSE_LOCATION_X);
		side_axis.decay = 1.0f;
		side_axis.mouse_sensitivity = 0.08f;
		side_axis.func = [this](float a) { OnLookSideways(a); };
		input_component->AddAxis(side_axis);
	}

	model->model = LoadModelAsset("cue");
	model->shader = LoadShaderAsset("default");
	model->SetTextureUnit(SHADER_UNITT_BASE_TEXTURE, LoadTextureAsset("Resources/cue.png"));
	model->SetTextureUnit(SHADER_UNITT_PHONG_MAP, LoadTextureAsset("Resources/cue_phong.png"));
	model->SetTextureUnit(SHADER_UNITT_REFLECTION_CM, LoadCubeMapAsset("Resources/Skybox/TropicalSunnyDay.png"));

	FinishWatching();
}

void PoolPlayer::OnHit(bool pressed) 
{
	if (!cue_ball_body)
		return;

	if (pressed && currrent_mode == Shooting)
		Shoot(1.0f);
}

void PoolPlayer::OnGrabMouse(bool pressed)
{
	if (pressed)
		input_component->GrabMouse();
}

void PoolPlayer::OnReleaseMouse(bool pressed)
{
	if (pressed)
		input_component->ReleaseMouse();
}

void PoolPlayer::OnLookSideways(float amount)
{
	local_transform.rotation += glm::vec3(0, amount, 0);
}

bool PoolPlayer::AreBallsStill() 
{
	for (PoolBall* ball : pool_balls)
		if (!ball->IsStill())
			return false;

	return true;
}

void PoolPlayer::FinishWatching() 
{
	currrent_mode = Shooting;
	camera->local_transform.location -= GetWorldLocation();
	camera->local_transform.rotation.x = 0.0f;
	camera->SetTransformParent(this);

	model->local_transform.location = glm::vec3(0, 0, -1);
	model->local_transform.rotation = glm::vec3(10.0f, 0, 0);
	model->SetTransformParent(this);
}

void PoolPlayer::Shoot(float power) 
{
	cue_ball_body->ApplyForce(local_transform.GetForward() * power);
	currrent_mode = Watching;

	camera->local_transform.location = camera->GetWorldLocation();
	camera->SetTransformParent(nullptr);

	model->local_transform.location = model->GetWorldLocation();
	model->local_transform.rotation = glm::vec3(10.0f, local_transform.rotation.y, 0);
	model->SetTransformParent(nullptr);
}

void PoolPlayer::Tick(float delta_time) 
{
	Super::Tick(delta_time);

	//Fetch cue ball
	if (!cue_ball)
	{
		pool_balls = GetWorld()->GetAllObjects<PoolBall>();

		for (PoolBall* ball : pool_balls)
			if (ball->GetNumber() == 0) 
			{
				cue_ball = ball;
				break;
			}

		//Still null
		if (!cue_ball)
			return;

		cue_ball_body = cue_ball->GetComponent<Body>();
		transform_parent = cue_ball;
	}
	

	if (currrent_mode == Watching && AreBallsStill())
		FinishWatching();


	//Camera transitions
	const float transition_power = CLAMP(2.0f * delta_time, 0.0f, 1.0f);
	glm::vec3 desired_position;

	if (currrent_mode == Shooting)
		desired_position = glm::vec3(0, 5, -15);

	else
	{
		desired_position = glm::vec3(0, 45, -20);
		glm::vec3 desired_rotation = glm::vec3(-70, 0, 0);
		camera->local_transform.rotation = camera->local_transform.rotation * (1.0f - transition_power) + desired_rotation * transition_power;
	}

	camera->local_transform.location = camera->local_transform.location * (1.0f - transition_power) + desired_position * transition_power;
}