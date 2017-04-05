#include "PoolBall.h"
#include <Game.h>
#include <DefaultShader.h>
#include <sstream>
#include "PoolWorld.h"
#include "PoolPlayer.h"


#define PI 3.141592653589793f


glm::vec3 GetBallLocation(int number)
{
#define BALL_LOC(x, y) return glm::vec3(x * 2.02f, 20.0f, 12.0f + y * 1.75f) 

	switch (number)
	{
	case 0:
		BALL_LOC(0, -17);
		break;

	case 1:
		BALL_LOC(0, 0);
		break;

	case 2:
		BALL_LOC(0.5f, 1);
		break;
	case 9:
		BALL_LOC(-0.5f, 1);
		break;

	case 10:
		BALL_LOC(1, 2);
		break;
	case 8:
		BALL_LOC(0, 2);
		break;
	case 3:
		BALL_LOC(-1, 2);
		break;

	case 12:
		BALL_LOC(1.5f, 3);
		break;
	case 5:
		BALL_LOC(0.5f, 3);
		break;
	case 11:
		BALL_LOC(-0.5f, 3);
		break;
	case 4:
		BALL_LOC(-1.5f, 3);
		break;

	case 15:
		BALL_LOC(2, 4);
		break;
	case 14:
		BALL_LOC(1, 4);
		break;
	case 7:
		BALL_LOC(0, 4);
		break;
	case 13:
		BALL_LOC(-1, 4);
		break;
	case 6:
		BALL_LOC(-2, 4);
		break;

	default:
		BALL_LOC(0, 0);
		break;
	};
}

PoolBall::PoolBall(int number)
	: number(number)
{
	SetTags(OBJ_TAG_PROP | POBJ_TAG_BALL);
	model = MakeComponent<ModelComponent>();

	body = MakeComponent<Body>();
	body->gravity_enabled = true;
	body->start_awake = false;
	body->always_awake = false;
	body->mass = 1.0f;
	body->drag = 0.01f;

	sphere = MakeComponent<SphereCollider>();
	sphere->radius = 1.0f;

	model->local_transform.scale *= sphere->radius;
	model->SetFloatUnit(SHADER_UNITF_ROUGHNESS, 0.8f);
	model->SetFloatUnit(SHADER_UNITF_SHININESS, 50.0f);
	model->SetFloatUnit(SHADER_UNITF_REFLECTIVENESS, 1.0f);

	local_transform.location = GetBallLocation(number);
}

void PoolBall::BuildComponents()
{
	model->model = LoadModelAsset("ball");
	model->shader = LoadShaderAsset("default");

	if(number != 0)
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

void PoolBall::OnCollide(Collider* collider, HitInfo& hit_info) 
{
	if (!potted && collider->HasTag(POBJ_TAG_POCKET))
	{
		PoolPlayer* player = GetWorld()->GetFirstObject<PoolPlayer>();
		if (player)
		{
			player->OnPotted(this);
			potted = true;
		}
	}
	if (collider->HasTag(POBJ_TAG_FLOOR))
	{
		PoolPlayer* player = GetWorld()->GetFirstObject<PoolPlayer>();
		if (player)
			player->OnFloored(this);
	}
}

void PoolBall::Reset() 
{
	body->velocity = glm::vec3(0);
	body->ClearFrameProperties();
	local_transform.location = GetBallLocation(number);
	SetActive(true);
	potted = false;
}