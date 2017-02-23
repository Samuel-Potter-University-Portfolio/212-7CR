#include "FloatBarrier.h"
#include "AABoxCollider.h"
#include "DefaultShader.h"
#include "Game.h"


FloatBarrier::FloatBarrier(BarrierDirection direction)
{
	tags |= E_TAG_PROP;
	model_comp = MakeComponent<ModelComponent>();
	MakeComponent<AABoxCollider>();

	if(direction == Horizontal)
		transform.scale = glm::vec3(20, 2, 2);
	else
		transform.scale = glm::vec3(2, 2, 20);

	model_comp->SetFloatUnit(SHADER_UNIT_SHININESS, 10.0f);
	model_comp->SetFloatUnit(SHADER_UNIT_ROUGHNESS, 0.2f);
}

void FloatBarrier::WindowBegin()
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["Cube"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/float.png"]);
}
