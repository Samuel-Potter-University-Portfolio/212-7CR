#pragma once
#include "API.h"
#include "Element.h"
#include "Model.h"
#include "Shader.h"

#include <string>

class POOT_API QuadUI : public Element
{
public:
	std::string texture_key = "";

	inline GLuint GetTextureID() { return texture; }

protected:
	Shader* shader = nullptr;
	GLuint texture = 0;

	virtual void Begin() override;
	virtual void Render(RenderRequest& request);
};

