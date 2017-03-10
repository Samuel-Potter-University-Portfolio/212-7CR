#pragma once
#include "Element.h"
#include "Model.h"
#include "Shader.h"

#include <string>


enum TextAlignment 
{
	Left, Right
};


class POOT_API TextElement : public Element
{
public:
	std::string font_sheet_key = "";
	std::string text = "";
	TextAlignment aligment = Left;

	inline GLuint GetTextureID() { return character_sheet; }

protected:
	Shader* shader = nullptr;
	GLuint character_sheet = 0;

	virtual void Begin() override;
	virtual void Render(RenderRequest& request, float delta_time);
};

