#include "TextElement.h"
#include "GameObject.h"


void TextElement::Begin()
{
	shader = GameObject::LoadShaderAsset("ui_bitmap_font");
	character_sheet = GameObject::LoadTextureAsset(font_sheet_key);
}

void TextElement::Render(RenderRequest& request)
{
	if (!shader)
		return;

	shader->Start();
	shader->AmbiguousRender(request, this);
	shader->Stop();
}
