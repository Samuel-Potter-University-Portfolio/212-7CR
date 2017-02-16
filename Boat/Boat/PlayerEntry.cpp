#include "PlayerEntry.h"
#include "PlayerBoat.h"
#include "Game.h"

PlayerEntry::PlayerEntry()
{
	tags |= E_TAG_PLAYER | E_TAG_UI;
	model_comp = MakeComponent<ModelComponent>();
	camera_comp = MakeComponent<CameraComponent>();
	
	transform.location = glm::vec3(0, 1.0f, 0);
	model_comp->transform.rotation = glm::vec3(0, 180.0f, 0);
	model_comp->transform.location = glm::vec3(0, 1.3f, 0);
	model_comp->transform.scale = glm::vec3(10, 10, 10);
	model_comp->SetVisable(true);

	camera_comp->transform.location = glm::vec3(0, 8.4f, -1.3f);
	camera_comp->transform.location += model_comp->transform.location;
}

void PlayerEntry::WindowBegin()
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["ui_quad"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["ui"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/title.png"]);
}

void PlayerEntry::LogicTick(float delta_time) 
{
	__super::LogicTick(delta_time);

	if (!IsVisable())
		return;

	Keyboard& keyboard = g_game->GetWindow()->GetKeyboard();

	if (keyboard.GetKeyState(GLFW_KEY_ENTER))
	{
		SetVisable(false);

		PlayerBoat* player_boat = new PlayerBoat;
		GetWorld()->AddEntity(player_boat);
		GetWorld()->SetMainCamera(player_boat->camera_comp);
	}
}

void PlayerEntry::WindowTick(float delta_time)
{
	__super::WindowTick(delta_time);

	if (!IsVisable())
		return;
	
	Mouse& mouse = g_game->GetWindow()->GetMouse();

	if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_LEFT))
		mouse.Lock();

	else if (mouse.GetButtonState(GLFW_MOUSE_BUTTON_RIGHT))
		mouse.Unlock();

	transform.rotation += glm::vec3(0, 3, 0) * delta_time;
}