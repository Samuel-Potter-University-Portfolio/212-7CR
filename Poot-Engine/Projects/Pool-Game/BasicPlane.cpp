#include "BasicPlane.h"
#include "PoolWorld.h"
#include <PlaneCollider.h>


BasicPlane::BasicPlane()
{
	SetTags(OBJ_TAG_ENV | POBJ_TAG_FLOOR);
	local_transform.location.y = 0;

	model = MakeComponent<ModelComponent>();
	model->local_transform.rotation = glm::vec3(90, 0, 0);
	model->local_transform.scale *= 100;
	model->local_transform.location = glm::vec3(1, 0, 1) * -50.0f;

	PlaneCollider* plane = MakeComponent<PlaneCollider>();
	plane->properties.friction = 0.01f;
	plane->properties.restitution = 1.0f;
}


void BasicPlane::BuildComponents() 
{
	model->model = LoadModelAsset("quad");//"Cube");
	model->shader = LoadShaderAsset("default");
	model->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));
}