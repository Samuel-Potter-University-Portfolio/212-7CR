#include "WaterSurface.h"
#include "Logger.h"
#include "Game.h"


WaterSurface::WaterSurface()
	: reflection_fbo(1024, 1024), refraction_fbo(1024, 1024)
{
	tags |= E_TAG_WATER;
	model_comp = MakeComponent<ModelComponent>();
	reflection_camera = MakeComponent<CameraComponent>();
	refraction_camera = MakeComponent<CameraComponent>();
}

void WaterSurface::WindowBegin() 
{
	__super::WindowBegin();

	model_comp->model = g_game->GetWindow()->GetModelLoader()["water_surface"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["water"];

	reflection_fbo.Create();
	refraction_fbo.Create();
	model_comp->SetTextureUnit(0, reflection_fbo.GetTextureID());
	model_comp->SetTextureUnit(1, refraction_fbo.GetTextureID());
}

void WaterSurface::WindowDestroy() 
{
	__super::WindowDestroy();
	reflection_fbo.CleanUp();
	refraction_fbo.CleanUp();
}

void WaterSurface::LogicTick(float delta_time)
{
	CameraComponent* main_camera = GetWorld()->GetMainCamera();

	if (main_camera)
	{
		reflection_camera->transform.location = main_camera->transform.location;
		reflection_camera->transform.rotation = main_camera->transform.rotation;

		refraction_camera->transform.location = main_camera->transform.location;
		refraction_camera->transform.rotation = main_camera->transform.rotation;
	}

	reflection_camera->transform.location.y *= -1;
	reflection_camera->transform.rotation.x *= -1;
	__super::LogicTick(delta_time);
}

void WaterSurface::WindowTick(float delta_time) 
{
	__super::WindowTick(delta_time);
	
	World* world = GetWorld();
	Renderer* renderer = world ? world->GetRenderer() : nullptr;

	if (!renderer)
		return;

	const float aspect_ratio = g_game->GetWindow()->GetAspectRatio();

	reflection_fbo.Bind();
	renderer->Render(aspect_ratio, reflection_camera, E_TAG_ALL, E_TAG_WATER);
	reflection_fbo.Unbind();

	refraction_fbo.Bind();
	renderer->Render(aspect_ratio, refraction_camera, E_TAG_ALL, E_TAG_WATER);
	refraction_fbo.Unbind();
}