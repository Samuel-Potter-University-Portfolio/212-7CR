#include "SkyBox.h"
#include "Game.h"


SkyBox::SkyBox()
{
	model_comp = MakeComponent<ModelComponent>();
}


void SkyBox::WindowBegin() 
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["skybox"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["skybox"];
	model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader().GetCubeMap("Resources/Skybox/TropicalSunnyDay.png"));
}
