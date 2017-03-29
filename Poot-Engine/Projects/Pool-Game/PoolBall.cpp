#include "PoolBall.h"
#include <Game.h>
#include <DefaultShader.h>
#include <sstream>


#define PI 3.141592653589793f


PoolBall::PoolBall(int number)
	: number(number)
{
	SetTags(OBJ_TAG_PROP);
	model = MakeComponent<ModelComponent>();

	Body* body = MakeComponent<Body>();
	body->gravity_enabled = true;
	body->start_awake = true;
	body->always_awake = false;
	body->mass = 1.0f;
	body->drag = 0.01f;

	sphere = MakeComponent<SphereCollider>();
	sphere->radius = 1.0f;

	model->local_transform.scale *= sphere->radius;
	model->SetFloatUnit(SHADER_UNITF_ROUGHNESS, 0.8f);
	model->SetFloatUnit(SHADER_UNITF_SHININESS, 50.0f);
	model->SetFloatUnit(SHADER_UNITF_REFLECTIVENESS, 1.0f);
}

void PoolBall::BuildComponents()
{
	model->model = LoadModelAsset("ball");
	model->shader = LoadShaderAsset("default");

	model->SetIntUnit(SHADER_UNITI_USING_PHONG_MAP, 1);

	std::stringstream str_stream;
	str_stream << "Resources/ball_" << number << ".png";
	model->SetTextureUnit(SHADER_UNITT_BASE_TEXTURE, LoadTextureAsset(str_stream.str()));

	if(number <= 8)
		model->SetTextureUnit(SHADER_UNITT_PHONG_MAP, LoadTextureAsset("Resources/ball_solid_phong.png"));
	else
		model->SetTextureUnit(SHADER_UNITT_PHONG_MAP, LoadTextureAsset("Resources/ball_stripe_phong.png"));

	model->SetTextureUnit(SHADER_UNITT_REFLECTION_CM, LoadCubeMapAsset("Resources/Skybox/TropicalSunnyDay.png"));
}

void PoolBall::Tick(float delta_time) 
{
	const float circumference = 2 * PI * sphere->radius;

	model->local_transform.rotation = local_transform.location * 360.0f / circumference;
	//local_transform.location += glm::vec3(0, 1, 0) * delta_time;
}