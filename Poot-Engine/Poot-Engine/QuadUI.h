#pragma once
#include "Element.h"
#include "Model.h"
#include "Shader.h"

#include <string>

class QuadUI : public Element
{
public:
	std::string texture_key;
	GLuint texture;

protected:
	Shader* shader;

	virtual void Begin() override;
	virtual void Render(RenderRequest& request, float delta_time);
};

