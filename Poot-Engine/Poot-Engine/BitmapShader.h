#pragma once
#include "API.h"
#include "Shader.h"

class POOT_API BitmapShader : public Shader
{
private:
	class Model* quad_model = nullptr;

	GLuint uniform_model_matrix;
	GLuint uniform_frame_size;

	GLuint uniform_anchor;
	GLuint uniform_colour;

	GLuint uniform_current_char;
	GLuint uniform_letter_location;

	void DrawLetter(const char& c, glm::vec2& letter_location, const int alignment);

protected:
	virtual void Start() override;
	virtual void Stop() override;
	virtual bool Load() override;
	virtual void AmbiguousRender(const RenderRequest& request, Component* component) override;
	virtual void AttachShaders() override;
};
