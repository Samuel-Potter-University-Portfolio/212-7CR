#include "PoolEntry.h"
#include "PoolPlayer.h"
#include "PoolDebugPlayer.h"
#include <World.h>
#include <QuadUI.h>


PoolEntry::PoolEntry()
{
	SetTags(OBJ_TAG_PLAYER);

	local_transform.location = glm::vec3(0, 20, 17);

	main_canvas = MakeComponent<Canvas>();
	main_canvas->canvas_mode = Scaled;
	main_canvas->sorting_layer = 10;

	{
		QuadUI* quad = main_canvas->MakeElement<QuadUI>();
		quad->texture_key = "Resources/title.png";
		quad->local_transform.scale *= 400;
		quad->local_transform.location = glm::vec2(0, 0);
		quad->anchor = glm::vec2(0, 0);
	}

	camera = MakeComponent<CameraComponent>();
	camera->local_transform.location = glm::vec3(0, 5, -10);
	input_component = MakeComponent<InputComponent>();
}

void PoolEntry::BuildComponents() 
{
	//Register mouse hit
	{
		InputEvent event;
		event.AddInput(GLFW_KEY_ENTER);
		event.func = [this](bool pressed) { OnEnter(pressed); };
		input_component->AddEvent(event);
	}
}

void PoolEntry::Tick(float delta_time)
{
	local_transform.rotation += glm::vec3(0, 5, 0) * delta_time;
}

void PoolEntry::OnEnter(bool pressed)
{
	if (pressed)
	{
#ifdef _DEBUG
		SetActive(false);
		PoolDebugPlayer* player = new PoolDebugPlayer;
		GetWorld()->AddObject(player);
		GetWorld()->SetMainCamera(player->GetComponent<CameraComponent>());
#else
		SetActive(false);
		PoolPlayer* player = new PoolPlayer;
		GetWorld()->AddObject(player);
		GetWorld()->SetMainCamera(player->GetComponent<CameraComponent>());
#endif
	}
}
