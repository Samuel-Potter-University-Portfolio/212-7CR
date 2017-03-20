#include "PoolTable.h"
#include "PoolTable.h"


PoolTable::PoolTable()
{
	SetTags(OBJ_TAG_ENV | POBJ_TAG_TABLE);

	body_model = MakeComponent<ModelComponent>();
	surface_model = MakeComponent<ModelComponent>();
}

void PoolTable::BuildComponents() 
{
	body_model->model = LoadModelAsset("Resources/table_body.obj");
	body_model->shader = LoadShaderAsset("default");
	body_model->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));

	surface_model->model = LoadModelAsset("Resources/table_surface.obj");
	surface_model->shader = LoadShaderAsset("default");
	surface_model->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));
}
