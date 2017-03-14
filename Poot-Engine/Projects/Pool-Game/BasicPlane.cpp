#include "BasicPlane.h"
#include <PlaneCollider.h>


BasicPlane::BasicPlane()
{
	SetTags(OBJ_TAG_ENV);
	local_transform.location.y = -15;

	model = MakeComponent<ModelComponent>();
	model->local_transform.rotation = glm::vec3(90, 0, 0);
	model->local_transform.scale *= 100;
	model->local_transform.location = glm::vec3(1, 0, 1) * -50.0f;

	PlaneCollider* plane = MakeComponent<PlaneCollider>();
	plane->properties.friction = 0.01f;
	plane->properties.restitution = 1.0f;
	/*
	Body* body = MakeComponent<Body>();
	body->gravity_enabled = false;
	body->start_awake = false;
	body->mass = 3.0f;
	body->drag = 0.1f;

	SphereCollider* sphere = MakeComponent<SphereCollider>();
	sphere->radius = 1.0f;

	model->local_transform.scale *= sphere->radius;
	*/
}


void BasicPlane::BuildComponents() 
{
	model->model = LoadModelAsset("quad");//"Cube");
	model->shader = LoadShaderAsset("default");
	model->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));
}