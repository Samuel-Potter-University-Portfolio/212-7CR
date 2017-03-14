#include "BasicBox.h"
#include <Game.h>
#include <SphereCollider.h>


BasicBox::BasicBox()
{
	SetTags(OBJ_TAG_PROP);
	model = MakeComponent<ModelComponent>();

	Body* body = MakeComponent<Body>();
	body->gravity_enabled = false;
	body->start_awake = false;
	body->mass = 3.0f;
	body->drag = 0.1f;

	SphereCollider* sphere = MakeComponent<SphereCollider>();
	sphere->radius = 1.0f;

	model->local_transform.scale *= sphere->radius;
}

void BasicBox::BuildComponents()
{
	model->model = LoadModelAsset("Resources/unit_sphere.obj");//"Cube");
	model->shader = LoadShaderAsset("default");
	model->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));	
}

void BasicBox::Tick(float delta_time) 
{
	//local_transform.rotation += glm::vec3(10, 20, 30) * delta_time;
	//local_transform.location += glm::vec3(0, 1, 0) * delta_time;
}