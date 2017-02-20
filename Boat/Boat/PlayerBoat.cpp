#include "Game.h"
#include "PlayerBoat.h"
#include "Logger.h"


PlayerBoat::PlayerBoat()
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
	//camera_comp->transform.location = glm::vec3(0, 1.7f, -10.3f);
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
	body->AddAngularAcceleration(transform.GetXZForward() * cosf(track) * 15.0f);

	Keyboard& keyboard = g_game->GetWindow()->GetKeyboard();
	const float speed = 10.0f;
	const float turn_speed = 100.0f;

	if (keyboard.GetKeyState(GLFW_KEY_SPACE))
		body->GetCurrentVelocity().y = 0.1f;
	if (keyboard.GetKeyState(GLFW_KEY_LEFT_CONTROL))
		body->AddAcceleration(camera_comp->transform.GetUp() * -speed);

	if (keyboard.GetKeyState(GLFW_KEY_W))
		body->AddAcceleration(transform.GetXZForward() * speed);
	if (keyboard.GetKeyState(GLFW_KEY_S))
		body->AddAcceleration(transform.GetXZForward() * -speed);

	if (keyboard.GetKeyState(GLFW_KEY_D))
		body->AddAngularAcceleration(transform.GetUp() * -turn_speed);
	if (keyboard.GetKeyState(GLFW_KEY_A))
		body->AddAngularAcceleration(transform.GetUp() * turn_speed);
}

void PlayerBoat::WindowTick(float delta_time) 
{
	__super::WindowTick(delta_time);
	Mouse& mouse = g_game->GetWindow()->GetMouse();
	
	if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_LEFT))
		mouse.Lock();

	else if(mouse.GetButtonState(GLFW_MOUSE_BUTTON_RIGHT))
		mouse.Unlock();


	if (mouse.IsLocked())
	{
		camera_comp->transform.rotation += glm::vec3(0.0, -2.0, 0.0) * delta_time * mouse.GetScaledVelocity().x;
		camera_comp->transform.rotation += glm::vec3(2.0, 0.0, 0.0) * delta_time * mouse.GetScaledVelocity().y;

		if (camera_comp->transform.rotation.x > 89.0f)
			camera_comp->transform.rotation.x = 89.0f;

		if (camera_comp->transform.rotation.x < -89.0f)
			camera_comp->transform.rotation.x = -89.0f;
	}

}