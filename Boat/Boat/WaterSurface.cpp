#include "WaterSurface.h"
#include "Logger.h"
#include "Game.h"


WaterSurface::WaterSurface()
	: frame_buffer(512, 512)
{
	tags |= E_TAG_WATER;
	model_comp = MakeComponent<ModelComponent>();
	camera_comp = MakeComponent<CameraComponent>();

	camera_comp->transform.location = glm::vec3(0, 10, 0);
	camera_comp->transform.rotation = glm::vec3(90, 0, 0);
}

void WaterSurface::WindowBegin() 
{
	__super::WindowBegin();

	model_comp->model = g_game->GetWindow()->GetModelLoader()["water_surface"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["water"];

	frame_buffer.Create();
	model_comp->SetTextureUnit(0, frame_buffer.GetTextureID());
}

void WaterSurface::WindowDestroy() 
{
	__super::WindowDestroy();
	frame_buffer.CleanUp();
}

void WaterSurface::WindowTick(float delta_time) 
{
	__super::WindowTick(delta_time);

	camera_comp->transform.rotation += glm::vec3(0, 45, 0) * delta_time;

	World* world = GetWorld();
	Renderer* renderer = world ? world->GetRenderer() : nullptr;

	if (!renderer)
		return;

	frame_buffer.Bind();
	renderer->Render(frame_buffer.GetAspectRatio(), camera_comp, E_TAG_ALL, E_TAG_WATER);
	frame_buffer.Unbind();
}