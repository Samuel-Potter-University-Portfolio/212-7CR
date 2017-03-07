#include "BasicBox.h"
#include <Game.h>


BasicBox::BasicBox()
{
	tags |= E_TAG_PROP;

	model = MakeComponent<ModelComponent>();
}

void BasicBox::WindowBegin() 
{
	Super::WindowBegin();

	model->model = g_game->GetWindow()->GetModelLoader()["Cube"];
	model->shader = g_game->GetWindow()->GetShaderLoader()["default"];
	model->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/planks.png"]);
}
