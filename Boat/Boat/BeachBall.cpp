#include "BeachBall.h"
#include "DefaultShader.h"
#include "Game.h"

BeachBall::BeachBall()
{
	tags |= E_TAG_PROP;
	model_comp = MakeComponent<ModelComponent>();
	sphere_collider = MakeComponent<SphereCollider>();
	sphere_collider->GetProperties().restitution = 1.3f;

	sphere_collider->SetRadius(2.0f);
	model_comp->transform.scale *= 2.0f;

	body = MakeComponent<RigidBody>();
	body->velocity_damping_factor = 0.0001f;
	sphere_collider->transform.location = glm::vec3(0, 0, 0);

	model_comp->SetFloatUnit(SHADER_UNIT_SHININESS, 100.0f);
	model_comp->SetFloatUnit(SHADER_UNIT_ROUGHNESS, 0.0f);
}

void BeachBall::WindowBegin()
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["Resources/unit_sphere.obj"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/beach_ball.png"]);
}

