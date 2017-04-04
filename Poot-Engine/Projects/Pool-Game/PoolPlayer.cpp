#include "PoolPlayer.h"
#include <World.h>
#include <DefaultShader.h>


#include <sstream>


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

	{
		main_canvas = MakeComponent<Canvas>();
		main_canvas->canvas_mode = Scaled;

		{
			force_text = main_canvas->MakeElement<TextElement>();
			force_text->font_sheet_key = "Resources/arial_ascii_bitmap.bmp";
			force_text->local_transform.scale *= 50;
			force_text->local_transform.location = glm::vec2(30, 30);
			force_text->anchor = glm::vec2(-1, -1);
			force_text->aligment = Left;
			force_text->colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}

		{
			Canvas* sub_canvas = main_canvas->MakeElement<Canvas>();
			sub_canvas->canvas_mode = Scaled;

			console_text = sub_canvas->MakeElement<TextElement>();
			console_text->font_sheet_key = "Resources/arial_ascii_bitmap.bmp";
			console_text->local_transform.scale *= 20;
			console_text->text = "0\n1\n2\n3\n4\n5\n6\n8\n9";
			console_text->local_transform.location = glm::vec2(-20, 350);
			console_text->anchor = glm::vec2(1, -1);
			console_text->aligment = Right;
			console_text->colour = glm::vec4(0.8f, 1.0f, 0.8f, 1.0f);
		}
	}
}

PoolPlayer::~PoolPlayer()
{
}

void PoolPlayer::BuildComponents()
{
	Print("~Controls~");
	Print("Toggle Debug View: (F1)");
	Print("Toggle fullscreen: (F11)");

	//Register mouse grab
	{
		InputEvent grab_event;
		grab_event.AddInput(GLFW_KEY_G);
		grab_event.AddInput(GLFW_MOUSE_BUTTON_MIDDLE);
		grab_event.func = [this](bool pressed) { OnGrabMouse(pressed); };
		input_component->AddEvent(grab_event);

		InputEvent release_event;
		release_event.AddInput(GLFW_MOUSE_BUTTON_RIGHT);
		release_event.AddInput(GLFW_KEY_ESCAPE);
		release_event.func = [this](bool pressed) { OnReleaseMouse(pressed); };
		input_component->AddEvent(release_event);

		Print("Grab Mouse: (G/MMB)");
		Print("Release Mouse: (RMB/ESC)");
	}
	//Register mouse hit
	{
		InputEvent event;
		event.AddInput(GLFW_MOUSE_BUTTON_LEFT);
		event.AddInput(GLFW_KEY_SPACE);
		event.is_spammable = false;
		event.func = [this](bool pressed) { OnHit(pressed); };
		input_component->AddEvent(event);
		Print("Shoot ball: (LMB/Space)");
	}
	//Register camera change
	{
		InputEvent event;
		event.AddInput(GLFW_KEY_V);
		event.is_spammable = false;
		event.func = [this](bool pressed) { OnChangeCameraMode(pressed); };
		input_component->AddEvent(event);
		Print("Camera Mode:	(V)");
	}
	//Register mouse look 
	{
		InputAxis side_axis;
		side_axis.AddNegative(GLFW_MOUSE_LOCATION_X);
		side_axis.AddPositive(GLFW_KEY_LEFT);
		side_axis.AddNegative(GLFW_KEY_RIGHT);
		side_axis.decay = 1.0f;
		side_axis.mouse_sensitivity = 0.08f;
		side_axis.func = [this](float a) { OnLookSideways(a); };
		input_component->AddAxis(side_axis);
		Print("Aim Cue: (Move Mouse/Left/Right)");
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
	if (!cue_ball_body || !AreBallsStill())
		return;
	
	if (pressed)
		is_shooting = true;

	if (!pressed && is_shooting)
		Shoot();
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
	local_transform.rotation += glm::vec3(0, amount, 0) * (currrent_mode == Watching ? -1.0f : 1.0f);
}

bool PoolPlayer::AreBallsStill() 
{
	for (PoolBall* ball : pool_balls)
		if (!ball->IsStill())
			return false;

	return true;
}

void PoolPlayer::Print(std::string message)
{
	for (int i = 1; i < CONSOLE_BUFFER_SIZE; i++)
		console_buffer[CONSOLE_BUFFER_SIZE - i] = console_buffer[CONSOLE_BUFFER_SIZE - i - 1];
	console_buffer[0] = message;

	std::stringstream str_stream;

	for (int i = CONSOLE_BUFFER_SIZE - 1; i >= 0; i--)
		str_stream << console_buffer[i] << '\n';

	console_text->text = str_stream.str();
}

void PoolPlayer::OnChangeCameraMode(bool pressed) 
{
	if (pressed)
	{
		if (currrent_mode == Shooting)
		{
			camera->local_transform.location = camera->GetWorldLocation();
			camera->SetTransformParent(nullptr);

			currrent_mode = Watching;
		}
		else if (currrent_mode == Watching)
		{
			camera->local_transform.location -= GetWorldLocation();
			camera->local_transform.rotation.x = 0.0f;
			camera->SetTransformParent(this);

			currrent_mode = Shooting;
		}
	}
}

void PoolPlayer::FinishWatching() 
{
	//Attach cue model
	model->local_transform.location = glm::vec3(0, 0, -1);
	model->local_transform.rotation = glm::vec3(10.0f, 0, 0);
	model->SetTransformParent(this);

	shot_in_progress = false;
}

void PoolPlayer::Shoot() 
{
	//Detach cue model
	model->local_transform.location = glm::vec3(0, 0, -1);
	model->local_transform.location = model->GetWorldLocation();
	model->local_transform.rotation = glm::vec3(10.0f, local_transform.rotation.y, 0);
	model->SetTransformParent(nullptr);

	//Apply force
	const float min_power = 0.5f;
	const float max_power = 5.0f;
	const float actual_power = min_power * (1.0f - shot_power) + max_power * shot_power;
	cue_ball_body->ApplyForce(local_transform.GetForward() * actual_power);

	//Reset tracking vars
	is_shooting = false;
	shot_power = 0.0f;
	shot_timer = 0.0f;
	force_text->text = "";
	shot_in_progress = true;
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
	

	if (shot_in_progress && AreBallsStill())
		FinishWatching();

	//Power anim
	if (is_shooting)
	{
		shot_power = cosf(shot_timer) * -0.5f + 0.5f;
		shot_timer += delta_time;

		model->local_transform.location = glm::vec3(0, 0, -1.0f - 5.0f * shot_power);

		//Text animation
		{
			const int shot_percent = shot_power * 100;

			std::stringstream message;
			message << shot_percent << '%';
			force_text->text = message.str();

			const glm::vec4 RED = glm::vec4(1, 0, 0, 1);
			const glm::vec4 YLW = glm::vec4(1, 1, 0, 1);
			const glm::vec4 GRN = glm::vec4(0, 1, 0, 1);

			if (shot_power < 0.10f)
				force_text->colour = RED;

			//0.1 - 0.3
			else if (shot_power < 0.30f)
			{
				const float start = 0.1f;
				const float end = 0.3f;
				const float v = (shot_power - start) / (end - start);

				force_text->colour = RED * (1.0f - v) + YLW * v;
			}

			else if (shot_power < 0.5f)
				force_text->colour = YLW;

			//0.5 - 0.8
			else if (shot_power < 0.8f)
			{
				const float start = 0.5f;
				const float end = 0.8f;
				const float v = (shot_power - start) / (end - start);

				force_text->colour = YLW * (1.0f - v) + GRN * v;
			}

			else
				force_text->colour = GRN;
		}
	}



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