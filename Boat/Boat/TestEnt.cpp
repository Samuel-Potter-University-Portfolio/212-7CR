#include "Game.h"
#include "TestEnt.h"
#include "ModelComponent.h"
#include "Logger.h"

#include "DefaultShader.h"

TestEnt::TestEnt()
{
	model_comp = new ModelComponent(nullptr, nullptr);
	AddComponent(model_comp);
}

TestEnt::~TestEnt()
{
}

void TestEnt::WindowBegin() 
{
	__super::WindowBegin();
	shader = new DefaultShader();
	shader->Load();

	Mesh mesh(
	{
		0.0, 0.0, 0.0,	0.0, 1.0, 0.0,
		1.0, 1.0, 0.0,	1.0, 0.0, 0.0,
	},
	{
		2,1,0, 3,2,0,
	}
	);

	model = g_game->GetWindow()->GetModelLoader().CreateModel(mesh);

	model_comp->model = model;
	model_comp->shader = shader;
}

void TestEnt::WindowDestroy() 
{
	__super::WindowDestroy();
	delete shader;
}

void TestEnt::LogicTick(float delta_time) 
{
	__super::LogicTick(delta_time);

	transform.rotation += glm::vec3(0.0, 0.0, -90) * delta_time;
	//transform.location += glm::vec3(-0.2f, -0.1f, 0) * delta_time;
}