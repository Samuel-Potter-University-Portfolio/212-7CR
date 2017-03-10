#include "QuadUI.h"
#include "GameObject.h"


void QuadUI::Begin() 
{
	shader = GameObject::LoadShaderAsset("ui_quad");
	texture = GameObject::LoadTextureAsset(texture_key);
}

void QuadUI::Render(RenderRequest& request)
{
	if (!shader)
		return;

	shader->Start();
	shader->AmbiguousRender(request, this);
	shader->Stop();
}