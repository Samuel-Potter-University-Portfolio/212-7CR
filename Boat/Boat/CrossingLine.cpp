#include "CrossingLine.h"
#include "Game.h"


CrossingLine::CrossingLine(CrossingLineType type)
	: type(type)
{
	tags |= E_TAG_PROP;
	model_comp = MakeComponent<ModelComponent>();
}

void CrossingLine::WindowBegin()
{
	__super::WindowBegin();
	model_comp->model = g_game->GetWindow()->GetModelLoader()["Resources/crossing_line.obj"];
	model_comp->shader = g_game->GetWindow()->GetShaderLoader()["default"];

	if(type == Finish)
		model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/finish_line.png"]);
	else
		model_comp->SetTextureUnit(0, g_game->GetWindow()->GetTextureLoader()["Resources/start_line.png"]);
}
