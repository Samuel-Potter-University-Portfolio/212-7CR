#include "Pocket.h"
#include "PoolWorld.h"


Pocket::Pocket()
{
	SetTags(OBJ_TAG_PROP | POBJ_TAG_POCKET);
	model = MakeComponent<ModelComponent>();
	
	sphere = MakeComponent<SphereCollider>();
	sphere->radius = 2.0f;

	model->local_transform.scale *= sphere->radius;
}

void Pocket::BuildComponents()
{
	model->model = LoadModelAsset("ball");
	model->shader = LoadShaderAsset("default");
	model->SetTextureUnit(0, LoadTextureAsset("Resources/ball_8.png"));
	model->SetActive(false);
}
