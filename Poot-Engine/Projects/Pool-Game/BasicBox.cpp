#include "BasicBox.h"
#include <Game.h>


BasicBox::BasicBox()
{
	SetTags(OBJ_TAG_PROP);
	model = MakeComponent<ModelComponent>();
}

void BasicBox::BuildComponents()
{
	model->model = LoadModelAsset("Cube");
	model->shader = LoadShaderAsset("default");
	model->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));	
}

void BasicBox::Tick(float delta_time) 
{
	local_transform.rotation += glm::vec3(0, 30, 0) * delta_time;
	local_transform.location += glm::vec3(0, 0.1f, 0) * delta_time;
}