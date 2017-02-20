#include "BoxProp.h"
#include "AABoxCollider.h"
#include "Game.h"


BoxProp::BoxProp()
{
	tags |= E_TAG_PROP;
	model_comp = MakeComponent<ModelComponent>();
	MakeComponent<AABoxCollider>();
	transform.scale = glm::vec3(10, 10, 10);
}

void BoxProp::WindowBegin()
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["Cube"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/planks.png"]);
}
