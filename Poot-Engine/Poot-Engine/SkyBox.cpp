#include "SkyBox.h"
#include "Game.h"


SkyBox::SkyBox(const std::string cube_map_key)
	: cube_map_key(cube_map_key)
{
	SetTags(OBJ_TAG_SKYBOX);
	model_comp = MakeComponent<ModelComponent>();
}

void SkyBox::BuildComponents()
{
	model_comp->model = LoadModelAsset("skybox");
	model_comp->shader = LoadShaderAsset("skybox");
	model_comp->SetTextureUnit(0, LoadCubeMapAsset(cube_map_key));
}