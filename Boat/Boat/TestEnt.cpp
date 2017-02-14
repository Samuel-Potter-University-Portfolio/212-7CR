#include "Game.h"
#include "TestEnt.h"
#include "Logger.h"


TestEnt::TestEnt()
{
	tags |= E_TAG_PLAYER;
	model_comp = MakeComponent<ModelComponent>();
	camera_comp = MakeComponent<CameraComponent>();
	sphere_collider = MakeComponent<SphereCollider>();
	body = MakeComponent<RigidBody>();

	transform.location = glm::vec3(0, 10.0f, 0);
	camera_comp->transform.location = glm::vec3(0, 3.0f, -3.0f);
}

void TestEnt::WindowBegin() 
{
	__super::WindowBegin();
	//model_comp->model = g_game->GetWindow()->GetModelLoader()["Resources/row_boat.obj"];
	model_comp->model = g_game->GetWindow()->GetModelLoader()["Resources/unit_sphere.obj"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/planks.png"]);
}

void TestEnt::WindowDestroy() 
{
	__super::WindowDestroy();
}

void TestEnt::LogicTick(float delta_time) 
{
	__super::LogicTick(delta_time);
	
	Keyboard& keyboard = g_game->GetWindow()->GetKeyboard();
	const float speed = 10.0f;

	if (keyboard.GetKeyState(GLFW_KEY_SPACE))
		body->GetCurrentVelocity().y = 0.1f;
	if (keyboard.GetKeyState(GLFW_KEY_LEFT_CONTROL))
		body->AddAcceleration(camera_comp->transform.GetUp() * -speed);

	if (keyboard.GetKeyState(GLFW_KEY_W))
		body->AddAcceleration(camera_comp->transform.GetXZForward() * speed);
	if (keyboard.GetKeyState(GLFW_KEY_S))
		body->AddAcceleration(camera_comp->transform.GetXZForward() * -speed);

	if (keyboard.GetKeyState(GLFW_KEY_D))
		body->AddAcceleration(camera_comp->transform.GetXZRight() * speed);
	if (keyboard.GetKeyState(GLFW_KEY_A))
		body->AddAcceleration(camera_comp->transform.GetXZRight() * -speed);
}

void TestEnt::WindowTick(float delta_time) 
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