#include "FloatBarrier.h"
#include "AABoxCollider.h"
#include "DefaultShader.h"
#include "Game.h"


FloatBarrier::FloatBarrier(BarrierDirection direction)
{
	tags |= E_TAG_PROP;
	model_comp = MakeComponent<ModelComponent>();
	AABoxCollider* box_collider = MakeComponent<AABoxCollider>();

	if (direction == Horizontal)
	{
		box_collider->transform.scale = glm::vec3(20, 2, 2);
	}
	else
	{
		box_collider->transform.scale = glm::vec3(2, 2, 20);
		model_comp->transform.rotation = glm::vec3(0, 90.0f, 0);
	}
	model_comp->transform.scale = glm::vec3(1.7, 1, 1);

	model_comp->SetFloatUnit(SHADER_UNIT_SHININESS, 10.0f);
	model_comp->SetFloatUnit(SHADER_UNIT_ROUGHNESS, 0.2f);
}

void FloatBarrier::WindowBegin()
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["Resources/float.obj"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/float.png"]);
}
