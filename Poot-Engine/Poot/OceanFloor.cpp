#include "OceanFloor.h"
#include "Game.h"

OceanFloor::OceanFloor()
{
	tags |= E_TAG_ENV;
	model_comp = MakeComponent<ModelComponent>();
	transform.location = glm::vec3(0, -30, 0);
}

void OceanFloor::WindowBegin()
{
	__super::WindowBegin();

	model_comp->model = g_game->GetWindow()->GetModelLoader()["ocean_floor"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/sand.png"]);
}
