#include "Game.h"
#include "PlayerBoat.h"
#include <math.h>
#include <sstream>
#include "Logger.h"


#define PI 3.141592653589793f
#define DEG_RAD(x) (x * 180.f/PI)

PlayerBoat::PlayerBoat()
	: camera_mode(BoatCam)
{
	tags |= E_TAG_PLAYER;
	model_comp = MakeComponent<ModelComponent>();
	model_comp_test = MakeComponent<ModelComponent>();
	camera_comp = MakeComponent<CameraComponent>();
	sphere_collider = MakeComponent<SphereCollider>();

	body = MakeComponent<RigidBody>();
	body->angular_damping_factor = 0.03f;

	transform.location = glm::vec3(0, 1.0f, 0);
	model_comp->transform.rotation = glm::vec3(0, 180.0f, 0);
	model_comp->transform.location = glm::vec3(0, 1.3f, 0);
	model_comp->SetVisable(true);

	camera_comp->transform.location = glm::vec3(0, 3.4f, -1.3f);
	camera_comp->transform.location += model_comp->transform.location;

	sphere_collider->transform.location = glm::vec3(0, 0, 0);
	sphere_collider->SetRadius(3.8f);
	model_comp_test->transform.scale *= sphere_collider->GetRadius();
	model_comp_test->transform.location = sphere_collider->transform.location;
	model_comp_test->SetVisable(false);
}

void PlayerBoat::WindowBegin() 
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["Resources/row_boat.obj"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/planks.png"]);

	model_comp_test->model = g_game->GetWindow()->GetModelLoader()["Resources/unit_sphere.obj"];
	model_comp_test->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp_test->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/planks.png"]);
}

void PlayerBoat::WindowDestroy() 
{
	__super::WindowDestroy();
}

void PlayerBoat::LogicTick(float delta_time) 
{
	__super::LogicTick(delta_time);
	track += delta_time * 2.0f;

	//Bobbing/Swaying
	body->AddAcceleration(transform.GetUp() * cosf(track) * 5.0f);
	body->AddAngularAcceleration(transform.GetForward() * cosf(track) * 15.0f);

	//Input
	Keyboard& keyboard = g_game->GetWindow()->GetKeyboard();
	const float speed = 15.0f;
	const float turn_speed = 200.0f;

	if (keyboard.GetKeyState(GLFW_KEY_SPACE))
		body->GetCurrentVelocity().y = 0.1f;
	if (keyboard.GetKeyState(GLFW_KEY_LEFT_CONTROL))
		body->AddAcceleration(camera_comp->transform.GetUp() * -speed);

	if (keyboard.GetKeyState(GLFW_KEY_W))
		body->AddAcceleration(transform.GetXZForward() * speed);
	if (keyboard.GetKeyState(GLFW_KEY_S))
		body->AddAcceleration(transform.GetXZForward() * -speed);

	if (keyboard.GetKeyState(GLFW_KEY_D))
		body->AddAngularAcceleration(glm::vec3(0, 1, 0) * -turn_speed);
	if (keyboard.GetKeyState(GLFW_KEY_A))
		body->AddAngularAcceleration(glm::vec3(0, 1, 0) * turn_speed);


	//Change camera mode
	const bool current_state = keyboard.GetKeyState(GLFW_KEY_V);

	//Button pressed (And released)
	if (current_state != last_camera_button_state && last_camera_button_state)
	{
		switch (camera_mode)
		{
		case BoatCam:
			camera_mode = TopDown;
			break;
		case TopDown:
			camera_comp->transform.rotation = glm::vec3(0, 0, 0);
			camera_mode = ActionCam;
			break;
		case ActionCam:
			camera_mode = BoatCam;
			break;
		};


		//Set Camera position
		if (camera_mode == BoatCam)
		{
			camera_comp->transform.location = glm::vec3(0, 3.4f, -1.3f);
			camera_comp->transform.location += model_comp->transform.location;
		}
		if (camera_mode == TopDown)
		{
			camera_comp->transform.rotation = glm::vec3(-88.0f, 0, 0);
		}
	}
	last_camera_button_state = current_state;


	//Set Camera position
	if (camera_mode == TopDown)
	{
		camera_comp->transform.location = transform.location + glm::vec3(0, 35.4f, 0);
	}
	if (camera_mode == ActionCam)
	{
		const float clamp = 100.0f;
		glm::vec3 location = transform.location / clamp;
		location.x = roundf(location.x);
		location.y = 0;
		location.z = roundf(location.z);
		location *= clamp;

		camera_comp->transform.location = glm::vec3(0, 10.4f, 0) + location;
	}
}

void PlayerBoat::WindowTick(float delta_time) 
{
	__super::WindowTick(delta_time);

	//Log fps
	GLFWwindow* window = g_game->GetWindow()->GetGLFWwindow();
	const int fps = g_game->GetWindow()->GetCurrentTickRate();
	const int ups = g_game->GetGameLogic()->GetCurrentTickRate();

	std::stringstream str_stream;
	str_stream << "Boat [FPS:" << fps << " UPS:" << ups << "]";
	glfwSetWindowTitle(window, str_stream.str().c_str());


	Mouse& mouse = g_game->GetWindow()->GetMouse();
	
	if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_LEFT))
		mouse.Lock();

	else if(mouse.GetButtonState(GLFW_MOUSE_BUTTON_RIGHT))
		mouse.Unlock();


	if (mouse.IsLocked() && camera_mode == BoatCam)
	{
		camera_comp->SetParent(this);
		camera_comp->transform.rotation += glm::vec3(0.0, -2.0, 0.0) * delta_time * mouse.GetScaledVelocity().x;
		camera_comp->transform.rotation += glm::vec3(2.0, 0.0, 0.0) * delta_time * mouse.GetScaledVelocity().y;

		if (camera_comp->transform.rotation.x > 89.0f)
			camera_comp->transform.rotation.x = 89.0f;

		if (camera_comp->transform.rotation.x < -89.0f)
			camera_comp->transform.rotation.x = -89.0f;
	}
	if (mouse.IsLocked() && camera_mode == TopDown)
	{
		camera_comp->SetParent(nullptr);
		camera_comp->transform.rotation += glm::vec3(0.0, -2.0, 0.0) * delta_time * mouse.GetScaledVelocity().x;
	}
	if (camera_mode == ActionCam)
	{
		camera_comp->SetParent(nullptr);
		glm::vec3 look_at = transform.location - camera_comp->transform.location;
		camera_comp->transform.rotation.y = look_at.z != 0 ? DEG_RAD(atanf(look_at.x / look_at.z)) + (look_at.z >= 0.0f ? 1.0f : 180.0f) : 0.0f;
	}

}