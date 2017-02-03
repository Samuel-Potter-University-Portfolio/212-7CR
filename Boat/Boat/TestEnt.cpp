#include "Game.h"
#include "TestEnt.h"
#include "Logger.h"


TestEnt::TestEnt()
{
	model_comp = new ModelComponent(nullptr, nullptr);
	camera_comp = new CameraComponent;

	camera_comp->transform.location = glm::vec3(0, 0, -3.0f);
	AddComponent(model_comp);
	AddComponent(camera_comp);
}

TestEnt::~TestEnt()
{
}

void TestEnt::WindowBegin() 
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["cube"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
}

void TestEnt::WindowDestroy() 
{
	__super::WindowDestroy();
	delete shader;
}

void TestEnt::LogicTick(float delta_time) 
{
	__super::LogicTick(delta_time);

	Keyboard& keyboard = g_game->GetWindow()->GetKeyboard();
	Mouse& mouse = g_game->GetWindow()->GetMouse();

	if (keyboard.GetKeyState(GLFW_KEY_W))
		transform.rotation += glm::vec3(10.0, 0.0, 0.0) * delta_time * 10.0f;
	if (keyboard.GetKeyState(GLFW_KEY_A))
		transform.rotation += glm::vec3(0.0, 10.0, 0.0) * delta_time * 10.0f;
	

	if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_3))
	{
		mouse.Lock();
		transform.rotation += glm::vec3(0.0, 1.0, 0.0) * delta_time * 2.0f * mouse.GetScaledVelocity().x;
		transform.rotation += glm::vec3(1.0, 0.0, 0.0) * delta_time * 2.0f * mouse.GetScaledVelocity().y;
	}
	else
		mouse.Unlock();
}

void TestEnt::WindowTick(float delta_time) 
{
	__super::WindowTick(delta_time);

}