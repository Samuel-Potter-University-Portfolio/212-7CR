#include "WaterSurface.h"
#include "Logger.h"
#include "Game.h"


WaterSurface::WaterSurface()
{
	model_comp = MakeComponent<ModelComponent>();
}

void WaterSurface::WindowBegin() 
{
	__super::WindowBegin();

	model_comp->model = g_game->GetWindow()->GetModelLoader()["water_surface"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/test_text.jpg"]);
}