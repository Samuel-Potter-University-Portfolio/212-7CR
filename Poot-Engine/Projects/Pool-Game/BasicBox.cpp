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