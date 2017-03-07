#include "BasicBox.h"
#include <Game.h>


BasicBox::BasicBox()
{
	SetTags(OBJ_TAG_PROP);
	model = MakeComponent<ModelComponent>();
}

void BasicBox::BuildComponents()
{
	model->model = g_game->GetWindow()->GetModelLoader()["Cube"];
	model->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/planks.png"]);
}